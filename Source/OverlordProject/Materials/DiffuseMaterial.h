#pragma once


class DiffuseMaterial final : public Material<DiffuseMaterial>
{
public:
	DiffuseMaterial() : Material<DiffuseMaterial>{ L"Effects/PosNormTex3D.fx" } { }
	~DiffuseMaterial() override = default;

	DiffuseMaterial(const DiffuseMaterial&) = delete;
	DiffuseMaterial(DiffuseMaterial&&) noexcept = delete;

	DiffuseMaterial& operator=(const DiffuseMaterial&) = delete;
	DiffuseMaterial& operator=(DiffuseMaterial&&) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture{ nullptr };
};

