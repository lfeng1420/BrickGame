#pragma once

class CGameScene : public LayerColor, public IEventHandler
{
public:
	virtual ~CGameScene();

	// Create scene
	static Scene* CreateScene(const TGameSceneContext* pContext = nullptr);

	// create
	static LayerColor* create(const TGameSceneContext* pContext = nullptr);

	// Init
	bool init();

	// On event
	void OnEvent(int nEventID, const char* pContext, int nLen);

	// On vote
	bool OnVote(int nEventID, char* pContext, int nLen);

	// frame update
	void update(float dt);

	// Import context first step
	void ImportContextFirst(const TGameSceneContext* pContext);

	// Import context last step
	void ImportContextLast(const TGameSceneContext* pContext);

public:
	// Init ui
	virtual void InitUI();

	// Update level or speed
	virtual void UpdateLevelOrSpeed(Vector<Sprite*> vecSpr, int& nOldVal, int nNewVal);

	// Adjust clicked button id
	virtual bool AdjustClickBtnID(const Vec2* pos, int& nBtnID);

protected:
	// Create key event listener
	void __CreateKeyListener();

	// Play bgmusic
	void __PlayBGM(bool bPlayFlag, bool bLoopFlag = true);

	// On button click
	void __OnClickButton(Ref* pSender, int nBtnID);

    // On menu touch
    void __OnMenuTouch(Ref* pSender, MenuItem** pSelectedMenuItem, Menu::TouchState enTouchState);

	// Export context
	void __ExportContext(TGameSceneContext* pContext);

private:
	// On game over
	void __OnGameOver(const char* pContext, int nLen);

	// On game data update
	void __OnGameDataUpdate(const char* pContext, int nLen);

	// On brick state update
	void __OnBrickStateUpdate(const char* pContext, int nLen);

	// On bgm update
	void __OnBGMUpdate(const char* pContext, int nLen);

	// On play effect
	void __OnPlayEffect(const char* pContext, int nLen);

	// On vibrate
	void __OnVibrate(const char* pContext, int nLen);

	// Initialize all bricks
	void __InitAllBricks(float& fLeftX, float& fTopY);

	// Initialize controller
	void __InitController(float fTopY, float fBottomY);

	// Initialize menu
	void __InitBottomMenu(float& fBottomY);

	// Initialize all labels
	void __InitRightUI(float fLeftX, float fBottomY);

	// Initialize tips
	void __InitTips();

	// Apply right hand mode
	void __ApplyRightHandMode();

	// Update score, current score or max score
	void __UpdateScore(Vector<Sprite*>& vecSpr, int& nOldScore, int nNewScore);

	// On press a button
	void __OnButtonPressed(int nButtonID);

	// On release a button
	void __OnButtonReleased(int nButtonID);

	// Click start button
	void __OnClickStartBtn();

	// Click sound button
	void __OnClickSoundBtn();

	// Click reset button
	void __OnClickResetBtn();

	// Click setup button
	void __OnClickSetupBtn();

	// Play effect
	void __PlayEffect(const char* szEffect);

protected:
	enum
	{
		SCORE_NUM_COUNT = 6,
		SPEED_NUM_COUNT = 2,
		LEVEL_NUM_COUNT = 2,
		LEVEL_SPEED_NUM_PADDING = 2,
		CONTROLLER_INNER_PADDING = 2,
		TIPS_LABEL_SIZE = 36,
	};

protected:
	// Bricks state
	bool				m_arrBrickState[ROW_COUNT * COLUMN_COUNT];

	// Small bricks state
	bool				m_arrSmallBrickState[SMALL_BRICK_COUNT];

	// All bricks sprite
	Vector<Sprite*>		m_vecBrickSpr;

	// All small bricks sprite
	Vector<Sprite*>		m_vecSmallBrickSpr;

	// Score labels
	Vector<Sprite*>		m_vecScoreSpr;

	// Max score labels
	Vector<Sprite*>		m_vecMaxScoreSpr;

	// Level labels
	Vector<Sprite*>		m_vecLevelSpr;

	// Speed labels
	Vector<Sprite*>		m_vecSpeedSpr;

	// Start button
	MenuItemToggle*		m_pStartBtn;

	// Sound button
	MenuItemToggle*		m_pSoundBtn;

	// Reset button
	MenuItemSprite*		m_pResetBtn;

	// Setup button
	MenuItemSprite*		m_pSetupBtn;

	// Direction button
	Vector<MenuItem*>	m_vecDirBtn;

	// Current game data
	SEventContextDataUpdate	m_stGameData;

	// Controller center pos
	Vec2				m_oControllerCenterPos;

	// Controller center area size
	Size				m_oControllerCenterSize;

	// Game pause sprite
	Sprite*				m_pPauseSpr;

	// Misc sprite vector
	Vector<Sprite*>		m_vecMiscSpr;

	// Temp string
	string				m_strSprName;

	// Tips
	Label*				m_pTipsLabel;

	// Playing bgm flag
	bool				m_bPlayingBGMFlag;

	// Controller last pressed button id
	EnButtonID			m_enLeftBtnID;
	EnButtonID			m_enRightBtnID;
};