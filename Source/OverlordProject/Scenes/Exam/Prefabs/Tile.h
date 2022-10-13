#pragma once
#include "GridBasedObject.h"

class Tile final : public GridBasedObject
{
public:
	Tile(int32_t c, int32_t r,int light);
	~Tile() override = default;

	Tile(const Tile&) = delete;
	Tile(Tile&&) noexcept = delete;

	Tile& operator=(const Tile&) = delete;
	Tile& operator=(Tile&&) noexcept = delete;

	//SETTERS
	static void SetSize(uint16_t size) { m_Size = size; }
	void SetOccupied(GameObject* pSlot) {m_pSlot = pSlot; }

	//GETTERs
	static uint16_t GetSize() { return m_Size; }
	bool GetOccupied() const;
	GameObject* const GetSlot() const { return m_pSlot; }


protected:
	void Initialize(const SceneContext&) override;

private:
	static uint16_t m_Size;
	GameObject* m_pSlot;
};

