#pragma once
#include "Arena.h"
#include "Utils.h"

#include "Prefabs/Bomb.h"
#include "Prefabs/StageTimer.h"

class Backdrop;
class Spikes;
class UI;
class Wall;
class BomberMan;
class Tile;

class BombermanScene final : public GameScene
{
public:
	BombermanScene();
	~BombermanScene() override;

	BombermanScene(const BombermanScene&) = delete;
	BombermanScene(BombermanScene&&) noexcept = delete;

	BombermanScene& operator=(const BombermanScene&) = delete;
	BombermanScene& operator=(BombermanScene&&) noexcept = delete;

	bool NotifyArena(Events event, GameObject* const pObject, const XMINT2& gridPos);

	uint32_t GetPlayersActive() const;
	static uint32_t GetRounds() { return m_Rounds; }
	static uint32_t GetPlayerWonID() { return m_PlayerWonID; }

	void ResetScene();

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
	void PostDraw() override;

	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	std::vector<BomberMan*> m_pCharacters;
	UI* m_pHUD;
	Arena* m_pArena;
	Backdrop* m_pBackdrop;

	std::vector<XMINT2> m_SpawnPositions;
	int m_ID;

	StageTimer* m_pTimer;
	StageState m_State;

	uint32_t m_NrOfActivePlayers;
	uint32_t m_MaxNumberOfPlayers;
	static uint32_t m_Rounds;
	static uint32_t m_PlayerWonID;

	bool startup = true;
};

