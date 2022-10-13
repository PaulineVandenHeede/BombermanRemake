#include "stdafx.h"
#include "ToonTexMaterial_Skinned.h"

ToonTexMaterial_Skinned::ToonTexMaterial_Skinned():
	Material(L"Effects/ToonShaderTex_Skinned.fx")
{
}

void ToonTexMaterial_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void ToonTexMaterial_Skinned::InitializeEffectVariables()
{
}

void ToonTexMaterial_Skinned::OnUpdateModelVariables(const SceneContext&, const ModelComponent* pModel) const
{
	//Retrieve The Animator from the ModelComponent
	if (!pModel->HasAnimator())
		return;
	//Make sure the animator is not NULL (ASSERT_NULL_)
	ModelAnimator* pAnimator = pModel->GetAnimator();
	ASSERT_NULL_(pAnimator);
	//Retrieve the BoneTransforms from the Animator
	//Set the 'gBones' variable of the effect (MatrixArray) > BoneTransforms
	SetVariable_MatrixArray(L"gBones", *(pAnimator->GetBoneTransforms().data()->m), UINT(pAnimator->GetBoneTransforms().size()));
}
