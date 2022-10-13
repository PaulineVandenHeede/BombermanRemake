#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: GameObject{}
	, m_Length{ length }
	, m_pMaterial{ pMaterial }
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	XMFLOAT3 pos = pBone->GetTransform()->GetPosition();
	pBone->GetTransform()->Translate(pos.x + m_Length, pos.y, pos.z);

	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	//bindpose is invese of world matrix
	XMMATRIX bindpose = XMLoadFloat4x4(&GetTransform()->GetWorld());
	bindpose = DirectX::XMMatrixInverse(nullptr, bindpose);
	XMStoreFloat4x4(&m_BindPose, bindpose);

	for (BoneObject* pBone : GetChildren<BoneObject>())
	{
		pBone->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* const pEmpty = new GameObject{};
	AddChild(pEmpty);

	ModelComponent* const pModel = pEmpty->AddComponent<ModelComponent>(new ModelComponent{ L"Meshes/Bone.ovm" });
	pModel->SetMaterial(m_pMaterial);

	pEmpty->GetTransform()->Rotate(0.f, -90.f, 0.f);
	pEmpty->GetTransform()->Scale(m_Length, m_Length, m_Length);
}
