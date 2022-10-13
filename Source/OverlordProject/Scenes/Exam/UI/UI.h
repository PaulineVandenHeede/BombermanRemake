#pragma once

enum class ElementType
{
	PlayerOne,
	PlayerTwo,
	PlayerThree,
	PlayerFour,
	Text,
};

struct ElementUI
{
	XMFLOAT3 position;
	XMFLOAT4 colour;
};

struct TextElementUI : public ElementUI
{
	std::wstring text;
	uint32_t idFont;
};

struct SpriteElementUI : public ElementUI
{
	GameObject* pSprite;
};

struct PlayerUI
{
	TextElementUI name{};
	SpriteElementUI image{};
	SpriteElementUI stars[3]{};
	uint32_t roundsWon{ 0 };
};

class UI
{
public:
	UI() = default;
	virtual ~UI();

	UI(const UI&) = delete;
	UI(UI&&) noexcept = delete;

	UI& operator=(const UI&) = delete;
	UI& operator=(UI&&) noexcept = delete;

	virtual void Initialize(GameScene* const pScene, const SceneContext& sceneContext);
	void Update(GameScene* const pScene);

	uint32_t AddTextUIElement(const TextElementUI& text) { m_TextElements.push_back(text); return m_NrOfTexts++; }
	void UpdateText(uint32_t id, const std::wstring& text) { m_TextElements[id].text = text; }

	uint32_t AddSpriteUIElement(const SpriteElementUI& sprite) { m_SpriteElements.push_back(sprite); return m_NrOfSprites++; }

	const SpriteElementUI& GetSpriteElement(uint32_t id) const { return m_SpriteElements[id]; }
	const TextElementUI& GetTextElement(uint32_t id) const { return m_TextElements[id]; }
	const PlayerUI& GetPlayerUI(uint32_t id) const { return m_PlayerElements[id]; }

	void UpdatePosText(uint32_t id, const XMFLOAT3& pos) { m_TextElements[id].position = pos; }
	void UpdatePos(uint32_t id, const XMFLOAT3& pos) { m_SpriteElements[id].position = pos; }
	void UpdatePosPlayer(uint32_t id, const XMFLOAT3& pos) { m_PlayerElements[id].stars[0].position = pos; }
	void UpdatePlayerInfo(uint32_t id);
	void ResetPlayerInfo(uint32_t id);


protected:
	std::vector<TextElementUI> m_TextElements;
	std::vector<SpriteElementUI> m_SpriteElements;
	std::vector<SpriteFont*> m_pFonts;

	GameObject* pFullStar;
	GameObject* pEmptyStar;

	std::vector<PlayerUI> m_PlayerElements;
	std::vector<std::pair<XMFLOAT3,XMFLOAT3>> m_PlayerSpawnPositions;

	uint32_t m_NrOfTexts;
	uint32_t m_NrOfSprites;
};


class HUD final : public UI
{
public:
	HUD() = default;
	virtual ~HUD() override = default;

	HUD(const HUD&) = delete;
	HUD(HUD&&) noexcept = delete;

	HUD& operator=(const HUD&) = delete;
	HUD& operator=(HUD&&) noexcept = delete;

	void Initialize(GameScene* const pScene, const SceneContext& sceneContext) override;
};