#include "stdafx.h"
#include "ToonMaterialTex.h"

ToonMaterialTex::ToonMaterialTex()
	: Material<ToonMaterialTex>{ L"Effects/ToonShaderTex.fx"}
{
}

void ToonMaterialTex::SetDiffuseMap(const std::wstring& assetFile)
{
	m_pTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pTexture);
}

void ToonMaterialTex::InitializeEffectVariables()
{
	//Ambient
	//Light
	
	SetVariable_Vector(L"gLightDirection", DirectX::XMFLOAT3{ 0.551f, -0.534f, -0.359f });
	//SetVariable_Vector(L"gLightDirection", DirectX::XMFLOAT3{ 0.0f, -0.534f, -0.641f });
	//Specular
	//Rim
}
