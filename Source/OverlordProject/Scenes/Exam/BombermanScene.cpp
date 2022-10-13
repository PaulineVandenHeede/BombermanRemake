#include "stdafx.h"
#include "BombermanScene.h"

#include "Components/GridComponent.h"
#include "Prefabs/BomberMan.h"
#include "Prefabs/Spikes.h"
#include "Prefabs/Tile.h"
#include "Arena.h"
#include "MainMenuScene.h"
#include "UI/UI.h"
#include "Materials/Post/Backdrop.h"

uint32_t BombermanScene::m_Rounds{ 3 };
uint32_t BombermanScene::m_PlayerWonID{ 0 };

BombermanScene::BombermanScene()
	: GameScene{ L"BombermanScene" }
	, m_State(StageState::Normal)
	, m_MaxNumberOfPlayers(4)
	, m_NrOfActivePlayers(4)
{
	
}

BombermanScene::~BombermanScene()
{
	SafeDelete(m_pArena);
	SafeDelete(m_pHUD);

	for(int i{ 0 }; i < 4; ++i)
	{
		if (!m_pCharacters[i]->GetScene())
			SafeDelete(m_pCharacters[i]);
	}
}

bool BombermanScene::NotifyArena(Events event, GameObject* const pObject, const XMINT2& gridPos)
{
	//(event); (pObject); (gridPos);
	return m_pArena->Notify(event, pObject, gridPos);
	//return false;
}

uint32_t BombermanScene::GetPlayersActive() const
{
	return m_NrOfActivePlayers;
}

void BombermanScene::ResetScene()
{
	//Reset Players
	m_NrOfActivePlayers = m_MaxNumberOfPlayers;
	for(uint32_t i{ 0 }; i < m_MaxNumberOfPlayers; ++i)
	{
		BomberMan* const pPlayer = m_pCharacters[i];
		if(!pPlayer->GetScene())
		{
			AddChild(pPlayer);
		}
		XMFLOAT2 gridSpawnPos = GridComponent::GetSpawnPosition();
		XMINT2 spawnPos{ m_SpawnPositions[i] };
		pPlayer->GetTransform()->Translate(gridSpawnPos.x + static_cast<float>(spawnPos.x) * GridComponent::GetGridSize(),
			17.75f,
			gridSpawnPos.y + static_cast<float>(spawnPos.y) * GridComponent::GetGridSize());
	}
	
	//Reset Arena
	m_pArena->OnSceneReset(m_SpawnPositions, this, m_NrOfActivePlayers - 1);

	m_pTimer->Reset();
}

void BombermanScene::Initialize()
{
	m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawPhysXDebug = false;

	GameSceneExt::CreatePhysXGroundPlane(*this);

	//ARENA
	uint16_t tileSize = 20;
	XMINT2 gridDimensions{ 17,11 };
	Tile::SetSize(tileSize);
	XMFLOAT2 startPos{ tileSize / 2.f, tileSize / 2.f };
	GridComponent::SetGridDimensions(gridDimensions);
	GridComponent::SetGridSize(tileSize);

	//PLAYER SPAWN POSITIONS
	m_SpawnPositions.push_back({ 0,0 });
	m_SpawnPositions.push_back({ gridDimensions.x - 1, gridDimensions.y - 1 });
	m_SpawnPositions.push_back({ 0, gridDimensions.y - 1 });
	m_SpawnPositions.push_back({ gridDimensions.x - 1, 0 });

	m_pArena = new Arena{m_SpawnPositions, this, startPos, static_cast<uint32_t>(gridDimensions.x), static_cast<uint32_t>(gridDimensions.y), m_NrOfActivePlayers };
	
	//PLAYER(S)
	const auto pxDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	for(uint32_t i{ 0 }; i < m_NrOfActivePlayers; ++i)
	{
		CharacterDesc desc{ pxDefaultMaterial, 8.f, 19.f };
		desc.maxMoveSpeed = 45.f;
		desc.maxFallSpeed = 45.f;
		XMINT2 spawnPos = m_SpawnPositions[i];
		m_pCharacters.push_back(AddChild(new BomberMan{desc, spawnPos.x, spawnPos.y, i}));
		m_pArena->GetTile(spawnPos)->SetOccupied(m_pCharacters.back());
	}

	//CAMERA
	FixedCamera* const pCamera = new FixedCamera();
	pCamera->GetTransform()->Translate(tileSize * gridDimensions.x / 2.f, 350.f, -10.f);
	pCamera->GetTransform()->Rotate(70.f, 0.f, 0.f);
	/*pCamera->GetTransform()->Translate(tileSize, 20.f, -50.f);
	pCamera->GetTransform()->Rotate(0.f, 0.f, 0.f);*/
	AddChild(pCamera);

	SetActiveCamera(pCamera->GetComponent<CameraComponent>()); //Also sets pCamera in SceneContext

	m_SceneContext.pLights->SetDirectionalLight(pCamera->GetTransform()->GetPosition(), pCamera->GetTransform()->GetForward());

	//UI
	m_pHUD = new HUD{};
	m_pHUD->Initialize(this, m_SceneContext);

	//TIMER
	m_pTimer = AddChild(new StageTimer{ 120.f });

	//POST
	m_pBackdrop = MaterialManager::Get()->CreateMaterial<Backdrop>();
	m_pBackdrop->SetBackdropTexture(L"Textures/backdrop.jpg");
	m_pBackdrop->SetClearColour(m_SceneContext.settings.clearColor);
}

