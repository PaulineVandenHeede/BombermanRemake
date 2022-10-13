#pragma once
class PostBlur final : public PostProcessingMaterial
{
public:
	PostBlur();
	~PostBlur() override = default;

	PostBlur(const PostBlur&) = delete;
	PostBlur(PostBlur&&) noexcept = delete;

	PostBlur& operator=(const PostBlur&) = delete;
	PostBlur& operator=(PostBlur&&) noexcept = delete;

protected:
	void Initialize(const GameContext&) override {}
};

