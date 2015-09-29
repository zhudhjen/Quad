#ifndef __TITLE_VIEW_H__
#define __TITLE_VIEW_H__

#include "common.h"
#include "MouseEventReceiver.h"
#include "View.h"
#include "TitleViewSink.h"
#include <map>

class TitleView : public View, public TitleViewSink<TitleView> {
public:
	explicit TitleView(irr::IrrlichtDevice *device);
	~TitleView(void);

	/* Initialize */
	void initialize(void);

	/* Drop */
	void drop(void);

	/* Animate */
	void update(void);

	void setGameStartCommand(const RefPtr<ICommandEx> &game_start_command);
	void setSwitchWindowCommand(const RefPtr<ICommandEx> &switch_window_command);

	void onGameStartCommandComplete(const bool success);
private:
	SAppContext *m_context;
	MouseEventReceiver *m_receiver;
	GameMode m_game_mode;
	WINDOW m_new_view;

	RefPtr<ICommandEx> m_game_start_command;
	RefPtr<ICommandEx> m_switch_window_command;
};

#endif

TitleView::TitleView(irr::IrrlichtDevice *device) :
	View(device),
	TitleViewSink(this),
	m_context(NULL),
	m_receiver(NULL),
	m_game_mode(),
	m_new_view(WINDOW::GAME)
{

}

TitleView::~TitleView(void) {

}

void TitleView::initialize(void) {
	irr::IrrlichtDevice *device = getIrrlichtDevice();
	irr::gui::IGUIEnvironment *env = device->getGUIEnvironment();
	irr::video::IVideoDriver* driver = device->getVideoDriver();

	irr::gui::IGUISkin* skin = env->getSkin();//使字体更加美观
	irr::gui::IGUIFont* font = env->getFont("../media/fonthaettenschweiler.bmp");
	
	if (font)
		skin->setFont(font);
	skin->setFont(env->getBuiltInFont(), irr::gui::EGDF_TOOLTIP);


	//env->addStaticText(L"QUAD", irr::core::rect<irr::s32>(300, 100, 400, 140), true);  //加入静态文本
	env->addImage(driver->getTexture("../media/quad.png"), irr::core::position2d<int>(260, 100));

	//加入按键
	irr::gui::IGUIButton * p_onePlayer = env->addButton(irr::core::rect<irr::s32>(300, 220, 500, 220 + 60), 0, GUI_ID_ONE_PLAYER,
		L"1 PLAYER", L"");
	p_onePlayer->setVisible(true);
	
	irr::gui::IGUIButton * p_twoPlayer = env->addButton(irr::core::rect<irr::s32>(300, 290, 500, 290 + 60), 0, GUI_ID_TWO_PLAYER,
		L"2 PLAYERS", L"");
	p_twoPlayer->setVisible(true);
	irr::gui::IGUIButton * p_quit = env->addButton(irr::core::rect<irr::s32>(300, 360, 500, 360 + 60), 0, GUI_ID_QUIT_BUTTON,
		L"QUIT", L"");
	p_quit->setVisible(true);


	irr::gui::IGUIButton * p_easy = env->addButton(irr::core::rect<irr::s32>(300, 220, 500, 220 + 60), 0, GUI_ID_EASY_MODE,
		L"EASY", L"");
	p_easy->setVisible(false);
	irr::gui::IGUIButton * p_normal = env->addButton(irr::core::rect<irr::s32>(300, 290, 500, 290 + 60), 0, GUI_ID_NORMAL_MODE,
		L"NORMAL", L"");
	p_normal->setVisible(false);
	irr::gui::IGUIButton * p_difficult = env->addButton(irr::core::rect<irr::s32>(300, 360, 500, 360 + 60), 0, GUI_ID_DIFFICULT_MODE,
		L"HARD", L"");
	p_difficult->setVisible(false);
	irr::gui::IGUIButton * p_back = env->addButton(irr::core::rect<irr::s32>(300, 430, 500, 430 + 32), 0, GUI_ID_BACK_BUTTON,
		L"BACK", L"");
	p_back->setVisible(false);
	


	// Store the appropriate data in a context structure.
	m_context = new SAppContext;
	m_context->device = device;
	m_context->ptrBtn["onePlayer"] = p_onePlayer;
	m_context->ptrBtn["twoPlayer"] = p_twoPlayer;
	m_context->ptrBtn["quit"] = p_quit;
	m_context->ptrBtn["easy"] = p_easy;
	m_context->ptrBtn["normal"] = p_normal;
	m_context->ptrBtn["difficult"] = p_difficult;
	m_context->ptrBtn["back"] = p_back;

	m_receiver = new MouseEventReceiver(*m_context);
	device->setEventReceiver(m_receiver);

	env->addImage(driver->getTexture("../media/irrlichtlogo2.png"), irr::core::position2d<int>(10, 10));

	View::initialize();
}
void TitleView::drop(void) {
	getIrrlichtDevice()->setEventReceiver(NULL);
	delete m_receiver;
	delete m_context;
	m_receiver = NULL;
	m_context = NULL;
	View::drop();
}
void TitleView::update(void) {
	if (!getEnable()) {
		return;
	}

	if (m_receiver->getReady()) {
		m_game_mode = m_receiver->getGameMode();
		m_new_view = m_receiver->getNewView();

		m_game_start_command.Deref().SetParameter(RefPtrHelper::TypeCast<SingleParameter<GameMode>, ICommandParameter>(RefPtr<SingleParameter<GameMode>>(&SingleParameter<GameMode>(m_game_mode))));
		m_switch_window_command.Deref().SetParameter(RefPtrHelper::TypeCast<SingleParameter<WINDOW>, ICommandParameter>(RefPtr<SingleParameter<WINDOW>>(&SingleParameter<WINDOW>(m_new_view))));

		m_game_start_command.Deref().Exec();
		return;
	}

	View::update();
}

void TitleView::setGameStartCommand(const RefPtr<ICommandEx> &game_start_command) {
	m_game_start_command = game_start_command;
}
void TitleView::setSwitchWindowCommand(const RefPtr<ICommandEx> &switch_window_command) {
	m_switch_window_command = switch_window_command;
}
void TitleView::onGameStartCommandComplete(const bool success) {
	if (!getEnable()) {
		return;
	}
	if (success) {
		m_switch_window_command.Deref().Exec();
	}
}
