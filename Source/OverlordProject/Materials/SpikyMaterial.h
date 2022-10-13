#pragma once

class SpikyMaterial final : public Material<SpikyMaterial>
{
public:
	SpikyMaterial() : Material<SpikyMaterial>{ L"Effects/SpikyShader.fx" } {}
	~SpikyMaterial() override = default;
	 
	SpikyMaterial(const SpikyMaterial&) = delete;
	SpikyMaterial(SpikyMaterial&&) noexcept = delete;

	SpikyMaterial& operator=(const SpikyMaterial&) = delete;
	SpikyMaterial& operator=(SpikyMaterial&&) noexcept = delete;

protected:
	void InitializeEffectVariables() override;

private:


};

