#pragma once
#include "UI/UI.h"

class BomberMan;
class Backdrop;

class WonMenuScene final : public GameScene
{
public:
	WonMenuScene();
	virtual ~WonMenuScene() override = default;

	WonMenuScene(const WonMenuScene&) = delete;
	WonMenuScene(WonMenuScene&&) noexcept = delete;

	WonMenuScene& operator=(const WonMenuScene&) = delete;
	WonMenuScene& operator=(WonMenuScene&&) noexcept = delete;


protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	Backdrop* m_pBackdrop;
	BomberMan* m_pPlayer;
	SpriteFont* m_pFontBig;
	TextElementUI m_Text;


	float m_Timer;
	float m_Interval;
};

