#include "stdafx.h"
#include "StageTimer.h"

#include "Scenes/Exam/BombermanScene.h"

std::wstring StageTimer::GetAsText() const
{
	int min = static_cast<int>(m_RemainingTime) / 60;
	int sec = static_cast<int>(m_RemainingTime) - (min * 60);
	if (sec < 10)
	{
		return std::to_wstring(min) + L" : 0" + std::to_wstring(sec);
	}
	return std::to_wstring(min) + L" : " + std::to_wstring(sec);
}

void StageTimer::Reset()
{
	m_HurryThreshold = 30.f;
	m_RemainingTime = m_StageTime;
}

void StageTimer::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	m_RemainingTime -= deltaTime;

	if (m_HurryThreshold > m_RemainingTime)
	{
		BombermanScene* const pScene = reinterpret_cast<BombermanScene*>(GetScene());
		pScene->NotifyArena(Events::Hurry, this, { 0,0 });
		m_HurryThreshold = -1.f;
	}
}
