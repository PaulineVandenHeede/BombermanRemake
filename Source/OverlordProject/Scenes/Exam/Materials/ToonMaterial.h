#pragma once
class ToonMaterial final : public Material<ToonMaterial>
{
public:
	ToonMaterial();
	~ToonMaterial() = default;

	ToonMaterial(const ToonMaterial&) = delete;
	ToonMaterial(ToonMaterial&&) noexcept = delete;
	ToonMaterial& operator=(const ToonMaterial&) = delete;
	ToonMaterial& operator=(ToonMaterial&&) noexcept = delete;

	void SetColor(const XMFLOAT4& colour) const;

protected:
	void InitializeEffectVariables() override;
};

