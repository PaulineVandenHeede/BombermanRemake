#include "stdafx.h"
#include "GridBasedObject.h"

#include "../Components/GridComponent.h"

#include "Spikes.h"

GridBasedObject::GridBasedObject(XMINT2 gridPos, float y)
	: GameObject{  }
	, m_pGridComponent{ AddComponent(new GridComponent{ gridPos }) }
	, m_State{ ObjectState::Active }
{
	XMFLOAT2 gridSpawnPos = GridComponent::GetSpawnPosition();
	GetTransform()->Translate(gridSpawnPos.x + static_cast<float>(gridPos.x) * GridComponent::GetGridSize(),
		y, 
		gridSpawnPos.y + static_cast<float>(gridPos.y) * GridComponent::GetGridSize());
}

void GridBasedObject::Activate(bool children)
{
	GameObject::Activate(children);
	m_State = ObjectState::Active;
}

void GridBasedObject::Deactivate(bool children)
{
	GameObject::Deactivate(children);
	m_State = ObjectState::Inactive;
}

void GridBasedObject::Initialize(const SceneContext&)
{
}

void GridBasedObject::Update(const SceneContext& sceneContext)
{
	/*if (typeid(*this) == typeid(Spikes))
		Logger::LogDebug(L"Update Grid based object spike!");*/

	switch (m_State)
	{
	case ObjectState::Active:
		UpdateActive(sceneContext);
		break;
	case ObjectState::DestroyAnim:
		UpdateDestroyAnim(sceneContext);
		break;
	case ObjectState::Destroy:
		UpdateDestroy(sceneContext);
		break;
	case ObjectState::Inactive:
	default:
		break;
	}
}

void GridBasedObject::OnSceneAttach(GameScene*)
{
	m_State = ObjectState::Active;
}

void GridBasedObject::OnSceneDetach(GameScene*)
{
	m_State = ObjectState::Inactive;
}