void BombermanScene::Update()
{
	const float deltaTime = m_SceneContext.pGameTime->GetElapsed(); (deltaTime);
	m_pHUD->UpdateText(0, m_pTimer->GetAsText());

	
	if(startup)
	{
		m_MaxNumberOfPlayers = MainMenuScene::GetNrOfPlayers();
		m_NrOfActivePlayers = m_MaxNumberOfPlayers;

		for(int i{ static_cast<int>(m_NrOfActivePlayers) }; i < 4; ++i)
		{
			m_pArena->GetTile(m_pCharacters[i]->GetGridComponent()->GetGridPos())->SetOccupied(nullptr);
			//RemoveChild(m_pCharacters[i]);
			m_pCharacters[i]->Deactivate(true);
		}

		startup = false;
	}

	m_pArena->Update(this);

	//Character update
	std::for_each(m_pCharacters.begin(), m_pCharacters.end(), [this](BomberMan* pPlayer)
		{
			if(pPlayer->GetState() == ObjectState::Destroy)
			{
				RemoveChild(pPlayer);
				--m_NrOfActivePlayers;
			}
		});
	if (m_NrOfActivePlayers <= 1)
	{
		std::vector<BomberMan*>::const_iterator it= std::find_if_not(m_pCharacters.cbegin(), m_pCharacters.cend(), [](BomberMan* pPlayer) { return pPlayer->GetState() == ObjectState::Inactive; });
		m_pArena->Notify(Events::LastOneStanding, *it, (*it)->GetGridComponent()->GetGridPos());
		m_pHUD->UpdatePlayerInfo(static_cast<uint32_t>((*it)->GetPlayerID()));
		ResetScene();
		Logger::LogInfo(L"Round over!");
		Logger::LogTodo(L"Win sequence");

		if((*it)->GetRoundsWon() == m_Rounds)
		{
			m_PlayerWonID = static_cast<uint32_t>((*it)->GetPlayerID());
			SceneManager::Get()->SetActiveGameScene(L"WonMenuScene");
		}

	}
	m_pHUD->Update(this);
}

void BombermanScene::OnGUI()
{
	PlayerUI player = m_pHUD->GetPlayerUI(0);
	ImGui::SliderFloat3("Position", ConvertUtil::ToImFloatPtr(player.stars[0].position), 0, m_SceneContext.windowWidth);
	m_pHUD->UpdatePosPlayer(1, player.stars[0].position);

	/*ImGui::Text("Player One");
	ImGui::Text("Text");
	TextElementUI textPlayer1 = m_pHUD->GetTextElement(1);
	ImGui::SliderFloat3("Position", ConvertUtil::ToImFloatPtr(textPlayer1.position), 0, m_SceneContext.windowWidth);
	m_pHUD->UpdatePosText(1, textPlayer1.position);

	ImGui::Text("Player Two");
	ImGui::Text("Text");
	TextElementUI textPlayer2 = m_pHUD->GetTextElement(2);
	ImGui::SliderFloat3("Position", ConvertUtil::ToImFloatPtr(textPlayer2.position), 0, m_SceneContext.windowWidth);
	m_pHUD->UpdatePosText(2, textPlayer2.position);

	ImGui::Text("Player Three");
	ImGui::Text("Text");
	TextElementUI textPlayer3 = m_pHUD->GetTextElement(3);
	ImGui::SliderFloat3("Position", ConvertUtil::ToImFloatPtr(textPlayer3.position), 0, m_SceneContext.windowWidth);
	m_pHUD->UpdatePosText(3, textPlayer3.position);

	ImGui::Text("Player Four");
	ImGui::Text("Text");
	TextElementUI textPlayer4 = m_pHUD->GetTextElement(4);
	ImGui::SliderFloat3("Position", ConvertUtil::ToImFloatPtr(textPlayer4.position), 0, m_SceneContext.windowWidth);
	m_pHUD->UpdatePosText(4, textPlayer4.position);*/

}

void BombermanScene::PostDraw()
{
	//ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 200.f, 20.f }, { 0.3f, 0.3f }, { 1.f,0.f });
}

void BombermanScene::OnSceneActivated()
{
	AddPostProcessingEffect(m_pBackdrop);
	m_pBackdrop->SetBackdropTexture(L"Textures/backdrop.jpg");
	m_MaxNumberOfPlayers = MainMenuScene::GetNrOfPlayers();
	m_NrOfActivePlayers = m_MaxNumberOfPlayers;

	for(BomberMan* pPlayer : m_pCharacters)
	{
		pPlayer->Reset();
		m_pHUD->ResetPlayerInfo(static_cast<uint32_t>(pPlayer->GetPlayerID()));
	}
}

void BombermanScene::OnSceneDeactivated()
{
	RemovePostProcessingEffect(m_pBackdrop);
}
