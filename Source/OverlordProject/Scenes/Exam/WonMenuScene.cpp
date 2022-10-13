#include "stdafx.h"
#include "WonMenuScene.h"

#include "BombermanScene.h"
#include "Materials/Post/Backdrop.h"
#include "Prefabs/BomberMan.h"


WonMenuScene::WonMenuScene()
	: GameScene{ L"WonMenuScene"}
{
}

void WonMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawPhysXDebug = false;

	m_Interval = 15.f;
	m_Timer = m_Interval;

	//CAMERA
	FixedCamera* const pCamera = new FixedCamera();
	pCamera->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, -30.f);
	AddChild(pCamera);
	SetActiveCamera(pCamera->GetComponent<CameraComponent>()); //Also sets pCamera in SceneContext

	m_pFontBig = ContentManager::Load<SpriteFont>(L"SpriteFonts/SuperSonicRocketShip_60.fnt");
	m_Text.position = { m_SceneContext.windowWidth / 2.f - 150.f, 0.f, 150.f };
	m_Text.colour = static_cast<XMFLOAT4>(Colors::White);
	m_Text.text = L"Player " + std::to_wstring(BombermanScene::GetPlayerWonID()) + L" won!";
	m_Text.idFont = 0;

	//character
	const auto pxDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	CharacterDesc desc{ pxDefaultMaterial, 8.f, 19.f };
	desc.maxMoveSpeed = 0.f;
	desc.maxFallSpeed = 0.f;
	m_pPlayer = AddChild(new BomberMan{ desc, -5, 0, 0 });
	m_pPlayer->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f + 2.f , m_SceneContext.windowHeight / 2.f - 1.f, -10.f);
	m_pPlayer->GetTransform()->Rotate(0.f, 180.f, 0.f);
	m_pPlayer->GetTransform()->Scale(0.25f);
	m_pPlayer->SetState(ObjectState::Inactive);
	m_pPlayer->Deactivate(true);


	//post
	m_pBackdrop = MaterialManager::Get()->CreateMaterial<Backdrop>();
	m_pBackdrop->SetBackdropTexture(L"UI/WonMenu.png");
	m_pBackdrop->SetClearColour(m_SceneContext.settings.clearColor);
	//AddPostProcessingEffect(m_pBackdrop);
}

void WonMenuScene::Update()
{
	m_Timer -= m_SceneContext.pGameTime->GetElapsed();

	if(m_Timer < 0.f)
	{
		SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
	}

	TextRenderer::Get()->DrawText(m_pFontBig, m_Text.text, { m_Text.position.x, m_Text.position.z }, m_Text.colour);
}

void WonMenuScene::OnGUI()
{
}

void WonMenuScene::OnSceneActivated()
{
	AddPostProcessingEffect(m_pBackdrop);
	m_pBackdrop->SetBackdropTexture(L"UI/WonMenu.png");
	m_pPlayer->Activate(true);
	m_pPlayer->SetState(ObjectState::Inactive);
	m_pPlayer->SetAnimation(BomberMan::AnimationID::Dancing);
	GamepadIndex id = m_pPlayer->GetPlayerID();
	m_pPlayer->SetTexture(static_cast<uint32_t>(id));

	m_Text.text = L"Player " + std::to_wstring(BombermanScene::GetPlayerWonID() + 1) + L" won!";

	m_Timer = m_Interval;
}

void WonMenuScene::OnSceneDeactivated()
{
	RemovePostProcessingEffect(m_pBackdrop);
}