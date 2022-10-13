#include "stdafx.h"
#include "Arena.h"

#include "Components/GridComponent.h"
#include "Prefabs/Bomb.h"
#include "Prefabs/Tile.h"
#include "Prefabs/Wall.h"
#include "Prefabs/Spikes.h"
#include "Prefabs/BomberMan.h"
#include "Prefabs/StageTimer.h"

Arena::Arena(const std::vector<XMINT2>& playerPositions, GameScene* const pScene, const XMFLOAT2& spawnPos, 
             uint32_t nrOfCols, uint32_t nrOfRows, uint32_t nrOfPlayers)
	: m_pTiles{}
	, m_pSoftWalls{}
{
	m_pPool = new Pool<Bomb>{ pScene, 20 };

	GridComponent::SetSpawnPosition(spawnPos);
	GridComponent::SetGridDimensions({ static_cast<int32_t>(nrOfCols), static_cast<int32_t>(nrOfRows) });
	GridComponent::SetGridSize(20);
	SpawnArena(pScene);
	SpawnObstacles(playerPositions, pScene, nrOfPlayers);

	//Sounds
	FMOD::System* const pSoundSystem = SoundManager::Get()->GetSystem();
	FMOD_RESULT result;
	//explosion
	result = pSoundSystem->createStream("Resources/Sounds/BombExplosion.wav", FMOD_LOOP_OFF, nullptr, &m_pSoundEffectExplosion);
	HANDLE_ERROR(result);
	//crate
	result = pSoundSystem->createStream("Resources/Sounds/WoodCrateDestroyed.wav", FMOD_LOOP_OFF, nullptr, &m_pSoundEffectCrateDestroyed);
	HANDLE_ERROR(result);
	//squish
	result = pSoundSystem->createStream("Resources/Sounds/SquishSound.wav", FMOD_LOOP_OFF, nullptr, &m_pSoundEffectSquish);
	HANDLE_ERROR(result);

	result = pSoundSystem->createChannelGroup("SoundEffects", &m_pSoundEffectGroup);
	HANDLE_ERROR(result);

	m_pSoundEffectGroup->setVolume(0.25f);
}

Arena::~Arena()
{
	SafeDelete(m_pPool);

	std::for_each(m_pSpikes.begin(), m_pSpikes.end(), [](Spikes* pSpikes)
		{
			if (pSpikes->GetScene() == nullptr)
			{
				SafeDelete(pSpikes);
				pSpikes = nullptr;
			}
		});
}

void Arena::Update(GameScene* const pScene)
{
	//ERASE REMOVE IDIOM
	m_pSoftWalls.erase(std::remove_if(m_pSoftWalls.begin(), m_pSoftWalls.end(), [pScene](Wall* pWall)
	{
		if(pWall->GetDestroyed())
		{
			pScene->RemoveChild(pWall,true);
			return true;
		}
		return false;
	}) , m_pSoftWalls.end());

	m_pHardWalls.erase(std::remove_if(m_pHardWalls.begin(), m_pHardWalls.end(), [pScene](Wall* pWall)
		{
			if (pWall->GetDestroyed())
			{
				pScene->RemoveChild(pWall, true);
				return true;
			}
			return false;
		}), m_pHardWalls.end());


	//Logger::LogDebug(L"Spawn Spike!");
	
	float deltaTime = pScene->GetSceneContext().pGameTime->GetElapsed();
	UpdateSpikes(deltaTime);
}

bool Arena::Notify(Events event, GameObject* const pObject, const XMINT2& gridPos)
{
	switch(event)
	{
	case Events::BombSpawned:
		return SpawnBomb(pObject, gridPos);
	case Events::BombExploded:
		return BombExploded(pObject, gridPos);
	case Events::PlayerMoved:
		return PlayerMoved(pObject, gridPos);
	case Events::LastOneStanding:
		return LastOneStanding(pObject, gridPos);
	case Events::Hurry:
		return StartSpikes(pObject, gridPos);
	case Events::Squished:
		return Squished(pObject, gridPos);
	default:
		return true;
	}
}

