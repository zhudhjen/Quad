#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "common.h"
#include "View.h"
#include "MouseEventReceiver.h"
#include "GameViewSink.h"
#include <map>
#include <vector>
#include <stdio.h>

class GameView : public View, public GameViewSink<GameView> {
public:
	explicit GameView(irr::IrrlichtDevice *device);
	~GameView(void);

	/* Initialize */
	void initialize(void);

	/* Drop */
	void drop(void);

	/* Animate */
	void update(void);

	/* Set game mode */
	void setGameModeRef(const RefPtr<GameMode> &game_mode);

	/* Set current player */
	void setCurrentPlayerRef(const RefPtr<COLOR> &current_player);

	/* Set status */
	void setStatusRef(const RefPtr<std::map<Position, std::vector<COLOR>>> &status);

	/* Set is column valid */
	void setIsColumnValidRef(const RefPtr<std::map<Position, bool>> &is_column_valid);

	/* Set last move */
	void setLastMoveRef(const RefPtr<Position> &last_move);

	/* Set undo count */
	void setUndoCountRef(const RefPtr<long> &undo_count);

	/* Set is game over */
	void setGameStateRef(const RefPtr<GAME_STATE> &is_game_over);

	/* Set AI decision */
	void setAiDecisionRef(const RefPtr<Position> &ai_decision);

	void setGameStartCommand(const RefPtr<ICommandEx> &game_start_command);
	void setPlayerMoveCommand(const RefPtr<ICommandEx> &player_move_command);
	void setAiMoveCommand(const RefPtr<ICommandEx> &ai_move_command);
	void setUndoCommand(const RefPtr<ICommandEx> &undo_command);
	void setSwitchWindowCommand(const RefPtr<ICommandEx> &switch_window_command);

	void onCurrentPlayerChange(void);
	void onGameStateChange(void);
	void onLastMoveChange(void);
	void onStatusChange(void);
	void onColumnValidChange(void);

	void onGameStartCommandComplete(const bool success);
	void onPlayerMoveCommandComplete(const bool success);
	void onAiMoveCommandComplete(const bool success);
	void onUndoCommandComplete(const bool success);
private:
	irr::scene::ICameraSceneNode *m_camera;
	irr::scene::ISceneNode *m_highlighted_scene_node, *m_origin_selected_scene_node;
	irr::scene::ISceneCollisionManager* m_collision_manager;

	bool m_ready, m_animating;
	irr::scene::IMeshSceneNode *m_board;
	std::map<Position, irr::scene::IMeshSceneNode*> m_column;
	std::map<Position, std::vector<irr::scene::IMeshSceneNode*>> m_beads;
	irr::scene::IMeshSceneNode *m_animating_bead, *m_next_bead;
	irr::core::vector3df m_animation_dest, m_fix_position, m_fix_rotation, m_fix_scale;
	irr::video::ITexture *m_dark_texture, *m_light_texture;

	irr::gui::IGUIStaticText* m_debug_text_1, * m_debug_text_2;

	MouseEventReceiver *m_mouse_receiver;
	MouseState m_last_mouse_state;

	/* References */
	RefPtr<GameMode> m_game_mode;
	RefPtr<COLOR> m_current_player;
	RefPtr<std::map<Position, std::vector<COLOR>>> m_status;
	RefPtr<std::map<Position, bool>> m_is_column_valid;
	RefPtr<Position> m_last_move;
	RefPtr<long> m_undo_count;
	RefPtr<GAME_STATE> m_game_state;
	RefPtr<Position> m_ai_decision;

	RefPtr<ICommandEx> m_game_start_command;
	RefPtr<ICommandEx> m_player_move_command;
	RefPtr<ICommandEx> m_ai_move_command;
	RefPtr<ICommandEx> m_undo_command;
	RefPtr<ICommandEx> m_switch_window_command;

	/* Check if the playground is ready for new action */
	const bool is_ready(void) const;

	SAppContext *m_context;

	WINDOW m_new_view;
	//GameMode m_game_mode;

	irr::gui::IGUIImage*p_blackWin;
	irr::gui::IGUIImage*p_whiteWin;
	//black_win and white_win

