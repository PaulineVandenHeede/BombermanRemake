#include "stdafx.h"
#include "Bomb.h"

//#include "Materials/ColorMaterial.h"
#include "../BombermanScene.h"
#include "BomberMan.h"
#include "../Components/GridComponent.h"
#include "../Materials/ToonMaterialTex.h"

float Bomb::m_Radius{ 8.f };

Bomb::Bomb(XMINT2 gridPos, float y)
	: GridBasedObject{ gridPos, y} //game object gets initialized first
	, m_pUserData{ nullptr }
	, m_MaxLifeTime{ 2.f }
	, m_RemainingLifeTime{ 5.f }
	, m_ExplosionRange{ 2 }
{
	m_IsActive = false;
	SetState(ObjectState::Inactive);
}

void Bomb::SetPlayerReference(BomberMan* pPlayer)
{
	if (m_pUserData.pPlayer)
	{
		Logger::LogWarning(L"Bomb already has a player reference");
	}
	m_pUserData.pPlayer = pPlayer;
	GetGridComponent()->SetGridPos(pPlayer->GetComponent<GridComponent>()->GetGridPos());
}

void Bomb::Initialize(const SceneContext&)
{
	//Model
	ModelComponent* const pModelComponent = AddComponent(new ModelComponent{ L"Meshes/Bomb.ovm", true });
	ToonMaterialTex* const pToonMaterial = MaterialManager::Get()->CreateMaterial<ToonMaterialTex>();
	pToonMaterial->SetDiffuseMap(L"Textures/Bomb.png");
	pModelComponent->SetMaterial(pToonMaterial);

	//Collision
	physx::PxMaterial* const pxSphereMaterial = PxGetPhysics().createMaterial(0.2f, 0.2f, 0.8f);
	RigidBodyComponent* const pRigidBodyComponent = AddComponent<RigidBodyComponent>(new RigidBodyComponent{  });
	physx::PxConvexMesh* const pxConvexMeshChair = ContentManager::Load<physx::PxConvexMesh>(L"Meshes/Sphere.ovpc");
	pRigidBodyComponent->AddCollider(PxConvexMeshGeometry{ pxConvexMeshChair, PxMeshScale(m_Radius) }, *pxSphereMaterial);
	pRigidBodyComponent->SetConstraint(RigidBodyConstraint::AllTrans, false);
	pRigidBodyComponent->SetConstraint(RigidBodyConstraint::AllRot, false);
	
	//GetTransform()->Scale(m_Radius);

}

void Bomb::UpdateActive(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();

	m_RemainingLifeTime -= deltaTime;

	if (m_RemainingLifeTime < 0.f)
	{
		BombermanScene* pScene = reinterpret_cast<BombermanScene*>(GetScene());
		pScene->NotifyArena(Events::BombExploded, this, GetGridComponent()->GetGridPos());
	}
}

void Bomb::Activate(bool children)
{
	GridBasedObject::Activate(children);

	RigidBodyComponent* pRigidBody = GetComponent<RigidBodyComponent>();
	if (!pRigidBody)
		return;
	pRigidBody->SetConstraint(RigidBodyConstraint::AllTrans, false);
	pRigidBody->SetConstraint(RigidBodyConstraint::AllRot, false);
	GetScene()->GetPhysxProxy()->GetPhysxScene()->addActor(*pRigidBody->GetPxRigidActor());

	m_RemainingLifeTime = m_MaxLifeTime;
}

void Bomb::Deactivate(bool children)
{
	GridBasedObject::Deactivate(children);

	//Notify player
	if(m_pUserData.pPlayer != nullptr)
	{
		m_pUserData.pPlayer->BombExploded();
		m_pUserData.pPlayer = nullptr;
	}

	//transform
	GetGridComponent()->SetGridPos(-3, -3);
	XMFLOAT2 gridSpawnPos = GridComponent::GetSpawnPosition();
	GetTransform()->Translate(gridSpawnPos.x + -3.f * GridComponent::GetGridSize(),
		0.f,
		gridSpawnPos.y + -3.f * GridComponent::GetGridSize());

	//Physics
	RigidBodyComponent* pRigidBody = /*m_pModel->*/GetComponent<RigidBodyComponent>();
	if (!pRigidBody)
		return;
	pRigidBody->SetConstraint(RigidBodyConstraint::AllTrans, true);
	pRigidBody->SetConstraint(RigidBodyConstraint::AllRot, true);
	GetScene()->GetPhysxProxy()->GetPhysxScene()->removeActor(*pRigidBody->GetPxRigidActor());
}
