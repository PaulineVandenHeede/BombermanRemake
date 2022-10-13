#pragma once
#include "UI/UI.h"

class Backdrop;
class BomberMan;

enum MainMenuState
{
	begin,
	main1,
	main2,
	main3,
	control,
	players
};

enum input
{
	back,
	next,
	up,
	down,
	enter,
	addPlayerOne,
	addPlayerTwo,
	addPlayerThree,
	addPlayerFour
};

class MainMenuScene final : public GameScene
{
public:
	MainMenuScene();
	virtual ~MainMenuScene() override = default;

	MainMenuScene(const MainMenuScene&) = delete;
	MainMenuScene(MainMenuScene&&) noexcept = delete;

	MainMenuScene& operator=(const MainMenuScene&) = delete;
	MainMenuScene& operator=(MainMenuScene&&) noexcept = delete;

	static uint32_t GetNrOfPlayers() { return m_NrOfPlayers; }

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	SpriteFont* m_pFontBig;
	SpriteFont* m_pFontNormal;
	BomberMan* m_pPlayerOne;
	BomberMan* m_pPlayerTwo;
	BomberMan* m_pPlayerThree;
	BomberMan* m_pPlayerFour;

	Backdrop* m_pBackdrop;

	FMOD::ChannelGroup* m_pSoundEffectGroup{ nullptr };
	FMOD::Sound* m_pSoundEffectNext{ nullptr };
	FMOD::Sound* m_pSoundEffectMove{ nullptr };
	
	TextElementUI m_Text;
	TextElementUI m_AddPlayerOne;
	TextElementUI m_AddPlayerTwo;
	TextElementUI m_AddPlayerThree;
	TextElementUI m_AddPlayerFour;

	float m_Timer;
	float m_Interval;
	float m_IntervalShort;

	MainMenuState m_State;
	static uint32_t m_NrOfPlayers;

	void UpdateBegin();
	void UpdateMenu1();
	void UpdateMenu2();
	void UpdateMenu3();
	void UpdateControlMenu();
	void UpdatePlayersMenu();
};

