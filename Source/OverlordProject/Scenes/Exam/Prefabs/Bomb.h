#pragma once
#include "GridBasedObject.h"

class GridComponent;
class BomberMan;

class Bomb final : public GridBasedObject
{
public:
	Bomb(XMINT2 gridPos, float y);
	~Bomb() override = default;

	Bomb(const Bomb&) = delete;
	Bomb(Bomb&&) noexcept = delete;

	Bomb& operator=(const Bomb&) = delete;
	Bomb& operator=(Bomb&&) noexcept = delete;

	//SETTERS
	static void SetRadius(float radius) { m_Radius = radius; }
	void SetPlayerReference(BomberMan* pPlayer);
	void SetNext(Bomb* pNext) { m_pUserData.pNext = pNext; }

	//GETTERS
	static float GetRadius() { return m_Radius; }
	Bomb* GetNext() const { return m_pUserData.pNext; }
	uint32_t GetExplosionRange() const { return m_ExplosionRange; }

	void Activate(bool children) override;
	void Deactivate(bool children) override;

protected:
	void Initialize(const SceneContext&) override;

	void UpdateActive(const SceneContext&) override;


private:
	union
	{
		BomberMan* pPlayer;
		Bomb* pNext;
	} m_pUserData;

	uint32_t m_ExplosionRange;
	float m_RemainingLifeTime;
	float m_MaxLifeTime;

	static float m_Radius;
};

