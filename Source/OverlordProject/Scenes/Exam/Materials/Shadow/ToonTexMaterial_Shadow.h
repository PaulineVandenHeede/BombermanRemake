#pragma once
class ToonTexMaterial_Shadow final : public Material<ToonTexMaterial_Shadow>
{
public:
	ToonTexMaterial_Shadow();
	~ToonTexMaterial_Shadow() override = default;

	ToonTexMaterial_Shadow(const ToonTexMaterial_Shadow& other) = delete;
	ToonTexMaterial_Shadow(ToonTexMaterial_Shadow&& other) noexcept = delete;
	ToonTexMaterial_Shadow& operator=(const ToonTexMaterial_Shadow& other) = delete;
	ToonTexMaterial_Shadow& operator=(ToonTexMaterial_Shadow&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext&, const ModelComponent*) const override;

private:
	TextureData* m_pDiffuseTexture{};
};

