#include "stdafx.h"
#include "ToonTexMaterial_Shadow.h"

ToonTexMaterial_Shadow::ToonTexMaterial_Shadow():
	Material(L"Effects/Shadow/ToonShaderTex_Shadow.fx")
{}

void ToonTexMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void ToonTexMaterial_Shadow::InitializeEffectVariables()
{
}

void ToonTexMaterial_Shadow::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	 *
	 * 2. Update the ShadowMap texture
	 *
	 * 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	*/
	const auto pShadowMapRenderer = ShadowMapRenderer::Get();

	const XMMATRIX world = DirectX::XMLoadFloat4x4(&pModel->GetTransform()->GetWorld());
	const XMMATRIX light = DirectX::XMLoadFloat4x4(&pShadowMapRenderer->GetLightVP());
	const XMMATRIX worldViewProjLight = DirectX::XMMatrixMultiply(world, light);
	XMFLOAT4X4 worldViewProjLightMatrix{};
	DirectX::XMStoreFloat4x4(&worldViewProjLightMatrix, worldViewProjLight);
	SetVariable_Matrix(L"gWorldViewProj_Light", worldViewProjLightMatrix);

	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());

	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	//Update Shadow Variables
	//const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	//...
}