	irr::gui::IGUIImage*p_draw;
	//draw

	irr::gui::IGUIImage*p_blackHold;
	irr::gui::IGUIImage*p_whiteHold;
	//black_hold and white hold
};

#endif

GameView::GameView(irr::IrrlichtDevice *device) :
	View(device),
	GameViewSink(this),
	m_camera(NULL),
	m_ready(true),
	m_board(),
	m_column(),
	m_beads(),
	m_animating_bead(NULL),
	m_animation_dest(),
	m_game_mode(),
	m_current_player(),
	m_status(),
	m_is_column_valid(),
	m_last_move(),
	m_undo_count(),
	m_game_state(),
	m_ai_decision(),
	m_game_start_command(),
	m_player_move_command(),
	m_ai_move_command(),
	m_undo_command(),
	m_switch_window_command()

{

}

GameView::~GameView(void) {

}

void GameView::initialize(void) {
	if (!getEnable()) {
		//return;
	}
	irr::IrrlichtDevice *device = getIrrlichtDevice();
	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *smgr = device->getSceneManager();
	const irr::scene::IGeometryCreator *geom = smgr->getGeometryCreator();
	irr::gui::IGUIEnvironment *env = device->getGUIEnvironment();
	irr::gui::IGUISkin* skin = env->getSkin();//使字体更加美观
	irr::gui::IGUIFont* font = env->getFont("../media/fonthaettenschweiler.bmp");
	if (font) {
		skin->setFont(font);
	}
	skin->setFont(env->getBuiltInFont(), irr::gui::EGDF_TOOLTIP);

	m_board = smgr->addMeshSceneNode(geom->createCubeMesh(irr::core::vector3df(100.0F, 5.0F, 100.0F)), NULL, -1, irr::core::vector3df(0.0F, 0.0F, 0.0F), irr::core::vector3df(0.0F, 0.0F, 0.0F));

	irr::video::ITexture* tex = driver->getTexture("../media/board.jpg");
	m_light_texture = driver->getTexture("../media/light.jpg");
	m_dark_texture = driver->getTexture("../media/dark.jpg");
	m_board->setMaterialTexture(0, tex);
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			Position pos(i, j);
			m_column[pos] = smgr->addMeshSceneNode(geom->createCylinderMesh(2.0F, 45.0F, 255, irr::video::SColor(255, 255, 255, 255)), m_board, i * 4 + j + 1, irr::core::vector3df(i * 24.0F - 36.0F, 0.0F, j * 24.0F - 36.0F));
			m_column[pos]->setMaterialTexture(0, tex);
			irr::scene::ISceneNode *top = smgr->addMeshSceneNode(geom->createSphereMesh(2.0F, 255, 255), m_column[pos], i * 4 + j + 1, irr::core::vector3df(0, 45.0F, 0));
			top->setMaterialTexture(0, tex);
		}
	}

	m_next_bead = smgr->addMeshSceneNode(geom->createSphereMesh(5.0F, 255, 255), NULL, m_beads.size() + 17, irr::core::vector3df(0.0F, 40.0F, 0.0F));
	if (m_current_player.Deref() == COLOR::BLACK) {
		m_next_bead->setMaterialTexture(0, m_dark_texture);
	} else {
		m_next_bead->setMaterialTexture(0, m_light_texture);
	}
	m_next_bead->setVisible(false);

	m_camera = smgr->addCameraSceneNodeMaya(0, -1500.0F, 0.0F, 0.0F, -1, 100.0F, true);
	m_camera->setTarget(irr::core::vector3df(0.0F, 30.0F, 0.0F));
	getIrrlichtDevice()->getSceneManager()->setActiveCamera(m_camera);

	smgr->addLightSceneNode(m_camera, irr::core::vector3df(0, 0, 20), irr::video::SColorf(1.0f, 1.0f, 1.0f), 2000);
	smgr->setAmbientLight(irr::video::SColorf(0.8f, 0.8f, 0.8f));

	// Highlight
	m_highlighted_scene_node = NULL;
	m_origin_selected_scene_node = NULL;
	m_collision_manager = smgr->getSceneCollisionManager();

	// GUI
	m_debug_text_1 = env->addStaticText(L"test, test", irr::core::rect<irr::s32>(10, 10, 200, 200), false, true);
	m_debug_text_2 = env->addStaticText(L"test2, test2", irr::core::rect<irr::s32>(10, 100, 200, 200), false, true);
	m_debug_text_1->setVisible(false);
	m_debug_text_2->setVisible(false);

	// action buttons
	irr::gui::IGUIButton * p_restart = env->addButton(irr::core::rect<irr::s32>(650, 10, 790, 10 + 32), 0, GUI_ID_RESTART,
		L"RESTART", L"");
	p_restart->setVisible(true);
	irr::gui::IGUIButton * p_menu = env->addButton(irr::core::rect<irr::s32>(650, 52,790, 52 + 32), 0, GUI_ID_MENU,
		L"MENU", L"");
	p_menu->setVisible(true);

	p_restart->setDrawBorder();


	// Store the appropriate data in a context structure.
	m_context = new SAppContext;
	m_context->device = device;
	m_context->ptrBtn["restart"] = p_restart;
	m_context->ptrBtn["menu"] = p_menu;

	// Mouse events
	m_mouse_receiver = new MouseEventReceiver(*m_context);
	device->setEventReceiver(m_mouse_receiver);

	//black_win
	p_blackWin = env->addImage(driver->getTexture("../media/black_win.jpg"), irr::core::position2d<int>(10, 10));
	p_blackWin->setVisible(false);
	//white_win
	p_whiteWin = env->addImage(driver->getTexture("../media/white_win.jpg"), irr::core::position2d<int>(10, 10));
	p_whiteWin->setVisible(false);
	
	//draw
	p_draw = env->addImage(driver->getTexture("../media/draw.jpg"), irr::core::position2d<int>(10, 10));
	p_draw->setVisible(false);

	//black_hold
	p_blackHold = env->addImage(driver->getTexture("../media/black_hold.jpg"), irr::core::position2d<int>(10, 10));
	
	//white_hold
	p_whiteHold = env->addImage(driver->getTexture("../media/white_hold.jpg"), irr::core::position2d<int>(10, 10));
	

	

	if (m_current_player.Deref() == BLACK){
		p_blackHold->setVisible(true);
		p_whiteHold->setVisible(false);
	}
	else if (m_current_player.Deref() == WHITE ){
		p_blackHold->setVisible(false);
		p_whiteHold->setVisible(true);
	}

	View::initialize();
}

