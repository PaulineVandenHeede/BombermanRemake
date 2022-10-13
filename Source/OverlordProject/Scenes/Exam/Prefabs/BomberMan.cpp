#include "stdafx.h"
#include "BomberMan.h"

#include "Prefabs/Character.h"
#include "Bomb.h"
#include "DiffuseMaterial_Skinned.h"
#include "../Materials/ToonTexMaterial_Skinned.h"

#include "../Components/GridComponent.h"

#include "Scenes/Exam/BombermanScene.h"

BomberMan::BomberMan(const CharacterDesc& desc, int32_t col, int32_t row, uint32_t playerID)
	: GridBasedObject{ {col, row}, 17.75f }
	, m_pAnimator( nullptr )
	, m_pController( nullptr )
	, m_CharacterDesc( desc )
	, m_MoveAcceleration( 0.f )
	, m_FallAcceleration( 0.f )
	, m_MoveSpeed( 0.f )
	, m_BombCapacity( 2 )
	, m_CurrentCapacity( 2 )
	, m_RoundsWon( 0 )
{
	/*CharacterDesc desc{ pMaterial, 8.f, 19.f };*/
	m_CharacterDesc.actionId_MoveForward = Input::MoveForward;
	m_CharacterDesc.actionId_MoveBackward = Input::MoveBackward;
	m_CharacterDesc.actionId_MoveLeft = Input::MoveLeft;
	m_CharacterDesc.actionId_MoveRight = Input::MoveRight;

	m_CharacterDesc.controller.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	m_CharacterDesc.controller.stepOffset = 0.f;
	m_CharacterDesc.controller.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;

	m_MoveAcceleration = desc.maxMoveSpeed / desc.moveAccelerationTime;
	m_FallAcceleration = desc.maxFallSpeed / desc.fallAccelerationTime;

	switch(playerID)
	{
	case 0:
		m_PlayerID = GamepadIndex::playerOne;
		break;
	case 1:
		m_PlayerID = GamepadIndex::playerTwo;
		break;
	case 2:
		m_PlayerID = GamepadIndex::playerThree;
		break;
	case 3:
		m_PlayerID = GamepadIndex::playerFour;
		break;
	default:
		break;
	}

}

void BomberMan::BombExploded()
{
	++m_CurrentCapacity;
	if (m_CurrentCapacity > m_BombCapacity)
		m_CurrentCapacity = m_BombCapacity;
}

void BomberMan::GameOver()
{
	Logger::LogTodo(L"Implement Game Over sequence player");
	m_pAnimator->SetAnimation(AnimationID::Dying);
	m_pAnimator->SetAnimationSpeed(2.f);
	AnimationClip clip = m_pAnimator->GetClip(AnimationID::Dying);
	m_DyingAnimationTime = (clip.duration / clip.ticksPerSecond) / m_pAnimator->GetAnimationSpeed();
	SetState(ObjectState::DestroyAnim);

	//Sound
	SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectDying, m_pSoundEffectGroup, false, nullptr);
}

void BomberMan::Reset()
{
	m_RoundsWon = 0;
}

void BomberMan::Won()
{
	++m_RoundsWon;
	m_pAnimator->SetAnimation(AnimationID::Dancing);
	m_pAnimator->SetAnimationSpeed(1.f);
}

void BomberMan::SetAnimation(AnimationID id)
{
	m_pAnimator->SetAnimation(id);
	m_pAnimator->SetAnimationSpeed(1.f);
}

void BomberMan::SetTexture(uint32_t id)
{
	m_pMaterial->SetDiffuseTexture(L"Textures/Player/Player_" + std::to_wstring(id) + L".png");
}

