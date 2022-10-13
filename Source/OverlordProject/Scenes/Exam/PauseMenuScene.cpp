#include "stdafx.h"
#include "PauseMenuScene.h"

#include "Materials/Post/Backdrop.h"
#include "Prefabs/BomberMan.h"

PauseMenuScene::PauseMenuScene()
	: GameScene{ L"PauseMenuScene"}
{
}

void PauseMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_State = PauseMenuState::pause1;

	//CAMERA
	FixedCamera* const pCamera = new FixedCamera();
	pCamera->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, -30.f);
	AddChild(pCamera);
	SetActiveCamera(pCamera->GetComponent<CameraComponent>()); //Also sets pCamera in SceneContext

	//input
	m_SceneContext.pInput->AddInputAction(InputAction{ pauseEnter, InputState::pressed, VK_RETURN, -1, XINPUT_GAMEPAD_B });
	m_SceneContext.pInput->AddInputAction(InputAction{ pauseBack, InputState::pressed, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });
	m_SceneContext.pInput->AddInputAction(InputAction{ pauseNext, InputState::pressed, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.pInput->AddInputAction(InputAction{ pauseDown, InputState::pressed, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.pInput->AddInputAction(InputAction{ pauseUp, InputState::pressed, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP });

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

	//post
	m_pBackdrop = MaterialManager::Get()->CreateMaterial<Backdrop>();
	m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_1.png");
	m_pBackdrop->SetClearColour(m_SceneContext.settings.clearColor);
	//AddPostProcessingEffect(m_pBackdrop);
}

void PauseMenuScene::Update()
{
	switch(m_State)
	{
	case pause1:
		UpdateMenu1();
		break;
	case pause2:
		UpdateMenu2();
		break;
	case pause3:
		UpdateMenu3();
		break;
	case pauseControl:
		UpdateControlMenu();
		break;
	default:
		break;
	}
}

void PauseMenuScene::OnGUI()
{
}

void PauseMenuScene::OnSceneActivated()
{
	AddPostProcessingEffect(m_pBackdrop);
	m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_1.png");
	m_State = pause1;
}

void PauseMenuScene::OnSceneDeactivated()
{
	RemovePostProcessingEffect(m_pBackdrop);
}


void PauseMenuScene::UpdateMenu1()
{
	if (m_SceneContext.pInput->IsActionTriggered(pauseUp))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_3.png");
		m_State = pause3;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(pauseDown))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_2.png");
		m_State = pause2;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(pauseNext))
	{
		SceneManager::Get()->SetActiveGameScene(L"BombermanScene");
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
}

void PauseMenuScene::UpdateMenu2()
{
	if(m_SceneContext.pInput->IsActionTriggered(pauseUp))
	{
		m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_1.png");
		m_State = pause1;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(pauseDown))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_3.png");
		m_State = pause3;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(pauseNext))
	{
		//Go to player sprite
		m_pBackdrop->SetBackdropTexture(L"UI/MainMenu_ControlScheme.png");
		m_State = pauseControl;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
}

void PauseMenuScene::UpdateMenu3()
{
	if (m_SceneContext.pInput->IsActionTriggered(pauseUp))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_2.png");
		m_State = pause2;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(pauseDown))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_1.png");
		m_State = pause1;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectMove, m_pSoundEffectGroup, false, nullptr);
	}
	if (m_SceneContext.pInput->IsActionTriggered(pauseNext))
	{
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
		PostQuitMessage(1);
	}
}

void PauseMenuScene::UpdateControlMenu()
{
	if (m_SceneContext.pInput->IsActionTriggered(pauseBack))
	{
		//Go to next scene
		m_pBackdrop->SetBackdropTexture(L"UI/PauseMenu_2.png");
		m_State = pause2;
		//sound
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectNext, m_pSoundEffectGroup, false, nullptr);
	}
}
