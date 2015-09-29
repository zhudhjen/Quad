#pragma once

#include "GameViewModel.h"
#include "SwitchWindowCommand.h"
#include <irrlicht.h>

template <class TTitleView, class TGameView>
class GameApp : SwitchWindowCommand<GameApp<TTitleView, TGameView>> {
public:
	GameApp(TTitleView* t, TGameView* g, irr::IrrlichtDevice* device) throw() : 
		m_title_view(t), m_game_view(g), m_device(device), SwitchWindowCommand<GameApp<TTitleView, TGameView>>(this) {}

	void Initialize() {
		// load
		m_vmdl.setGameModelRef(RefPtr<GameModel>(&m_mdl));

		// bind properties
		m_game_view->setCurrentPlayerRef(m_vmdl.getCurrentPlayerRef());
		m_game_view->setGameStateRef(m_vmdl.getGameStateRef());
		m_game_view->setGameModeRef(m_vmdl.getGameModeRef());
		m_game_view->setLastMoveRef(m_vmdl.getLastMoveRef());
		m_game_view->setStatusRef(m_vmdl.getStatusRef());
		m_game_view->setIsColumnValidRef(m_vmdl.getIsColumnValidRef());

		// notification
		m_vmdl.AddNotification(m_title_view->get_Notification());
		m_vmdl.AddNotification(m_game_view->get_Notification());

		// command complete
		m_title_view->setGameStartCommand(m_vmdl.getGameStartCommandRef());
		m_title_view->setSwitchWindowCommand(getSwitchWindowCommandRef());

		m_game_view->setGameStartCommand(m_vmdl.getGameStartCommandRef());
		m_game_view->setPlayerMoveCommand(m_vmdl.getPlayerMoveCommandRef());
		m_game_view->setAiMoveCommand(m_vmdl.getAiMoveCommandRef());
		m_game_view->setUndoCommand(m_vmdl.getUndoCommandRef());
		m_game_view->setSwitchWindowCommand(getSwitchWindowCommandRef());
	}

	void switchWindow(WINDOW window) const throw() {
		if (window == TITLE) {
			m_game_view->drop();
			m_title_view->initialize();
		}
		else if (window == GAME) {
			m_title_view->drop();
			m_game_view->initialize();
		}
	}

	void run() {
		while (m_device->run()) {
			m_title_view->update();
			m_game_view->update();
		}
	}

	void clean() {
		m_device->drop();
	}

private:
	TTitleView* m_title_view;
	TGameView* m_game_view;
	GameModel m_mdl;
	GameViewModel m_vmdl;
	irr::IrrlichtDevice* m_device;
};
