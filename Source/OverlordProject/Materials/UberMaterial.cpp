#include "stdafx.h"
#include "UberMaterial.h"

void UberMaterial::InitializeEffectVariables()
{
	//LIGHT
	SetVariable_Vector(L"gLightDirection", DirectX::XMFLOAT3{ 0.551f, -0.534f, 0.641f });

	//DIFFUSE
	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Scalar(L"gUseHalfLambertDiffuse", false);
	SetVariable_Vector(L"gColorDiffuse", static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Red));
	TextureData* pDiffuseTexture = ContentManager::Load<TextureData>(L"Textures/Skulls_Diffusemap.tga");
	SetVariable_Texture(L"gTextureDiffuse", pDiffuseTexture);

	//SPECULAR
	SetVariable_Vector(L"gColorSpecular", static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Green));
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", true);
	TextureData* pSpecularIntensityTexture = ContentManager::Load<TextureData>(L"Textures/Skulls_Heightmap.tga");
	SetVariable_Texture(L"gTextureSpecularIntensity", pSpecularIntensityTexture);
	SetVariable_Scalar(L"gShininess", 40);

	//AMBIENT
	SetVariable_Vector(L"gColorAmbient", static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
	SetVariable_Scalar(L"gAmbientIntensity", 0.f);

	//NORMAL MAPPING
	SetVariable_Scalar(L"gFlipGreenChannel", true);
	SetVariable_Scalar(L"gUseTextureNormal", true);
	TextureData* pNormalTexture = ContentManager::Load<TextureData>(L"Textures/Skulls_Normalmap.tga");
	SetVariable_Texture(L"gTextureNormal", pNormalTexture);

	//ENVIRONMENT MAPPING
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);
	TextureData* pEnvironmentTexture = ContentManager::Load<TextureData>(L"Textures/Sunol_Cubemap.dds");
	SetVariable_Texture(L"gCubeEnvironment", pEnvironmentTexture);
	SetVariable_Scalar(L"gReflectionStrength", 0.8f);
	SetVariable_Scalar(L"gRefractionStrength", 0.5f);
	SetVariable_Scalar(L"gRefractionIndex", 0.9f);

	//FRESNEL FALLOFF
	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Vector(L"gColorFresnel", static_cast<DirectX::XMFLOAT4>(DirectX::Colors::White));
	SetVariable_Scalar(L"gFresnelPower", 2.f);
	SetVariable_Scalar(L"gFresnelMultiplier", 1.5f);
	SetVariable_Scalar(L"gFresnelHardness", 2.f);

	//OPACITY
	SetVariable_Scalar(L"gOpacityIntensity", 1.f);
	SetVariable_Scalar(L"gTextureOpacityIntensity", false);
	//SetVariable_Texture(L"gTextureOpacity", m_pOpacityIntensityTexture);

	//SPECULAR MODELS
	SetVariable_Scalar(L"gUseSpecularBlinn", false);
	SetVariable_Scalar(L"gUseSpecularPhong", true);
}
