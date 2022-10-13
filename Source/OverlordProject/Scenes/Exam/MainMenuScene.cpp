#include "stdafx.h"
#include "MainMenuScene.h"

#include "Materials/Post/Backdrop.h"
#include "Prefabs/BomberMan.h"

uint32_t MainMenuScene::m_NrOfPlayers{ 0 };

MainMenuScene::MainMenuScene()
	: GameScene{ L"MainMenuScene"}
{
}

void MainMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawPhysXDebug = false;

	m_Interval = 2.f;
	m_IntervalShort = 1.f;
	m_Timer = m_Interval;
	m_State = MainMenuState::begin;
	m_NrOfPlayers = 0;

	//CAMERA
	FixedCamera* const pCamera = new FixedCamera();
	pCamera->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, -30.f);
	AddChild(pCamera);
	SetActiveCamera(pCamera->GetComponent<CameraComponent>()); //Also sets pCamera in SceneContext


	//Text element
	m_pFontBig = ContentManager::Load<SpriteFont>(L"SpriteFonts/SuperSonicRocketShip_60.fnt");
	m_Text.position = { m_SceneContext.windowWidth / 2.f - 250.f, 0.f, 100.f };
	m_Text.colour = static_cast<XMFLOAT4>(Colors::White);
	m_Text.text = L"Press ENTER/B/O to continue";
	m_Text.idFont = 0;

	m_pFontNormal = ContentManager::Load<SpriteFont>(L"SpriteFonts/SuperSonicRocketShip_32.fnt");
	m_AddPlayerOne.position = { 110.f, 0.f, 325.f };
	m_AddPlayerOne.colour = static_cast<XMFLOAT4>(Colors::White);
	m_AddPlayerOne.text = L"Press A to add player";
	m_AddPlayerOne.idFont = 0;

	m_AddPlayerTwo.position = { 395.f, 0.f, 325.f };
	m_AddPlayerTwo.colour = static_cast<XMFLOAT4>(Colors::White);
	m_AddPlayerTwo.colour.w = 0.5f;
	m_AddPlayerTwo.text = L"Press A to add player";
	m_AddPlayerTwo.idFont = 0;

	m_AddPlayerThree.position = { 662.f, 0.f, 325.f };
	m_AddPlayerThree.colour = static_cast<XMFLOAT4>(Colors::White);
	m_AddPlayerThree.colour.w = 0.5f;
	m_AddPlayerThree.text = L"Press A to add player";
	m_AddPlayerThree.idFont = 0;

	m_AddPlayerFour.position = { 935.f, 0.f, 325.f };
	m_AddPlayerFour.colour = static_cast<XMFLOAT4>(Colors::White);
	m_AddPlayerFour.colour.w = 0.5f;
	m_AddPlayerFour.text = L"Press A to add player";
	m_AddPlayerFour.idFont = 0;

	//input
	m_SceneContext.pInput->AddInputAction(InputAction{ enter, InputState::pressed, VK_RETURN, -1, XINPUT_GAMEPAD_B });
	m_SceneContext.pInput->AddInputAction(InputAction{ back, InputState::pressed, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });
	m_SceneContext.pInput->AddInputAction(InputAction{ next, InputState::pressed, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.pInput->AddInputAction(InputAction{ down, InputState::pressed, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.pInput->AddInputAction(InputAction{ up, InputState::pressed, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP });
	m_SceneContext.pInput->AddInputAction(InputAction{ addPlayerOne, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, GamepadIndex::playerOne });
	m_SceneContext.pInput->AddInputAction(InputAction{ addPlayerTwo, InputState::pressed, VK_RETURN, -1, XINPUT_GAMEPAD_A, GamepadIndex::playerTwo });
	m_SceneContext.pInput->AddInputAction(InputAction{ addPlayerThree, InputState::pressed, -1, -1, XINPUT_GAMEPAD_A, GamepadIndex::playerThree });
	m_SceneContext.pInput->AddInputAction(InputAction{ addPlayerFour, InputState::pressed, -1, -1, XINPUT_GAMEPAD_A, GamepadIndex::playerFour });


	//characters
	const auto pxDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	CharacterDesc desc{ pxDefaultMaterial, 8.f, 19.f };
	desc.maxMoveSpeed = 0.f;
	desc.maxFallSpeed = 0.f;
	m_pPlayerOne = AddChild(new BomberMan{ desc, -5, 0, 0 });
	m_pPlayerOne->GetTransform()->Translate(630.25f, m_SceneContext.windowHeight / 2.f + 3.f, -10.f);
	m_pPlayerOne->GetTransform()->Rotate(0.f, 180.f, 0.f);
	m_pPlayerOne->GetTransform()->Scale(0.125f);
	m_pPlayerOne->SetState(ObjectState::Inactive);
	m_pPlayerOne->Deactivate(true);

	m_pPlayerTwo = AddChild(new BomberMan{ desc, -5, 0, 1 });
	m_pPlayerTwo->GetTransform()->Translate(636.75f, m_SceneContext.windowHeight / 2.f + 3.f, -10.f);
	m_pPlayerTwo->GetTransform()->Rotate(0.f, 180.f, 0.f);
	m_pPlayerTwo->GetTransform()->Scale(0.125f);
	m_pPlayerTwo->SetState(ObjectState::Inactive);
	m_pPlayerTwo->Deactivate(true);

	m_pPlayerThree = AddChild(new BomberMan{ desc, -5, 0, 2 });
	m_pPlayerThree->GetTransform()->Translate(643.f, m_SceneContext.windowHeight / 2.f + 3.f, -10.f);
	m_pPlayerThree->GetTransform()->Rotate(0.f, 180.f, 0.f);
	m_pPlayerThree->GetTransform()->Scale(0.125f);
	m_pPlayerThree->SetState(ObjectState::Inactive);
	m_pPlayerThree->Deactivate(true);

	m_pPlayerFour = AddChild(new BomberMan{ desc, -5, 0, 3 });
	m_pPlayerFour->GetTransform()->Translate(649.5f, m_SceneContext.windowHeight / 2.f + 3.f, -10.f);
	m_pPlayerFour->GetTransform()->Rotate(0.f, 180.f, 0.f);
	m_pPlayerFour->GetTransform()->Scale(0.125f);
	m_pPlayerFour->SetState(ObjectState::Inactive);
	m_pPlayerFour->Deactivate(true);

	//post
	m_pBackdrop = MaterialManager::Get()->CreateMaterial<Backdrop>();
	m_pBackdrop->SetBackdropTexture(L"UI/MainMenu.png");
	m_pBackdrop->SetClearColour(m_SceneContext.settings.clearColor);

	//sound
	FMOD::System* const pSoundSystem = SoundManager::Get()->GetSystem();
	FMOD_RESULT result;
	//Dying
	result = pSoundSystem->createStream("Resources/Sounds/book page turn.wav", FMOD_DEFAULT, nullptr, &m_pSoundEffectNext);
	HANDLE_ERROR(result);
	//dropping
	result = pSoundSystem->createStream("Resources/Sounds/Click.wav", FMOD_DEFAULT, nullptr, &m_pSoundEffectMove);
	HANDLE_ERROR(result);

	result = pSoundSystem->createChannelGroup("SoundEffects", &m_pSoundEffectGroup);
	HANDLE_ERROR(result);

	m_pSoundEffectGroup->setVolume(0.5f);
}

void MainMenuScene::Update()
{
	switch(m_State)
	{
	case begin:
		UpdateBegin();
		break;
	case main1:
		UpdateMenu1();
		break;
	case main2:
		UpdateMenu2();
		break;
	case main3:
		UpdateMenu3();
		break;
	case control:
		UpdateControlMenu();
		break;
	case players:
		UpdatePlayersMenu();
	default:
		break;
	}
}

void MainMenuScene::OnGUI()
{
}

void MainMenuScene::OnSceneActivated()
{
	AddPostProcessingEffect(m_pBackdrop);
	m_pBackdrop->SetBackdropTexture(L"UI/MainMenu.png");
	m_State = begin;
	m_Timer = m_Interval;
}

void MainMenuScene::OnSceneDeactivated()
{
	RemovePostProcessingEffect(m_pBackdrop);
	m_pPlayerOne->Deactivate(true);
	m_pPlayerTwo->Deactivate(true);
	m_pPlayerThree->Deactivate(true);
	m_pPlayerFour->Deactivate(true);
}

void MainMenuScene::UpdateBegin()
{
	m_Timer -= m_SceneContext.pGameTime->GetElapsed();

	if (m_Timer < 0.f)
	{
		m_Timer = m_Interval;
		m_Text.colour.w = m_Text.colour.w < 0.6f ? 1.f : 0.5f;
	}

	TextRenderer::Get()->DrawText(m_pFontBig, m_Text.text, { m_Text.position.x, m_Text.position.z }, m_Text.colour);

	if (m_SceneContext.pInput->IsActionTriggered(enter))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_1.png");
		m_State = main1;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
}

void MainMenuScene::UpdateMenu1()
{
	if (m_SceneContext.pInput->IsActionTriggered(up))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_3.png");
		m_State = main3;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(down))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_2.png");
		m_State = main2;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(back))
	{
		//Go to previous scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu.png");
		m_State = begin;
		m_Timer = m_Interval;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(next))
	{
		//Go to player sprite
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_Player_0.png");
		m_State = players;
		m_Timer = m_IntervalShort;
		m_NrOfPlayers = 0;
		m_pPlayerOne->Activate(true);
		m_pPlayerOne->SetState(ObjectState::Inactive);
		m_pPlayerTwo->Activate(true);
		m_pPlayerTwo->SetState(ObjectState::Inactive);
		m_pPlayerThree->Activate(true);
		m_pPlayerThree->SetState(ObjectState::Inactive);
		m_pPlayerFour->Activate(true);
		m_pPlayerFour->SetState(ObjectState::Inactive);
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
}

void MainMenuScene::UpdateMenu2()
{
	if(m_SceneContext.pInput->IsActionTriggered(up))
	{
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_1.png");
		m_State = main1;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(down))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_3.png");
		m_State = main3;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(back))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu.png");
		m_State = begin;
		m_Timer = m_Interval;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(next))
	{
		//Go to player sprite
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_ControlScheme.png");
		m_State = control;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
}

void MainMenuScene::UpdateMenu3()
{
	if (m_SceneContext.pInput->IsActionTriggered(up))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_2.png");
		m_State = main2;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(down))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_1.png");
		m_State = main1;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(back))
	{
		//Go to previous scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu.png");
		m_State = begin;
		m_Timer = m_Interval;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(next))
	{
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
		PostQuitMessage(1);
	}
}

