#include "stdafx.h"
#include "UI.h"

#include "Scenes/Exam/BombermanScene.h"
#include "Scenes/Exam/MainMenuScene.h"

UI::~UI()
{
	for (uint32_t i{ 0 }; i < 4; ++i)
	{
		for (uint32_t j{ 0 }; j < BombermanScene::GetRounds(); ++j)
		{
			if (m_PlayerElements[i].stars[j].pSprite && !m_PlayerElements[i].stars[j].pSprite->GetScene())
				SafeDelete(m_PlayerElements[i].stars[j].pSprite);
		}
		if (!m_PlayerElements[i].image.pSprite->GetScene())
			SafeDelete(m_PlayerElements[i].image.pSprite);
	}
}

void UI::Initialize(GameScene* const, const SceneContext& sceneContext)
{
	m_NrOfTexts = 0;
	m_NrOfSprites = 0;
	m_pFonts.push_back(ContentManager::Load<SpriteFont>(L"SpriteFonts/SuperSonicRocketShip_60.fnt"));
	m_pFonts.push_back(ContentManager::Load<SpriteFont>(L"SpriteFonts/SuperSonicRocketShip_52.fnt"));
	m_pFonts.push_back(ContentManager::Load<SpriteFont>(L"SpriteFonts/SuperSonicRocketShip_40.fnt"));
	m_pFonts.push_back(ContentManager::Load<SpriteFont>(L"SpriteFonts/SuperSonicRocketShip_32.fnt"));
	m_PlayerSpawnPositions.push_back(std::make_pair<XMFLOAT3, XMFLOAT3>({ 115.f, 0.f, 200.f }, { 93.f, 200.f, 0.95f }));
	m_PlayerSpawnPositions.push_back(std::make_pair<XMFLOAT3, XMFLOAT3>({ 1075.f, 0.f, 200.f }, { sceneContext.windowWidth - 140.f, 200.f, 0.95f }));
	m_PlayerSpawnPositions.push_back(std::make_pair<XMFLOAT3, XMFLOAT3>({ 115.f, 0.f, 400.f }, { 93.f, 400.f, 0.95f }));
	m_PlayerSpawnPositions.push_back(std::make_pair<XMFLOAT3, XMFLOAT3>({ 1075.f, 0.f, 400.f }, { sceneContext.windowWidth - 140.f, 400.f, 0.95f }));

}

void UI::Update(GameScene* const pScene)
{
	for(const TextElementUI& element : m_TextElements)
	{
		TextRenderer::Get()->DrawText(m_pFonts[element.idFont], element.text, {element.position.x, element.position.z}, element.colour);
	}
	
	for(const SpriteElementUI& element : m_SpriteElements)
	{
		element.pSprite->GetTransform()->Translate(element.position);
	}

	for(uint32_t i{ 0 }; i < 4; ++i)
	{
		if(i >= MainMenuScene::GetNrOfPlayers())
		{
			//m_PlayerElements[i].image.pSprite->Deactivate(true);
			if(m_PlayerElements[i].image.pSprite->GetScene())
				pScene->RemoveChild(m_PlayerElements[i].image.pSprite);
			for (uint32_t j{ 0 }; j < BombermanScene::GetRounds(); ++j)
			{
				//m_PlayerElements[i].stars[j].pSprite->Deactivate(true);
				if (m_PlayerElements[i].stars[j].pSprite && m_PlayerElements[i].stars[j].pSprite->GetScene())
					pScene->RemoveChild(m_PlayerElements[i].stars[j].pSprite);
			}
			continue;
		}

		TextRenderer::Get()->DrawText(m_pFonts[m_PlayerElements[i].name.idFont], m_PlayerElements[i].name.text, { m_PlayerElements[i].name.position.x, m_PlayerElements[i].name.position.z}, m_PlayerElements[i].name.colour);
		m_PlayerElements[i].image.pSprite->GetTransform()->Translate(m_PlayerElements[i].image.position);
		for(uint32_t j{ 0 }; j < BombermanScene::GetRounds(); ++j)
		{
			if(m_PlayerElements[i].stars[j].pSprite)
				m_PlayerElements[i].stars[j].pSprite->GetTransform()->Translate(m_PlayerElements[i].stars[j].position);
		}
	}
}

