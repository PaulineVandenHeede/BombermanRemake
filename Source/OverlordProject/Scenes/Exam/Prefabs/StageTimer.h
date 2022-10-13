#pragma once

class StageTimer final : public GameObject
{
public:
	StageTimer(float stageTime) : m_StageTime(stageTime), m_RemainingTime(stageTime), m_HurryThreshold(30.f) {}
	virtual ~StageTimer() = default;

	StageTimer(const StageTimer&) = delete;
	StageTimer(StageTimer&&) noexcept = delete;

	StageTimer& operator=(const StageTimer&) = delete;
	StageTimer& operator=(StageTimer&&) noexcept = delete;

	//GETTERS
	std::wstring GetAsText() const;
	float GetRemainingTime() const { return m_RemainingTime; }

	void Reset();

protected:
	virtual void Update(const SceneContext& sceneContext) override;

private:
	float m_StageTime;
	float m_RemainingTime;

	float m_HurryThreshold;
};