void MainMenuScene::UpdateControlMenu()
{
	if (m_SceneContext.pInput->IsActionTriggered(back))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_2.png");
		m_State = main2;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
}

void MainMenuScene::UpdatePlayersMenu()
{
	m_Timer -= m_SceneContext.pGameTime->GetElapsed();
	switch(m_NrOfPlayers)
	{
	case 0:
		if(m_Timer < 0.f)
		{
			m_Timer = m_IntervalShort;
			m_AddPlayerOne.colour.w = m_AddPlayerOne.colour.w < 0.6f ? 1.f : 0.5f;
		}
		if (m_SceneContext.pInput->IsActionTriggered(addPlayerOne))
		{
			bool connected = m_SceneContext.pInput->IsGamepadConnected(static_cast<GamepadIndex>(m_NrOfPlayers)); (connected);

#ifdef _DEBUG
			if (connected)
				Logger::LogDebug(L"gamepad connected for player {}", m_NrOfPlayers);
#endif

			//Go to next scene
			m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_Player_1.png");
			++m_NrOfPlayers;
			m_AddPlayerOne.colour.w = 1.f;
			m_pPlayerOne->SetAnimation(BomberMan::AnimationID::Dancing);
			m_Timer = m_IntervalShort;
		}
		break;
	case 1:
		if (m_Timer < 0.f)
		{
			m_Timer = m_IntervalShort;
			m_AddPlayerTwo.colour.w = m_AddPlayerTwo.colour.w < 0.6f ? 1.f : 0.5f;
		}
		if (m_SceneContext.pInput->IsActionTriggered(addPlayerTwo))
		{
			bool connected = m_SceneContext.pInput->IsGamepadConnected(static_cast<GamepadIndex>(m_NrOfPlayers)); (connected);

#ifdef _DEBUG
			if (connected)
				Logger::LogDebug(L"gamepad connected for player {}", m_NrOfPlayers);
#endif

			//Go to next scene
			m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_Player_2.png");
			++m_NrOfPlayers;
			m_AddPlayerTwo.colour.w = 1.f;
			m_pPlayerTwo->SetAnimation(BomberMan::AnimationID::Dancing);
			m_Timer = m_IntervalShort;
		}
		break;
	case 2:
		if (m_Timer < 0.f)
		{
			m_Timer = m_IntervalShort;
			m_AddPlayerThree.colour.w = m_AddPlayerThree.colour.w < 0.6f ? 1.f : 0.5f;
		}
		if (m_SceneContext.pInput->IsActionTriggered(addPlayerThree))
		{
			bool connected = m_SceneContext.pInput->IsGamepadConnected(static_cast<GamepadIndex>(m_NrOfPlayers)); (connected);

#ifdef _DEBUG
			if (connected)
				Logger::LogDebug(L"gamepad connected for player {}", m_NrOfPlayers);
#endif

			//Go to next scene
			m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_Player_3.png");
			++m_NrOfPlayers;
			m_AddPlayerThree.colour.w = 1.f;
			m_pPlayerThree->SetAnimation(BomberMan::AnimationID::Dancing);
			m_Timer = m_IntervalShort;
		}
		break;
	case 3:
		if (m_Timer < 0.f)
		{
			m_Timer = m_IntervalShort;
			m_AddPlayerFour.colour.w = m_AddPlayerFour.colour.w < 0.6f ? 1.f : 0.5f;
		}
		if (m_SceneContext.pInput->IsActionTriggered(addPlayerFour))
		{
			bool connected = m_SceneContext.pInput->IsGamepadConnected(static_cast<GamepadIndex>(m_NrOfPlayers)); (connected);

#ifdef _DEBUG
			if (connected)
				Logger::LogDebug(L"gamepad connected for player {}", m_NrOfPlayers);
#endif

			//Go to next scene
			m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_Player_4.png");
			++m_NrOfPlayers;
			m_AddPlayerFour.colour.w = 1.f;
			m_pPlayerFour->SetAnimation(BomberMan::AnimationID::Dancing);
			m_Timer = m_IntervalShort;
		}
		break;
	default:
		break;
	}

	TextRenderer::Get()->DrawText(m_pFontNormal, m_AddPlayerOne.text, { m_AddPlayerOne.position.x, m_AddPlayerOne.position.z }, m_AddPlayerOne.colour);
	TextRenderer::Get()->DrawText(m_pFontNormal, m_AddPlayerTwo.text, { m_AddPlayerTwo.position.x, m_AddPlayerTwo.position.z }, m_AddPlayerTwo.colour);
	TextRenderer::Get()->DrawText(m_pFontNormal, m_AddPlayerThree.text, { m_AddPlayerThree.position.x, m_AddPlayerThree.position.z }, m_AddPlayerThree.colour);
	TextRenderer::Get()->DrawText(m_pFontNormal, m_AddPlayerFour.text, { m_AddPlayerFour.position.x, m_AddPlayerFour.position.z }, m_AddPlayerFour.colour);


	if (m_SceneContext.pInput->IsActionTriggered(next))
	{
		if (m_NrOfPlayers == 0)
			return;

		m_State = main1;
		m_pPlayerOne->SetAnimation(BomberMan::Idle);
		m_pPlayerTwo->SetAnimation(BomberMan::Idle);
		m_pPlayerThree->SetAnimation(BomberMan::Idle);
		m_pPlayerFour->SetAnimation(BomberMan::Idle);
		m_pPlayerOne->Deactivate(true);
		m_pPlayerTwo->Deactivate(true);
		m_pPlayerThree->Deactivate(true);
		m_pPlayerFour->Deactivate(true);
		m_AddPlayerOne.colour.w = 0.5f;
		m_AddPlayerTwo.colour.w = 0.5f;
		m_AddPlayerThree.colour.w = 0.5f;
		m_AddPlayerFour.colour.w = 0.5f;

		SceneManager::Get()->SetActiveGameScene(L"BombermanScene");
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(back))
	{
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_1.png");
		m_State = main1;
		m_NrOfPlayers = 0;
		m_pPlayerOne->SetAnimation(BomberMan::Idle);
		m_pPlayerTwo->SetAnimation(BomberMan::Idle);
		m_pPlayerThree->SetAnimation(BomberMan::Idle);
		m_pPlayerFour->SetAnimation(BomberMan::Idle);
		m_pPlayerOne->Deactivate(true);
		m_pPlayerTwo->Deactivate(true);
		m_pPlayerThree->Deactivate(true);
		m_pPlayerFour->Deactivate(true);
		m_AddPlayerOne.colour.w = 0.5f;
		m_AddPlayerTwo.colour.w = 0.5f;
		m_AddPlayerThree.colour.w = 0.5f;
		m_AddPlayerFour.colour.w = 0.5f;

		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);

	}
}