void Arena::OnSceneReset(const std::vector<XMINT2>& playerPositions, GameScene* const pScene, uint32_t nrOfPlayers)
{
	//Remove all soft walls
	std::for_each(m_pSoftWalls.begin(), m_pSoftWalls.end(), [pScene, this](Wall* pWall)
		{
			GetTile(pWall->GetComponent<GridComponent>()->GetGridPos())->SetOccupied(nullptr);
			pScene->RemoveChild(pWall, true);
		});
	m_pSoftWalls.clear();

	XMINT2 dimensions = GridComponent::GetGridDimensions();

	GameObject* pDummy = new GameObject{};

	//Add new soft walls
	SpawnSoftWalls(playerPositions, pScene, nrOfPlayers);

	SafeDelete(pDummy);

	m_Spikes = false;
	m_SpikesTimer = 0.f;

	//Put back all hard walls
	std::for_each(m_pHardWalls.begin(), m_pHardWalls.end(), [pScene, this](Wall* pWall)
	{
			if(pWall->GetScene() == nullptr)
			{
				pScene->AddChild(pWall);
			}
	});

	XMFLOAT2 spawnPos = GridComponent::GetSpawnPosition();
	uint32_t gridSize = GridComponent::GetGridSize();
	std::for_each(m_pSpikes.begin(), m_pSpikes.end(), [pScene, spawnPos, gridSize](Spikes* const pSpikes)
		{
			XMINT2 gridPos = pSpikes->GetGridComponent()->GetGridPos();
			pSpikes->GetTransform()->Translate(spawnPos.x + static_cast<float>(gridPos.x) * GridComponent::GetGridSize(),
				300.f,
				spawnPos.y + static_cast<float>(gridPos.y) * GridComponent::GetGridSize());
			pSpikes->Deactivate(true);
			//pScene->RemoveChild(pSpikes);
		});
}

bool Arena::TileAvailable(const XMINT2& gridPos) const
{
	if (!GetTile(gridPos))
		return false;

	return !GetTile(gridPos)->GetOccupied();
}

Tile* const Arena::GetTile(const XMINT2& gridPos) const
{
	if (gridPos.x >= GridComponent::GetGridDimensions().x || gridPos.y >= GridComponent::GetGridDimensions().y)
		return nullptr;

	return m_pTiles[gridPos.y * GridComponent::GetGridDimensions().x + gridPos.x];
}

Spikes* const Arena::GetSpikes(const XMINT2& gridPos) const
{
	if (gridPos.x >= GridComponent::GetGridDimensions().x || gridPos.y >= GridComponent::GetGridDimensions().y)
		return nullptr;

	return m_pSpikes[gridPos.y * GridComponent::GetGridDimensions().x + gridPos.x];
}

void Arena::SpawnArena(GameScene* const pScene)
{
	XMINT2 gridDimensions = GridComponent::GetGridDimensions();

	for (int j{ -1 }; j <= gridDimensions.y; ++j)
	{
		for (int i{ -1 }; i <= gridDimensions.x; ++i)
		{ 
			Tile* const pTile = new Tile{ i, j,abs((i + j) % 2)};
			//pTile->GetTransform()->Translate(m_SpawnPos.x + static_cast<float>(i * size), 0.f, m_SpawnPos.y + static_cast<float>(j * size));
			pScene->AddChild(pTile);
			pTile->GetComponent<GridComponent>()->SetGridPos(i, j);

			if (i == -1 || i == gridDimensions.x || j == -1 || j == gridDimensions.y)
			{
				pScene->AddChild(new Wall{ WallType::Fence, i, j });
				//pBox->GetTransform()->Translate(m_SpawnPos.x + static_cast<float>(i) * size, 0.1f, m_SpawnPos.y + static_cast<float>(j) * size);
				pTile->GetComponent<GridComponent>()->SetGridPos(-1, -1);
			}
			else
			{
				m_pTiles.push_back(pTile);

				//Create Spikes
				m_pSpikes.push_back(pScene->AddChild(new Spikes{ {i,j}, 300 }));
				m_pSpikes.back()->Deactivate(true);
			}
		}
	}
}

