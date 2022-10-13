#pragma once

class Backdrop;

enum PauseMenuState
{
	pause1,
	pause2,
	pause3,
	pauseControl
};

enum inputPauseMenu
{
	pauseBack,
	pauseNext,
	pauseUp,
	pauseDown,
	pauseEnter,
};

class PauseMenuScene final : public GameScene
{
public:
	PauseMenuScene();
	virtual ~PauseMenuScene() override = default;

	PauseMenuScene(const PauseMenuScene&) = delete;
	PauseMenuScene(PauseMenuScene&&) noexcept = delete;

	PauseMenuScene& operator=(const PauseMenuScene&) = delete;
	PauseMenuScene& operator=(PauseMenuScene&&) noexcept = delete;


protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	Backdrop* m_pBackdrop;
	PauseMenuState m_State;

	FMOD::ChannelGroup* m_pSoundEffectGroup{ nullptr };
	FMOD::Sound* m_pSoundEffectNext{ nullptr };
	FMOD::Sound* m_pSoundEffectMove{ nullptr };
	
	void UpdateMenu1();
	void UpdateMenu2();
	void UpdateMenu3();
	void UpdateControlMenu();
};

