#pragma once
#include "Misc/PostProcessingMaterial.h"

class Backdrop final : public PostProcessingMaterial
{
public:
	Backdrop();
	~Backdrop() = default;

	Backdrop(const Backdrop&) = delete;
	Backdrop(Backdrop&&) noexcept = delete;

	Backdrop& operator=(const Backdrop&) = delete;
	Backdrop& operator=(Backdrop&&) noexcept = delete;

	void SetBackdropTexture(const std::wstring& assetFile);
	void SetClearColour(const XMFLOAT4& clearColour);

protected:
	void Initialize(const GameContext& gameContext) override;

private:
	TextureData* m_pBackdropTexture;
};