void Arena::SpawnObstacles(const std::vector<XMINT2>& playerPositions, GameScene* const pGameScene, uint32_t nrOfPlayers)
{
	(nrOfPlayers);
	XMINT2 gridDimensions = GridComponent::GetGridDimensions();

	for (int32_t j{ 0 }; j < gridDimensions.y; ++j)
	{
		for (int32_t i{ 0 }; i < gridDimensions.x; ++i)
		{
			std::vector<XMINT2>::const_iterator it = std::find_if(playerPositions.cbegin(), playerPositions.cend(), [i, j](const XMINT2& pos)
				{
					//check if pos is on player position or next to it
					return((pos.x == i || pos.x + 1 == i || pos.x - 1 == i) && pos.y == j) || ((pos.y  + 1 == j || pos.y - 1 == j) && pos.x == i);
				});
			if (it != playerPositions.cend())
				continue;

			if (i % 2 == 1 && j % 2 == 1)
			{
				Wall* const pBox = pGameScene->AddChild(new Wall{ WallType::Hard, i , j });
				//pBox->GetTransform()->Translate(m_SpawnPos.x + static_cast<float>(i * size), 0.1f, m_SpawnPos.y + static_cast<float>(j * size));
				GetTile({i,j})->SetOccupied(pBox);
				m_pHardWalls.push_back(pBox);
			}
			else if (!GetTile({ i, j })->GetOccupied())
			{
				if (MathHelper::randF(0.f, 1.f) < 0.8f)
				{
					Wall* const pBox = pGameScene->AddChild(new Wall{ WallType::Soft, i, j});
					//pBox->GetTransform()->Translate(m_SpawnPos.x + static_cast<float>(i * size), 0.1f, m_SpawnPos.y + static_cast<float>(j * size));
					GetTile({i,j})->SetOccupied(pBox);
					m_pSoftWalls.push_back(pBox);
				}
			}
		}
	}
}

void Arena::SpawnSoftWalls(const std::vector<XMINT2>& playerPositions, GameScene* const pGameScene, uint32_t nrOfPlayers)
{
	(nrOfPlayers);
	XMINT2 gridDimensions = GridComponent::GetGridDimensions();

	for (int32_t j{ 0 }; j < gridDimensions.y; ++j)
	{
		for (int32_t i{ 0 }; i < gridDimensions.x; ++i)
		{
			std::vector<XMINT2>::const_iterator it = std::find_if(playerPositions.cbegin(), playerPositions.cend(), [i, j](const XMINT2& pos)
				{
					//check if pos is on player position or next to it
					return((pos.x == i || pos.x + 1 == i || pos.x - 1 == i) && pos.y == j) || ((pos.y + 1 == j || pos.y - 1 == j) && pos.x == i);
				});
			if (it != playerPositions.cend())
				continue;
			else if (!GetTile({ i, j })->GetOccupied())
			{
				if (MathHelper::randF(0.f, 1.f) < 0.8f)
				{
					Wall* const pBox = pGameScene->AddChild(new Wall{ WallType::Soft, i, j });
					GetTile({ i,j })->SetOccupied(pBox);
					m_pSoftWalls.push_back(pBox);
				}
			}
		}
	}
}

void Arena::UpdateSpikes(float deltaTime)
{
	if (!m_Spikes)
		return;

	m_SpikesTimer -= deltaTime;
	if (m_SpikesTimer < 0.f)
	{
		SpawnSpike();
		m_SpikesTimer += m_SpikesInterval;
	}
}

