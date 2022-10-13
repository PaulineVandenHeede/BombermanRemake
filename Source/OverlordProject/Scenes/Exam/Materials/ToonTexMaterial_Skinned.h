#pragma once
class ToonTexMaterial_Skinned: public Material<ToonTexMaterial_Skinned>
{
public:
	ToonTexMaterial_Skinned();
	~ToonTexMaterial_Skinned() override = default;

	ToonTexMaterial_Skinned(const ToonTexMaterial_Skinned& other) = delete;
	ToonTexMaterial_Skinned(ToonTexMaterial_Skinned&& other) noexcept = delete;
	ToonTexMaterial_Skinned& operator=(const ToonTexMaterial_Skinned& other) = delete;
	ToonTexMaterial_Skinned& operator=(ToonTexMaterial_Skinned&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* /*pModel*/) const;

private:
	TextureData* m_pDiffuseTexture{};
};

