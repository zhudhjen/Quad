#ifndef __BTN_EVENT_RECEIVER__
#define __BTN_EVENT_RECEIVER__


#include "common.h"
#include "View.h"
#include <map>

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_ONE_PLAYER,//����ģʽ
	GUI_ID_TWO_PLAYER,//˫��ģʽ
	GUI_ID_EASY_MODE,//��
	GUI_ID_NORMAL_MODE,//�е�
	GUI_ID_DIFFICULT_MODE,//����
	GUI_ID_BACK_BUTTON, //���ػ�ӭ����
	GUI_ID_RESTART,//���¿�ʼ
	GUI_ID_MENU //gameView�е�backBtn

};

struct MouseState {
	irr::core::position2di Position;
	bool LeftButtonDown;
	bool MiddleButtonDown;
	bool RightButtonDown;
	MouseState() : LeftButtonDown(false), MiddleButtonDown(false), RightButtonDown(false) {}
};

struct SAppContext
{
	irr::IrrlichtDevice *device;
	std::map <std::string, irr::gui::IGUIButton * > ptrBtn;   //������------������ַ

};



class MouseEventReceiver : public irr::IEventReceiver
{
public:
	MouseEventReceiver(SAppContext & context) :
		Context(context),
		m_game_mode(),
		m_new_view(WINDOW::GAME),
		m_ready(false),
		m_menu(false),
		m_restart(false)

	{ }

	virtual bool OnEvent(const irr::SEvent& event)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
			switch (event.MouseInput.Event) {
			case irr::EMIE_LMOUSE_PRESSED_DOWN:
				m_mouse_state.LeftButtonDown = true;
				break;

			case irr::EMIE_LMOUSE_LEFT_UP:
				m_mouse_state.LeftButtonDown = false;
				break;

			case irr::EMIE_MMOUSE_PRESSED_DOWN:
				m_mouse_state.RightButtonDown = true;
				break;
			case irr::EMIE_MMOUSE_LEFT_UP:
				m_mouse_state.RightButtonDown = false;
				break;

			case irr::EMIE_RMOUSE_PRESSED_DOWN:
				m_mouse_state.RightButtonDown = true;
				break;
			case irr::EMIE_RMOUSE_LEFT_UP:
				m_mouse_state.RightButtonDown = false;
				break;

			case irr::EMIE_MOUSE_MOVED:
				m_mouse_state.Position.X = event.MouseInput.X;
				m_mouse_state.Position.Y = event.MouseInput.Y;
				break;

			default:
				break;
			}
		}
		if (event.EventType == irr::EET_GUI_EVENT)
		{
			irr::s32 id = event.GUIEvent.Caller->getID();
			irr::gui::IGUIEnvironment* env = Context.device->getGUIEnvironment();

			if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
			{
				switch (id)
				{
				case GUI_ID_QUIT_BUTTON:
				{
					Context.device->closeDevice();
					return true;
				}
				case GUI_ID_ONE_PLAYER:
				{
					//ת���Ѷ�ѡ��
					((irr::gui::IGUIButton *)(Context.ptrBtn["onePlayer"]))->setVisible(false);
					((irr::gui::IGUIButton *)(Context.ptrBtn["twoPlayer"]))->setVisible(false);
					((irr::gui::IGUIButton *)(Context.ptrBtn["quit"]))->setVisible(false);
					((irr::gui::IGUIButton *)(Context.ptrBtn["easy"]))->setVisible(true);
					((irr::gui::IGUIButton *)(Context.ptrBtn["normal"]))->setVisible(true);
					((irr::gui::IGUIButton *)(Context.ptrBtn["difficult"]))->setVisible(true);
					((irr::gui::IGUIButton *)(Context.ptrBtn["back"]))->setVisible(true);
					m_game_mode.setPvp(false);
					return true;
				}
				case GUI_ID_TWO_PLAYER:
				{
					//ת������ģʽ
					m_game_mode.setPvp(true);
					m_new_view = WINDOW::GAME;
					m_ready = true;
					return true;
				}
				case GUI_ID_EASY_MODE:
				{//�Ѷȼ�
					m_game_mode.setLevel(LEVEL::EASY);
					m_new_view = WINDOW::GAME;
					m_ready = true;
					return true;
				}
				case GUI_ID_NORMAL_MODE:
				{//�Ѷ��е�
					m_game_mode.setLevel(LEVEL::NORMAL);
					m_new_view = WINDOW::GAME;
					m_ready = true;
					return true;
				}
				case GUI_ID_DIFFICULT_MODE:
				{//�Ѷ�����
					m_game_mode.setLevel(LEVEL::HARD);
					m_new_view = WINDOW::GAME;
					m_ready = true;
					return true;
				}
				case GUI_ID_BACK_BUTTON:
				{//���ص���ӭ����
					((irr::gui::IGUIButton *)(Context.ptrBtn["onePlayer"]))->setVisible(true);
					((irr::gui::IGUIButton *)(Context.ptrBtn["twoPlayer"]))->setVisible(true);
					((irr::gui::IGUIButton *)(Context.ptrBtn["quit"]))->setVisible(true);
					((irr::gui::IGUIButton *)(Context.ptrBtn["easy"]))->setVisible(false);
					((irr::gui::IGUIButton *)(Context.ptrBtn["normal"]))->setVisible(false);
					((irr::gui::IGUIButton *)(Context.ptrBtn["difficult"]))->setVisible(false);
					((irr::gui::IGUIButton *)(Context.ptrBtn["back"]))->setVisible(false);
					return true;
				}
				case GUI_ID_RESTART:
				{
					//���¿�ʼ
					m_restart = true;
					return true;
				}
				case GUI_ID_MENU:
				{
					//����Ϸ���淵�ػ�ӭ����
					m_new_view = WINDOW::TITLE;
					m_menu = true;
					return true;
				}
				default:
					return false;
				}
			}
		}
		return false;
	}

	const GameMode getGameMode() const {
		return m_game_mode;
	}

	const WINDOW getNewView() const {
		return m_new_view;
	}

	const bool getReady(void) const {
		return m_ready;
	}

	const bool getRestart(void) const {
		return m_restart;
	}

	void setRestart(const bool restart) {
		m_restart = restart;
	}

	const bool getMenu(void) const {
		return m_menu;
	}

	void setMenu(const bool menu) {
		m_menu = menu;
	}

	const MouseState & getMouseState(void) const {
		return m_mouse_state;
	}

private:
	SAppContext & Context;
	MouseState m_mouse_state;
	GameMode m_game_mode;
	WINDOW m_new_view;
	bool m_ready, m_menu, m_restart;
};

#endif