void Arena::SpawnSpike()
{
	if (m_Horizontal) //horizontale iteration
	{
		if (m_UpLeft)
		{
			--m_GridPos.x;
		}
		else
		{
			++m_GridPos.x;
		}

		//Logger::LogDebug(L"gridPos: {}, {}", m_GridPos.x, m_GridPos.y);
		GetSpikes(m_GridPos)->Activate(true);
		
		--m_CurrentNumber;
		//Logger::LogDebug(L"currentNumber: {}", currentNumber);
		if (m_CurrentNumber == 0)
		{
			--m_Iterations;
			m_Horizontal = false;
			//Logger::LogDebug(L"next currentNumber: {}", currentNumber);
			if (m_Iterations == 1)
			{
				m_TotalNumberHorizontal -= 1;
				m_CurrentNumber = m_TotalNumberVertical;
			}
			if (m_Iterations == 0)
			{
				m_TotalNumberHorizontal -= 1;
				m_Iterations = 1;
				m_CurrentNumber = m_TotalNumberVertical;
			}
		}
	}
	else //verticale iteration
	{
		if (m_UpLeft)
		{
			++m_GridPos.y;
		}
		else
		{
			--m_GridPos.y;
		}

		//Logger::LogDebug(L"gridPos: {}, {}", m_GridPos.x, m_GridPos.y);
		GetSpikes(m_GridPos)->Activate(true);
		--m_CurrentNumber;
		//Logger::LogDebug(L"currentNumber: {}", currentNumber);
		if (m_CurrentNumber == 0)
		{
			--m_Iterations;
			m_UpLeft = !m_UpLeft;
			m_Horizontal = true;
			//Logger::LogDebug(L"next currentNumber: {}", currentNumber);
			if (m_Iterations == 0)
			{
				m_Iterations = 1;
				m_TotalNumberVertical -= 1;
				m_CurrentNumber = m_TotalNumberHorizontal;
			}
		}
	}
}

bool Arena::SpawnBomb(GameObject* const pObject, const XMINT2& gridPos)
{
	Tile* pTile = GetTile(gridPos);
	if (!pTile)
		return false;
	if (pTile->GetOccupied())
		return false;

	XMFLOAT3 pos = pObject->GetTransform()->GetPosition();
	pos.y = 0.f;

	Bomb* pBomb = m_pPool->ActivateElement(pos);
	if(typeid(*pObject) == typeid(BomberMan))
	{
		pBomb->SetPlayerReference(reinterpret_cast<BomberMan*>(pObject));
	}
	else
	{
#ifdef _DEBUG
		Logger::LogWarning(L"given object with notify is not from BomberMan class");
#endif
	}

	pTile->SetOccupied(pBomb);
	return true;
}

bool Arena::BombExploded(GameObject* const pObject, const XMINT2& gridPos)
{
	if (typeid(*pObject) != typeid(Bomb))
		return false;

	Bomb* const pBomb = reinterpret_cast<Bomb*>(pObject);
	m_pPool->DeactivateElement(pBomb);

	SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectExplosion, m_pSoundEffectGroup, false, nullptr);


	int32_t explosionRange = static_cast<int>(pBomb->GetExplosionRange());
	GetTile(gridPos)->SetOccupied(nullptr);

	for(int32_t i{ 1 }; i < 5; ++i)
	{
		ClearTile(explosionRange, i, gridPos);
	}

	return true;
}

bool Arena::PlayerMoved(GameObject* const pObject, const XMINT2& gridPos)
{
	if(typeid(*pObject) == typeid(BomberMan))
	{
		if (!GetTile(gridPos))
			return false;

		if (!GetTile(gridPos)->GetSlot())
			return false;

		if(typeid(*GetTile(gridPos)->GetSlot()) == typeid(BomberMan))
			GetTile(gridPos)->SetOccupied(nullptr);
		BomberMan* const pPlayer = reinterpret_cast<BomberMan*>(pObject);
		XMINT2 currentGridPos = pPlayer->GetComponent<GridComponent>()->GetGridPos();
		if (!GetTile(currentGridPos))
			return false;
		GetTile(currentGridPos)->SetOccupied(pObject);
		return true;
	}
	return false;
}

bool Arena::LastOneStanding(GameObject* const pObject, const XMINT2& gridPos)
{
	(gridPos);
	if (!pObject)
		return false;
	if (typeid(*pObject) == typeid(BomberMan))
	{
		BomberMan* const pPlayer = reinterpret_cast<BomberMan*>(pObject);
		pPlayer->Won();

		return true;
	}
	return false;
}

