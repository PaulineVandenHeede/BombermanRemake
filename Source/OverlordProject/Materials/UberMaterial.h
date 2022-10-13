#pragma once
class UberMaterial final : public Material<UberMaterial>
{
public:
	UberMaterial() : Material<UberMaterial>(L"Effects/UberShader.fx") {  }
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial&) = default;
	UberMaterial(UberMaterial&&) noexcept = default;

	UberMaterial& operator=(const UberMaterial&) = default;
	UberMaterial& operator=(UberMaterial&&) noexcept = default;

protected:
	void InitializeEffectVariables() override;

private:
};