void BomberMan::Initialize(const SceneContext& sceneContext)
{
	//Controller
	m_pController = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	//XMFLOAT3 foward = m_pController->GetTransform()->GetForward();
	//Logger::LogInfo(L"forward Vector of controller: {}, {}, {}", foward.x, foward.y, foward.z);

#pragma region Model
	m_pModel = AddChild(new GameObject{});
	ModelComponent* pStickyModel = new ModelComponent{ L"Meshes/Sticky_Animated2.ovm", true };
	m_pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	m_pMaterial->SetDiffuseTexture(L"Textures/Player/Player_" + std::to_wstring(static_cast<uint32_t>(m_PlayerID)) + L".png");
	pStickyModel->SetMaterial(m_pMaterial);
	m_pModel->AddComponent(pStickyModel);
	m_pModel->GetTransform()->Rotate(0.f, 180.f, 0.f);
	m_pModel->GetTransform()->Translate(0.f, -16.f, 0.f);
	m_pAnimator = pStickyModel->GetAnimator();
	m_pAnimator->SetAnimation(AnimationID::Idle);
	m_pAnimator->SetAnimationSpeed(1.f);
	m_pAnimator->Play();
#pragma endregion Model

#pragma region Input

	if(m_PlayerID == GamepadIndex::playerOne)
	{
		auto inputAction = InputAction{ MoveLeft, InputState::down, 'A' };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ MoveRight, InputState::down, 'D' };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ MoveForward, InputState::down, 'W' };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ MoveBackward, InputState::down, 'S' };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ DropBomb, InputState::pressed, VK_SPACE, -1,  XINPUT_GAMEPAD_A, m_PlayerID };
		sceneContext.pInput->AddInputAction(inputAction);
	}
	else if(m_PlayerID == GamepadIndex::playerTwo)
	{
		auto inputAction = InputAction{ MoveLeftP2, InputState::down, VK_LEFT };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ MoveRightP2, InputState::down, VK_RIGHT };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ MoveForwardP2, InputState::down, VK_UP };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ MoveBackwardP2, InputState::down, VK_DOWN };
		sceneContext.pInput->AddInputAction(inputAction);

		inputAction = InputAction{ DropBombP2, InputState::pressed, VK_RETURN, -1,  XINPUT_GAMEPAD_A, m_PlayerID };
		sceneContext.pInput->AddInputAction(inputAction);
	}

	auto inputAction = InputAction{ Menu, InputState::pressed, VK_ESCAPE, -1,  XINPUT_GAMEPAD_BACK, m_PlayerID };
	sceneContext.pInput->AddInputAction(inputAction);
#pragma endregion Input

#pragma region Sounds
	FMOD::System* const pSoundSystem = SoundManager::Get()->GetSystem();
	FMOD_RESULT result;
	//Dying
	result = pSoundSystem->createStream("Resources/Sounds/hurt grunt.wav", FMOD_DEFAULT, nullptr, &m_pSoundEffectDying);
	HANDLE_ERROR(result);
	//dropping
	result = pSoundSystem->createStream("Resources/Sounds/DropItem.wav", FMOD_DEFAULT, nullptr, &m_pSoundEffectDrop);
	HANDLE_ERROR(result);

	result = pSoundSystem->createChannelGroup("SoundEffects", &m_pSoundEffectGroup);
	HANDLE_ERROR(result);

	m_pSoundEffectGroup->setVolume(0.5f);
#pragma endregion

}

void BomberMan::OnSceneAttach(GameScene*)
{
	//Reset state
	SetState(ObjectState::Active);
	//Reset animation
	m_pAnimator->SetAnimation(AnimationID::Idle);
	m_pAnimator->SetAnimationSpeed(1.f);
	//Reset grid pos > happens automatically
	//Reset position
	//GetTransform()->Translate(Tile::GetSize() / 2.f, 17.75f, Tile::GetSize() / 2.f);
	//Reset capacity
	m_CurrentCapacity = m_BombCapacity;

}

void BomberMan::UpdateActive(const SceneContext& sceneContext)
{
	HandleMovement(sceneContext);
	HandleBombDropping(sceneContext);
	HandleGridPosChange(sceneContext);

	if(sceneContext.pInput->IsActionTriggered(Menu))
	{
		SceneManager::Get()->SetActiveGameScene(L"PauseMenuScene");
	}
}

void BomberMan::UpdateDestroyAnim(const SceneContext& sceneContext)
{
	m_DyingAnimationTime -= sceneContext.pGameTime->GetElapsed();
	if (m_DyingAnimationTime < 0.1f)
	{
		SetState(ObjectState::Destroy);
	}
}

