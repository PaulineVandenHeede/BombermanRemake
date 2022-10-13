#pragma once
#include "GridBasedObject.h"

class Spikes : public GridBasedObject
{
public:
	Spikes(XMINT2 gridPos, float y);
	virtual ~Spikes() override = default;

	Spikes(const Spikes&) = delete;
	Spikes(Spikes&&) noexcept = delete;
	Spikes& operator=(const Spikes&) = delete;
	Spikes& operator = (Spikes&&) noexcept = delete;

	void Activate(bool children) override;

protected:
	void Initialize(const SceneContext&) override;

	void UpdateActive(const SceneContext&) override;

private:
	float m_SquishThreshold = 10.f;
};