void GameView::drop(void) {
	getIrrlichtDevice()->setEventReceiver(NULL);
	delete m_mouse_receiver;
	delete m_context;
	m_mouse_receiver = NULL;
	m_context = NULL;
	m_column.clear();
	m_beads.clear();
	View::drop();
}

void GameView::update(void) {
	if (!getEnable()) {
		return;
	}

	irr::IrrlichtDevice *device = getIrrlichtDevice();
	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *smgr = device->getSceneManager();
	const irr::scene::IGeometryCreator *geom = smgr->getGeometryCreator();

	if (m_mouse_receiver->getMenu()) {
		m_switch_window_command.Deref().SetParameter(RefPtrHelper::TypeCast<SingleParameter<WINDOW>, 
			ICommandParameter>(RefPtr<SingleParameter<WINDOW>>(&SingleParameter<WINDOW>(WINDOW::TITLE))));
		m_switch_window_command.Deref().Exec();
		return;
	}
	if (m_mouse_receiver->getRestart()) {
		m_game_start_command.Deref().SetParameter(RefPtrHelper::TypeCast<SingleParameter<GameMode>,
			ICommandParameter>(RefPtr<SingleParameter<GameMode>>(&SingleParameter<GameMode>(m_game_mode.Deref()))));
		m_game_start_command.Deref().Exec();
		drop();
		initialize();
		m_debug_text_2->setText(L"Restart Success");
		return;
	}

	MouseState mouse_state = m_mouse_receiver->getMouseState();

	// Mouse events
	bool left_click = m_last_mouse_state.LeftButtonDown & (!mouse_state.LeftButtonDown);
	bool left_down = (!m_last_mouse_state.LeftButtonDown) & mouse_state.LeftButtonDown;
	bool middle_down = (!m_last_mouse_state.MiddleButtonDown) & mouse_state.MiddleButtonDown;
	bool right_down = (!m_last_mouse_state.RightButtonDown) & mouse_state.RightButtonDown;
	m_last_mouse_state = mouse_state;

	if (!m_ready || m_game_state.Deref() != GAME_STATE::PLAYING) {
		// Restricted case

		// animation
		if (m_animating) {
			irr::core::vector3df pos = m_animating_bead->getPosition();
			pos += irr::core::vector3df(0.0F, -2.0F, 0.0F);
			m_animating_bead->setPosition(pos);

			if (pos.Y <= m_animation_dest.Y + 2.0F) {
				m_ready = true;
				m_animating = false;
				m_animating_bead->setPosition(m_animation_dest);
				m_animating_bead = NULL;
				m_animation_dest = irr::core::vector3df();
				if (!m_game_mode.Deref().getPvp() && m_current_player.Deref() == COLOR::WHITE && m_game_state.Deref() == GAME_STATE::PLAYING) {
					m_ready = false;
					View::update();
					m_ai_move_command.Deref().Exec();
					return;
				}
			}
		} 
	} else {
		// Free case

		// Column highlight
		if (m_highlighted_scene_node) {
			// m_highlighted_scene_node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
			m_next_bead->setVisible(false);
			m_highlighted_scene_node = NULL;
		}

		irr::scene::ISceneNode * selected_scene_node = m_collision_manager->getSceneNodeFromScreenCoordinatesBB
			(mouse_state.Position, 255);

		
		wchar_t pos[256];
		auto vec = m_camera->getPosition();
		if (selected_scene_node) {
			swprintf_s(pos, L"%f, %f, %f", vec.X, vec.Y, vec.Z);
			m_debug_text_2->setText(pos);
		} else {
			m_debug_text_2->setText(L"Mouse not found");
		}
		

		m_debug_text_1->setText(L"nothing selected");
		if (left_down) {
			m_origin_selected_scene_node = selected_scene_node;
		}
		if (selected_scene_node && ( !mouse_state.LeftButtonDown || m_origin_selected_scene_node &&
			(m_origin_selected_scene_node->getID() == selected_scene_node->getID()))) {
			wchar_t id[256];
			swprintf_s(id, L"selected %d", selected_scene_node->getID());
			m_debug_text_1->setText(id);
			if (selected_scene_node->getID() > 0) {
				if (selected_scene_node->getID() > 16) {
					m_highlighted_scene_node = selected_scene_node->getParent();
				} else {
					m_highlighted_scene_node = selected_scene_node;
				}
				wchar_t id[256];
				swprintf_s(id, L"highlighted %d", m_highlighted_scene_node->getID());
				m_debug_text_1->setText(id);
				Position pos((m_highlighted_scene_node->getID() - 1) / 4, (m_highlighted_scene_node->getID() - 1) % 4);
				if (m_is_column_valid.Deref()[pos]) {
					if (left_click && m_origin_selected_scene_node &&
						(m_origin_selected_scene_node->getID() == selected_scene_node->getID())) {
						m_origin_selected_scene_node = NULL;
						m_ready = false;
						m_player_move_command.Deref().SetParameter(RefPtrHelper::TypeCast<SingleParameter<Position>, 
							ICommandParameter>(RefPtr<SingleParameter<Position>>(&SingleParameter<Position>(pos))));
						m_player_move_command.Deref().Exec();
						return;
					} else {
						m_next_bead->setVisible(true);
						m_next_bead->setParent(m_column[pos]);
					}
				}
			}
		}
	}
	
	View::update();
}

