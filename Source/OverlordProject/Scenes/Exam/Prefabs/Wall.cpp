#include "stdafx.h"
#include "Wall.h"

#include "../Materials/ToonMaterial.h"

Wall::Wall(WallType type, int32_t col, int32_t row)
	: GridBasedObject{ {col, row}, 0.1f }
	, m_Type{ type }
	, m_Destroyed{ false }
{
}

bool Wall::Destroy()
{
	if (m_Type == WallType::Hard)
		return false;

	m_Destroyed = true;
	return true;
}

bool Wall::Squish()
{
	m_Destroyed = true;
	return true;
}

void Wall::Initialize(const SceneContext&)
{
	/*m_pModel = AddChild(new GameObject{});*/
	ModelComponent* pModelComponent{ nullptr };
	ToonMaterial* const pToonMaterial = MaterialManager::Get()->CreateMaterial<ToonMaterial>();
	switch (m_Type)
	{
	case WallType::Fence:
		pModelComponent = AddComponent(new ModelComponent{ L"Meshes/Fence.ovm", false });
		pToonMaterial->SetColor(static_cast<XMFLOAT4>(Colors::DarkSlateGray));
		break;
	case WallType::Hard:
		{
			pModelComponent = AddComponent(new ModelComponent{ L"Meshes/Stone.ovm", false });
			pToonMaterial->SetColor((XMFLOAT4)Colors::LightGray);
			int angle = static_cast<int>(MathHelper::randF(0.f, 4.f));
			GetTransform()->Rotate(0.f, angle * 90.f, 0.f);
			break;
		}
	case WallType::Soft:
		pModelComponent = AddComponent(new ModelComponent{ L"Meshes/Box.ovm", false });
		pToonMaterial->SetColor((XMFLOAT4)Colors::SaddleBrown);
		break;
	}
	pModelComponent->SetMaterial(pToonMaterial);

	//collision
	physx::PxMaterial* const pxSphereMaterial = PxGetPhysics().createMaterial(0.2f, 0.2f, 0.8f);
	RigidBodyComponent* const pRigidBodyComponent = /*m_pModel->*/AddComponent<RigidBodyComponent>(new RigidBodyComponent{ true });
	physx::PxConvexMesh* const pxConvexMeshChair = ContentManager::Load<physx::PxConvexMesh>(L"Meshes/Box.ovpc");
	pRigidBodyComponent->AddCollider(PxConvexMeshGeometry{ pxConvexMeshChair }, *pxSphereMaterial);
}