bool Arena::StartSpikes(GameObject* const pObject, const XMINT2&)
{
	if (typeid(*pObject) != typeid(StageTimer))
		return false;

	StageTimer* pTimer = reinterpret_cast<StageTimer*>(pObject);

	m_SpikesInterval = pTimer->GetRemainingTime() / (GridComponent::GetGridDimensions().x * GridComponent::GetGridDimensions().y);
	m_SpikesTimer = m_SpikesInterval;
	m_GridPos.x = GridComponent::GetGridDimensions().x;
	m_GridPos.y = 0;
	m_TotalNumberHorizontal = GridComponent::GetGridDimensions().x;
	m_TotalNumberVertical = GridComponent::GetGridDimensions().y - 1;
	m_Iterations = 2;
	m_CurrentNumber = m_TotalNumberHorizontal;
	m_Horizontal = true;
	m_UpLeft = true;
	m_Spikes = true;

	return true;
}

bool Arena::Squished(GameObject* const pObject, const XMINT2& gridPos)
{
	if (typeid(*pObject) != typeid(Spikes))
		return false;

	Tile* const pTile = GetTile(gridPos);
	GameObject* const pSlot = pTile->GetSlot();
	if (pSlot == nullptr)
	{
		return true;
	}

	ASSERT_NULL_(pSlot);

	if (typeid(*pSlot) == typeid(Wall))
	{
		Wall* pWallSlot = reinterpret_cast<Wall*>(pSlot);
		if (pWallSlot->Squish())
		{
			pTile->SetOccupied(pObject);
			SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectSquish, m_pSoundEffectGroup, false, nullptr);
		}
	}
	if (typeid(*pSlot) == typeid(Bomb))
	{
		Bomb* pBombSlot = reinterpret_cast<Bomb*>(pSlot);
		m_pPool->DeactivateElement(pBombSlot);
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectSquish, m_pSoundEffectGroup, false, nullptr);
		pTile->SetOccupied(pObject);
	}
	if (typeid(*pSlot) == typeid(BomberMan))
	{
		BomberMan* pPlayerSlot = reinterpret_cast<BomberMan*>(pSlot);
		pTile->SetOccupied(pObject);
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectSquish, m_pSoundEffectGroup, false, nullptr);
		pPlayerSlot->GameOver();
	}
	return false;
}

void Arena::ClearTile(int32_t explosionRange, int32_t direction, XMINT2 gridPos)
{
	--explosionRange;

	if (explosionRange < 0)
		return;

	XMINT2 dimensions = GridComponent::GetGridDimensions();
	//new grid pos + check if new grid pos is valid
	switch (direction)
	{
	case 1: //go right
		gridPos.x += 1;
		if (gridPos.x >= dimensions.x)
		{
			return;
		}
		break;
	case 2: //go left
		gridPos.x -= 1;
		if (gridPos.x < 0)
		{
			return;
		}
		break;
	case 3: //go up
		gridPos.y += 1;
		if (gridPos.y >= dimensions.y)
		{
			return;
		}
		break;
	case 4: //go down
		gridPos.y -= 1;
		if (gridPos.y < 0)
		{
			return;
		}
		break;
	default:
		break;
	}

	Tile* const pTile = GetTile(gridPos);
	GameObject* const pSlot = pTile->GetSlot();
	if(pSlot == nullptr)
	{
		//if tile empty go to next one
		ClearTile(explosionRange, direction, gridPos);
		return;
	}

	ASSERT_NULL_(pSlot);

	if (typeid(*pSlot) == typeid(Wall))
	{
		Wall* pWallSlot = reinterpret_cast<Wall*>(pSlot);
		if (pWallSlot->Destroy())
		{
			pTile->SetOccupied(nullptr);
			SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectCrateDestroyed, m_pSoundEffectGroup, false, nullptr);
			ClearTile(explosionRange, direction, gridPos);
		}
	}
	if (typeid(*pSlot) == typeid(Bomb))
	{
		Bomb* pBombSlot = reinterpret_cast<Bomb*>(pSlot);
		pTile->SetOccupied(nullptr);
		Notify(Events::BombExploded, pBombSlot, gridPos);
	}
	if (typeid(*pSlot) == typeid(BomberMan))
	{
		BomberMan* pPlayerSlot = reinterpret_cast<BomberMan*>(pSlot);
		pPlayerSlot->GameOver();
	}
}
