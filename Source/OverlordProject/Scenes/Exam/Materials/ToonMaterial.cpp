#include "stdafx.h"
#include "ToonMaterial.h"

ToonMaterial::ToonMaterial()
	: Material<ToonMaterial>{ L"Effects/ToonShader.fx"}
{
}

void ToonMaterial::SetColor(const XMFLOAT4& color) const
{
	SetVariable_Vector(L"gColorDiffuse", color);
}

void ToonMaterial::InitializeEffectVariables()
{
	//Ambient
	//Light
	
	SetVariable_Vector(L"gLightDirection", DirectX::XMFLOAT3{ 0.551f, -0.534f, -0.359f });
	//SetVariable_Vector(L"gLightDirection", DirectX::XMFLOAT3{ 0.0f, -0.534f, -0.641f });
	//Specular
	//Rim
}