void BomberMan::HandleMovement(const SceneContext& sceneContext)
{
	const float deltaTime = sceneContext.pGameTime->GetElapsed();

	InputManager* const pInput = sceneContext.pInput;
	
	//Gather input
	XMFLOAT2 move{ g_XMZero };
	if(pInput->IsGamepadConnected(m_PlayerID))
	{
		move = pInput->GetThumbstickPosition(true, m_PlayerID);
	}
	else
	{
		if(m_PlayerID == GamepadIndex::playerOne)
		{
			move.x = pInput->IsActionTriggered(MoveRight) ? 1.f : (pInput->IsActionTriggered(MoveLeft) ? -1.f : 0.f);
			move.y = pInput->IsActionTriggered(MoveForward) ? 1.f : (pInput->IsActionTriggered(MoveBackward) ? -1.f : 0.f);
		}
		if(m_PlayerID == GamepadIndex::playerTwo)
		{
			move.x = pInput->IsActionTriggered(MoveRightP2) ? 1.f : (pInput->IsActionTriggered(MoveLeftP2) ? -1.f : 0.f);
			move.y = pInput->IsActionTriggered(MoveForwardP2) ? 1.f : (pInput->IsActionTriggered(MoveBackwardP2) ? -1.f : 0.f);
		}
	}
	//Logger::LogDebug(L"Move: {}, {}", move.x, move.y);

	//not faster diagonal
	XMVECTOR movement = XMLoadFloat2(&move);
	movement = XMVector2Normalize(movement);
	XMStoreFloat2(&move, movement);

	//Movement
	float currAcceleration = m_MoveAcceleration * deltaTime;
	if (abs(move.x) > FLT_EPSILON || abs(move.y) > FLT_EPSILON)
	{
		//rotation
		float yaw = atan2f(move.x, move.y);
		m_pController->GetTransform()->Rotate(0.f, yaw, 0.f, false);

		//animation
		if (m_pAnimator->GetClipName() != L"Walking")
		{
			m_pAnimator->SetAnimation(AnimationID::Walking);
			m_pAnimator->SetAnimationSpeed(1.5f);
		}

		//movement
		m_MoveSpeed += currAcceleration;
	}
	else
	{
		//animation
		if (m_pAnimator->GetClipName() != L"Idle")
		{
			m_pAnimator->SetAnimation(AnimationID::Idle);
			m_pAnimator->SetAnimationSpeed(1.f);
		}

		//movement
		m_MoveSpeed -= currAcceleration;
	}
	MathHelper::Clamp(m_MoveSpeed, m_CharacterDesc.maxMoveSpeed, 0.f);

	movement = m_MoveSpeed * deltaTime * movement;
	XMStoreFloat2(&move, movement);

	//gravity
	float y{ 0.f };
	if (m_pController->GetCollisionFlags() != PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		y = -m_FallAcceleration* deltaTime;
	}

	//movement
	m_pController->Move({move.x, y, move.y});

	//Logger::LogInfo(L"Forward: {}, {}, {}", GetTransform()->GetForward().x, GetTransform()->GetForward().y, GetTransform()->GetForward().z);
}

void BomberMan::HandleBombDropping(const SceneContext& sceneContext)
{
	bool dropped = false;
	if (sceneContext.pInput->IsActionTriggered(DropBomb) && m_PlayerID == GamepadIndex::playerOne)
		dropped = true;
	if (sceneContext.pInput->IsActionTriggered(DropBombP2) && m_PlayerID == GamepadIndex::playerTwo)
		dropped = true;
	if (!dropped)
		return;

	if (m_CurrentCapacity <= 0)
	{
#ifdef _DEBUG
		Logger::LogInfo(L"Current capacity is 0");
#endif
		return;
	}

	//SPAWN BOMB
	BombermanScene* const pScene = reinterpret_cast<BombermanScene*>(GetScene());

	//PLACE BOMB ON PLAYER POS -> GET GRID POS
	XMINT2 gridPos = GetGridComponent()->GetGridPos();
	bool result = pScene->NotifyArena(Events::BombSpawned, this, gridPos);

	if(result)
	{
		--m_CurrentCapacity;
		SoundManager::Get()->GetSystem()->playSound(m_pSoundEffectDrop, m_pSoundEffectGroup, false, nullptr);
	}
}

void BomberMan::HandleGridPosChange(const SceneContext&)
{
	XMINT2 gridPos = GetGridComponent()->GetGridPos();
	XMINT2 prevGridPos = GetGridComponent()->GetPreviousGridPos();
	if ((gridPos.x == prevGridPos.x) && (gridPos.y == prevGridPos.y))
		return;

	BombermanScene* const pScene = reinterpret_cast<BombermanScene*>(GetScene());
	pScene->NotifyArena(Events::PlayerMoved, this, prevGridPos);
}
