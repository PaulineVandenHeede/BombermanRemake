#include "stdafx.h"
#include "Backdrop.h"

Backdrop::Backdrop()
	: PostProcessingMaterial(L"Effects/Post/BackDrop.fx")
{
}


void Backdrop::SetBackdropTexture(const std::wstring& assetFile)
{
	m_pBackdropTexture = ContentManager::Load<TextureData>(assetFile);
	m_pBaseEffect->GetVariableByName("gBackDrop")->AsShaderResource()->SetResource(m_pBackdropTexture->GetShaderResourceView());
}

void Backdrop::SetClearColour(const XMFLOAT4& clearColour)
{
	m_pBaseEffect->GetVariableByName("gClearColour")->AsVector()->SetFloatVector(&clearColour.x);
}

void Backdrop::Initialize(const GameContext&)
{
}