void UI::UpdatePlayerInfo(uint32_t id)
{
	PlayerUI& player = m_PlayerElements[id];
	++player.roundsWon;
	player.stars[player.roundsWon-1].pSprite->GetComponent<SpriteComponent>()->SetTexture(L"UI/UI_FullStar.png");
}

void UI::ResetPlayerInfo(uint32_t id)
{
	PlayerUI& player = m_PlayerElements[id];
	player.roundsWon = 0;

	for (uint32_t i{ 0 }; i < BombermanScene::GetRounds(); ++i)
	{
		player.stars[i].pSprite->GetComponent<SpriteComponent>()->SetTexture(L"UI/UI_EmptyStar.png");
	}
}

void HUD::Initialize(GameScene* const pScene, const SceneContext& sceneContext)
{
	UI::Initialize(pScene, sceneContext);

	TextElementUI timer{};
	timer.position = { sceneContext.windowWidth / 2.f - 40.f, 0.f, 30.f };
	timer.colour = static_cast<XMFLOAT4>(Colors::White);
	timer.text = L"00:00";
	timer.idFont = 0;
	AddTextUIElement(timer);

	for(uint32_t i{ 0 }; i < 4; ++i)
	{
		PlayerUI player{};

		//text
		player.name.position = m_PlayerSpawnPositions[i].first;
		player.name.colour = static_cast<XMFLOAT4>(Colors::White);
		player.name.text = L"Player " + std::to_wstring(i + 1);
		player.name.idFont = 3;

		//image
		player.image.position = m_PlayerSpawnPositions[i].second;
		player.image.colour = static_cast<XMFLOAT4>(Colors::White);
		player.image.pSprite = pScene->AddChild(new GameObject{});
		player.image.pSprite->AddComponent(new SpriteComponent{ L"UI/UI_Player_" + std::to_wstring(i + 1) + L".png", {0.5f,0.5f}, {1.f,1.f,1.f,1.f}});
		player.image.pSprite->GetTransform()->Scale(0.5f);
		//stars
		for (uint32_t j{ 0 }; j < BombermanScene::GetRounds(); ++j)
		{
			XMFLOAT3 startpos = player.image.position;
			player.stars[j].position = { startpos.x - 15.f + j * 40.f , startpos.y + 45.f, 0.9f };
			player.stars[j].pSprite = pScene->AddChild(new GameObject{});
			player.stars[j].pSprite->AddComponent(new SpriteComponent{ L"UI/UI_EmptyStar.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
			player.stars[j].pSprite->GetTransform()->Scale(0.5f);
		}

		m_PlayerElements.push_back(player);
	}


//#pragma region PlayerOne
//	PlayerUI playerOne{};
//	//text
//	playerOne.name.position = { 115.f, 0.f, 200.f };
//	playerOne.name.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerOne.name.text = L"Player 1";
//	playerOne.name.idFont = 3;
//
//	//image
//	playerOne.image.position = { 93.f, 200.f, 0.95f };
//	playerOne.image.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerOne.image.pSprite = pScene->AddChild(new GameObject{});
//	playerOne.image.pSprite->AddComponent(new SpriteComponent{ L"UI/UI_Player_1.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//	playerOne.image.pSprite->GetTransform()->Scale(0.5f);
//	//stars
//	for (int i{ 0 }; i < 3; ++i)
//	{
//		XMFLOAT3 startpos = playerOne.image.position;
//		playerOne.stars[i].position = { startpos.x - 15.f + i * 40.f , startpos.y + 45.f, 0.9f };
//		playerOne.stars[i].pSprite = pScene->AddChild(new GameObject{});
//		playerOne.stars[i].pSprite->AddComponent(new SpriteComponent{ L"UI/UI_EmptyStar.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//		playerOne.stars[i].pSprite->GetTransform()->Scale(0.5f);
//	}
//
//	m_PlayerElements.push_back(playerOne);
//#pragma endregion
//
//#pragma region PlayerTwo
//	PlayerUI playerTwo{};
//	//text
//	playerTwo.name.position = { 1075.f, 0.f, 200.f };
//	playerTwo.name.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerTwo.name.text = L"Player 2";
//	playerTwo.name.idFont = 3;
//	//image
//	playerTwo.image.position = { sceneContext.windowWidth - 140.f, 200.f, 0.95f };
//	playerTwo.image.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerTwo.image.pSprite = pScene->AddChild(new GameObject{});
//	playerTwo.image.pSprite->AddComponent(new SpriteComponent{ L"UI/UI_Player_2.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//	playerTwo.image.pSprite->GetTransform()->Scale(0.5f);
//	//stars
//	for(int i{0}; i <3; ++i)
//	{
//		XMFLOAT3 startpos = playerTwo.image.position;
//		playerTwo.stars[i].position = { startpos.x - 15.f + i * 40.f , startpos.y + 45.f, 0.9f };
//		playerTwo.stars[i].pSprite = pScene->AddChild(new GameObject{});
//		playerTwo.stars[i].pSprite->AddComponent(new SpriteComponent{ L"UI/UI_EmptyStar.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//		playerTwo.stars[i].pSprite->GetTransform()->Scale(0.5f);
//	}
//
//	m_PlayerElements.push_back(playerTwo);
//#pragma endregion
//
//#pragma region PlayerThree
//	PlayerUI playerThree{};
//	//text
//	playerThree.name.position = { 115.f, 0.f, 400.f };
//	playerThree.name.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerThree.name.text = L"Player 3";
//	playerThree.name.idFont = 3;
//	//image
//	playerThree.image.position = { 93.f, 400.f, 0.95f };
//	playerThree.image.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerThree.image.pSprite = pScene->AddChild(new GameObject{});
//	playerThree.image.pSprite->AddComponent(new SpriteComponent{ L"UI/UI_Player_3.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//	playerThree.image.pSprite->GetTransform()->Scale(0.5f);
//	//stars
//	for (int i{ 0 }; i < 3; ++i)
//	{
//		XMFLOAT3 startpos = playerThree.image.position;
//		playerThree.stars[i].position = { startpos.x - 15.f + i * 40.f , startpos.y + 45.f, 0.9f };
//		playerThree.stars[i].pSprite = pScene->AddChild(new GameObject{});
//		playerThree.stars[i].pSprite->AddComponent(new SpriteComponent{ L"UI/UI_EmptyStar.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//		playerThree.stars[i].pSprite->GetTransform()->Scale(0.5f);
//	}
//	m_PlayerElements.push_back(playerThree);
//#pragma endregion
//
//#pragma region PlayerFour
//	PlayerUI playerFour{};
//	//text
//	playerFour.name.position = { 1075.f, 0.f, 400.f };
//	playerFour.name.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerFour.name.text = L"Player 4";
//	playerFour.name.idFont = 3;
//	//image
//	playerFour.image.position = { sceneContext.windowWidth - 140.f, 400.f, 0.95f };
//	playerFour.image.colour = static_cast<XMFLOAT4>(Colors::White);
//	playerFour.image.pSprite = pScene->AddChild(new GameObject{});
//	playerFour.image.pSprite->AddComponent(new SpriteComponent{ L"UI/UI_Player_4.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//	playerFour.image.pSprite->GetTransform()->Scale(0.5f);
//	//stars
//	for (int i{ 0 }; i < 3; ++i)
//	{
//		XMFLOAT3 startpos = playerFour.image.position;
//		playerFour.stars[i].position = { startpos.x - 15.f + i * 40.f , startpos.y + 45.f, 0.9f };
//		playerFour.stars[i].pSprite = pScene->AddChild(new GameObject{});
//		playerFour.stars[i].pSprite->AddComponent(new SpriteComponent{ L"UI/UI_EmptyStar.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
//		playerFour.stars[i].pSprite->GetTransform()->Scale(0.5f);
//	}
//
//	m_PlayerElements.push_back(playerFour);
//#pragma endregion

	SpriteElementUI timerSprite{};
	timerSprite.position = { sceneContext.windowWidth / 2.f - 75.f , 60.f, 0.95f };
	timerSprite.colour = static_cast<XMFLOAT4>(Colors::White);
	timerSprite.pSprite = pScene->AddChild(new GameObject{});
	timerSprite.pSprite->AddComponent(new SpriteComponent{ L"UI/UI_Timer.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f } });
	timerSprite.pSprite->GetTransform()->Scale(0.5f);
	AddSpriteUIElement(timerSprite);
}
