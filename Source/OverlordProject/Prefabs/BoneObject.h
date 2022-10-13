#pragma once
class BoneObject final : public GameObject
{
public:
	BoneObject(BaseMaterial* pMaterial, float length = 5.f);
	~BoneObject() override = default;

	BoneObject(const BoneObject&) = delete;
	BoneObject(BoneObject&&) = delete;

	BoneObject& operator=(const BoneObject&) noexcept = delete;
	BoneObject& operator=(BoneObject&&) noexcept = delete;

	void AddBone(BoneObject* pBone);

	//[PART 2]
	const XMFLOAT4X4& GetBindPose() const { return m_BindPose; }
	void CalculateBindPose();

protected:
	void Initialize(const SceneContext&) override;

private:
	float m_Length{};
	BaseMaterial* m_pMaterial;

	//[PART 2]
	XMFLOAT4X4 m_BindPose{};
};

