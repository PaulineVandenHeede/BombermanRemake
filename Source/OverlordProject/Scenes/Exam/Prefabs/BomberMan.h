#pragma once
#include "GridBasedObject.h"
#include "Prefabs/Character.h"

class DiffuseMaterial_Skinned;

class BomberMan final : public GridBasedObject
{
public:
	enum AnimationID : int
	{
		Walking = 0,
		Dying = 1,
		Lifting = 2,
		Idle = 3,
		Dancing = 4
	};

	BomberMan(const CharacterDesc& desc, int32_t col, int32_t row, uint32_t playerID);
	~BomberMan() override = default;

	BomberMan(const BomberMan&) = delete;
	BomberMan(BomberMan&&) noexcept = delete;

	BomberMan& operator=(const BomberMan&) = delete;
	BomberMan& operator=(BomberMan&&) noexcept = delete;

	void BombExploded();
	void GameOver();
	void Reset();
	void Won();

	//GETTERS
	uint32_t GetRoundsWon() const { return m_RoundsWon; }
	GamepadIndex GetPlayerID() const { return m_PlayerID; }

	void SetAnimation(AnimationID id);
	void SetTexture(uint32_t id);

protected:
	void Initialize(const SceneContext&) override;

	void OnSceneAttach(GameScene*) override;

	void UpdateActive(const SceneContext&) override;
	void UpdateDestroyAnim(const SceneContext&) override;

private:
	enum Input : int
	{
		MoveForward,
		MoveBackward,
		MoveLeft,
		MoveRight,
		MoveForwardP2,
		MoveBackwardP2,
		MoveLeftP2,
		MoveRightP2,
		DropBomb,
		DropBombP2,
		Menu
	};


	ModelAnimator* m_pAnimator;
	ControllerComponent* m_pController;
	GameObject* m_pModel;
	DiffuseMaterial_Skinned* m_pMaterial;
	
	CharacterDesc m_CharacterDesc;

	float m_DyingAnimationTime;
	
	float m_MoveAcceleration, m_FallAcceleration, m_MoveSpeed;

	GamepadIndex m_PlayerID;
	int m_BombCapacity;
	int m_CurrentCapacity;

	uint32_t m_RoundsWon;

	//SOUNDS
	FMOD::ChannelGroup* m_pSoundEffectGroup{ nullptr };
	FMOD::Sound* m_pSoundEffectDying{ nullptr };
	FMOD::Sound* m_pSoundEffectDrop{ nullptr };



	void HandleMovement(const SceneContext& sceneContext);
	void HandleBombDropping(const SceneContext& sceneContext);
	void HandleGridPosChange(const SceneContext& sceneContext);
};

