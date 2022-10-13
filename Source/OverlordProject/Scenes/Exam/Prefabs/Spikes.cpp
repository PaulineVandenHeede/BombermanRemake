#include "stdafx.h"
#include "Spikes.h"

#include "../Materials/ToonMaterial.h"
#include "Scenes/Exam/BombermanScene.h"
#include "Scenes/Exam/Components/GridComponent.h"

Spikes::Spikes(XMINT2 gridPos, float y)
	: GridBasedObject{ gridPos, y}
{
}

void Spikes::Initialize(const SceneContext&)
{
	ModelComponent* pModel = AddComponent(new ModelComponent{ L"Meshes/Spikes.ovm" });
	ToonMaterial* const pToonMaterial = MaterialManager::Get()->CreateMaterial<ToonMaterial>();
	pToonMaterial->SetColor(static_cast<XMFLOAT4>(Colors::DarkSlateGray));
	pModel->SetMaterial(pToonMaterial);
	//collision
	//physx::PxMaterial* const pxSphereMaterial = PxGetPhysics().createMaterial(0.2f, 0.2f, 0.8f);
	//RigidBodyComponent* const pRigidBodyComponent = /*m_pModel->*/AddComponent<RigidBodyComponent>(new RigidBodyComponent{ false });
	//physx::PxConvexMesh* const pxConvexMeshChair = ContentManager::Load<physx::PxConvexMesh>(L"Meshes/Spikes.ovpc");
	//pRigidBodyComponent->AddCollider(PxConvexMeshGeometry{ pxConvexMeshChair }, *pxSphereMaterial);
	/*pRigidBodyComponent->SetConstraint(RigidBodyConstraint::AllTrans, false);
	pRigidBodyComponent->SetConstraint(RigidBodyConstraint::AllRot, false);*/

	//GridBasedObject::Deactivate(true);
}

void Spikes::UpdateActive(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	XMFLOAT3 pos = GetTransform()->GetPosition();
	pos.y -= 50 * deltaTime;
	MathHelper::Clamp(pos.y, FLT_MAX, 0.f);

	GetTransform()->Translate(pos);

	if(pos.y < m_SquishThreshold)
	{
		BombermanScene* const pScene = reinterpret_cast<BombermanScene*>(GetScene());
		pScene->NotifyArena(Events::Squished, this, GetGridComponent()->GetGridPos());
		m_SquishThreshold = -1.f;
	}
}

void Spikes::Activate(bool children)
{
	GridBasedObject::Activate(children);
	m_SquishThreshold = 10.f;

	/*RigidBodyComponent* const pRigidBodyComponent = GetComponent<RigidBodyComponent>();
	pRigidBodyComponent->SetConstraint(RigidBodyConstraint::AllTrans, true);*/
}