void GameView::setGameModeRef(const RefPtr<GameMode> &game_mode) {
	m_game_mode = game_mode;
}
void GameView::setCurrentPlayerRef(const RefPtr<COLOR> &current_player) {
	m_current_player = current_player;
}
void GameView::setStatusRef(const RefPtr<std::map<Position, std::vector<COLOR>>> &status) {
	m_status = status;
}
void GameView::setIsColumnValidRef(const RefPtr<std::map<Position, bool>> &is_column_valid) {
	m_is_column_valid = is_column_valid;
}
void GameView::setLastMoveRef(const RefPtr<Position> &last_move) {
	m_last_move = last_move;
}
void GameView::setUndoCountRef(const RefPtr<long> &undo_count) {
	m_undo_count = undo_count;
}
void GameView::setGameStateRef(const RefPtr<GAME_STATE> &game_state) {
	m_game_state = game_state;
}
void GameView::setAiDecisionRef(const RefPtr<Position> &ai_decision) {
	m_ai_decision = ai_decision;
}

void GameView::setGameStartCommand(const RefPtr<ICommandEx> &game_start_command) {
	m_game_start_command = game_start_command;
}
void GameView::setPlayerMoveCommand(const RefPtr<ICommandEx> &player_move_command) {
	m_player_move_command = player_move_command;
}
void GameView::setAiMoveCommand(const RefPtr<ICommandEx> &ai_move_command) {
	m_ai_move_command = ai_move_command;
}
void GameView::setUndoCommand(const RefPtr<ICommandEx> &undo_command) {
	m_undo_command = undo_command;
}
void GameView::setSwitchWindowCommand(const RefPtr<ICommandEx> &switch_window_command) {
	m_switch_window_command = switch_window_command;
}

