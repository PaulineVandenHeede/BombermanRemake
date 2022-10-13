#pragma once
#include "Pool.h"
#include "Prefabs/Bomb.h"

class Tile;
class Wall;
class Spikes;

enum class Events
{
	BombSpawned,
	BombExploded,
	PlayerMoved,
	LastOneStanding,
	Hurry,
	Squished
};


class Arena
{
public:
	Arena(const std::vector<XMINT2>& playerPositions, GameScene* const pScene, const XMFLOAT2& spawnPos, uint32_t nrOfCols, uint32_t nrOfRows, uint32_t nrOfPlayers);
	~Arena();

	Arena(const Arena&) = delete;
	Arena(Arena&&) noexcept = delete;

	Arena& operator=(const Arena&) = delete;
	Arena& operator=(Arena&&) noexcept = delete;

	void Update(GameScene* const pScene);
	bool Notify(Events event, GameObject* const pObject, const XMINT2& gridPos);
	void OnSceneReset(const std::vector<XMINT2>& playerPositions, GameScene* const pScene, uint32_t nrOfPlayers);

	//GETTERS
	bool TileAvailable(const XMINT2& gridPos) const;
	Tile* const GetTile(const XMINT2& gridPos) const;
	Spikes* const GetSpikes(const XMINT2& gridPos) const;

	//SETTERS


private:
	Pool<Bomb>* m_pPool;
	std::vector<Tile*> m_pTiles;
	std::vector<Wall*> m_pHardWalls;
	std::vector<Wall*> m_pSoftWalls;
	std::vector<Spikes*> m_pSpikes;

	FMOD::ChannelGroup* m_pSoundEffectGroup{ nullptr };
	FMOD::Sound* m_pSoundEffectExplosion{ nullptr };
	FMOD::Sound* m_pSoundEffectCrateDestroyed{ nullptr };
	FMOD::Sound* m_pSoundEffectSquish{ nullptr };


	float m_SpikesTimer;
	float m_SpikesInterval;

	uint32_t m_Iterations;

	uint32_t m_CurrentNumber;
	uint32_t m_TotalNumberHorizontal;
	uint32_t m_TotalNumberVertical;

	XMINT2 m_GridPos;

	bool m_UpLeft = true;
	bool m_Horizontal = true;
	bool m_Spikes = false;



	//Private Functions
	void SpawnArena(GameScene* const pGameScene);
	void SpawnObstacles(const std::vector<XMINT2>& playerPositions, GameScene* const pGameScene, uint32_t nrOfPlayers);
	void SpawnSoftWalls(const std::vector<XMINT2>& playerPositions, GameScene* const pGameScene, uint32_t nrOfPlayers);

	void UpdateSpikes(float deltaTime);
	void SpawnSpike();

	bool SpawnBomb(GameObject* const pObject, const XMINT2& gridPos);
	bool BombExploded(GameObject* const pObject, const XMINT2& gridPos);
	bool PlayerMoved(GameObject* const pObject, const XMINT2& gridPos);
	bool LastOneStanding(GameObject* const pObject, const XMINT2& gridPos);
	bool StartSpikes(GameObject* const pObject, const XMINT2& gridPos);
	bool Squished(GameObject* const pObject, const XMINT2& gridPos);

	void ClearTile(int32_t explosionRange, int32_t direction, XMINT2 gridPos);
};

