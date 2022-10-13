#include "stdafx.h"
#include "GridComponent.h"

#include "Scenes/Exam/Prefabs/BomberMan.h"
#include "Scenes/Exam/Prefabs/Tile.h"

XMFLOAT2 GridComponent::m_SpawnPosition{ 0.f, 0.f };
XMINT2 GridComponent::m_GridDimensions{ 17, 11 };
uint32_t GridComponent::m_GridSize{ 20 };

void GridComponent::Update(const SceneContext&)
{
	//Update grid pos according to game object's position
	m_PreviousGridPos = m_GridPos;

	XMFLOAT3 pos = GetGameObject()->GetTransform()->GetPosition();
	m_GridPos.x = int(pos.x) / m_GridSize;
	m_GridPos.y = int(pos.z) / m_GridSize;
}

void GridComponent::OnSceneAttach(GameScene*)
{
	//reset grid pos
	XMFLOAT3 pos = GetGameObject()->GetTransform()->GetPosition();
	m_GridPos.x = int(pos.x) / m_GridSize;
	m_GridPos.y = int(pos.z) / m_GridSize;
}
