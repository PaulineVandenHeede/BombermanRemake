#pragma once
class ToonMaterialTex final : public Material<ToonMaterialTex>
{
public:
	ToonMaterialTex();
	~ToonMaterialTex() = default;

	ToonMaterialTex(const ToonMaterialTex&) = delete;
	ToonMaterialTex(ToonMaterialTex&&) noexcept = delete;
	ToonMaterialTex& operator=(const ToonMaterialTex&) = delete;
	ToonMaterialTex& operator=(ToonMaterialTex&&) noexcept = delete;

	void SetDiffuseMap(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pTexture;
};

