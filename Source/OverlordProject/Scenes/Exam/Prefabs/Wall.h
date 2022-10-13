#pragma once
#include "GridBasedObject.h"

enum WallType
{
	Soft = 0,
	Hard = 1,
	Fence = 2
};

class Wall final : public GridBasedObject
{
public:
	Wall(WallType type, int32_t col, int32_t row);
	virtual ~Wall() = default;

	Wall(const Wall&) = delete;
	Wall(Wall&&) noexcept = delete;

	Wall& operator=(const Wall&) = delete;
	Wall& operator=(Wall&&) noexcept = delete;

	//Returns if wall is destroyed in the progress
	// > due to wall types this can be different
	bool Destroy();
	bool Squish();

	bool GetDestroyed() const { return m_Destroyed; }

protected:
	void Initialize(const SceneContext&) override;

private:
	WallType m_Type;
	bool m_Destroyed;
};