void GameView::onCurrentPlayerChange(void) {
	COLOR player = m_current_player.Deref();

}

void GameView::onGameStateChange(void) {
	if (!getEnable()) {
		return;
	}
	if (m_game_state.Deref() == GAME_STATE::WIN) {
		m_debug_text_2->setText(L"Game Over");
		if (m_current_player.Deref() == BLACK){
			p_blackWin->setVisible(true);
			p_whiteWin->setVisible(false);
		}
		else if (m_current_player.Deref() == WHITE){
			p_blackWin->setVisible(false);
			p_whiteWin->setVisible(true);
		}
		p_whiteHold->setVisible(false);
		p_blackHold->setVisible(false);


	} 
	else if (m_game_state.Deref() == GAME_STATE::DRAW){
		p_whiteHold->setVisible(false);
		p_blackHold->setVisible(false);
		p_draw->setVisible(true);

	}
	else {
		m_debug_text_2->setText(L"Go on");
	}
}

void GameView::onLastMoveChange(void) {
	if (!getEnable()) {
		return;
	}

	irr::IrrlichtDevice *device = getIrrlichtDevice();
	Position pos = m_last_move.Deref();

	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *smgr = device->getSceneManager();
	const irr::scene::IGeometryCreator *geom = smgr->getGeometryCreator();
	
	m_animating_bead = m_next_bead;
	m_animating_bead->setParent(m_column[pos]);
	m_animating_bead->setVisible(true);
	m_animation_dest = irr::core::vector3df(0, m_beads[pos].size() * 9.165F + 7.5F, 0);
	m_beads[pos].push_back(m_animating_bead);
	m_animating = true;

	m_next_bead = smgr->addMeshSceneNode(geom->createSphereMesh(5.0F, 255, 255), NULL, m_beads.size() + 17, irr::core::vector3df(0.0F, 40.0F, 0.0F));
	if (m_current_player.Deref() == COLOR::BLACK) {
		m_next_bead->setMaterialTexture(0, m_dark_texture);
	} else {
		m_next_bead->setMaterialTexture(0, m_light_texture);
	}
	m_next_bead->setVisible(false);


	if (m_current_player.Deref() == BLACK){
		p_blackHold->setVisible(true);
		p_whiteHold->setVisible(false);
	}
	else if (m_current_player.Deref() == WHITE){
		p_blackHold->setVisible(false);
		p_whiteHold->setVisible(true);
	}
	

}

void GameView::onStatusChange(void) {

}

void GameView::onColumnValidChange(void) {

}

void GameView::onGameStartCommandComplete(const bool success) {

}

void GameView::onPlayerMoveCommandComplete(const bool success) {

}
void GameView::onAiMoveCommandComplete(const bool success) {

}
void GameView::onUndoCommandComplete(const bool success) {

}

const bool GameView::is_ready(void) const {
	return m_ready;
}
