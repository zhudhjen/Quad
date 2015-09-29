#pragma once

#include "common.h"

template <class T>
class GameViewSink : public INotification
{
public:
	explicit GameViewSink(T* view) : m_p(view) {}

	RefPtr<INotification> get_Notification() throw() {
		return RefPtr<INotification>(static_cast<INotification*>(this));
	}

	virtual void OnPropertyChange(const std::string& str) {
		if (str == "CurrentPlayer") {
			m_p->onCurrentPlayerChange();
		}
		if (str == "GameState") {
			m_p->onGameStateChange();
		}
		if (str == "LastMove") {
			m_p->onLastMoveChange();
		}
		if (str == "Status") {
			m_p->onStatusChange();
		}
		if (str == "ColumnValid") {
			m_p->onColumnValidChange();
		}
	}

	virtual void OnCommandComplete(const std::string& str, bool success) {
		if (str == "GameStart") {
			m_p->onGameStartCommandComplete(success);
		}
		if (str == "PlayerMove") {
			m_p->onPlayerMoveCommandComplete(success);
		}
		if (str == "AiMove") {
			m_p->onAiMoveCommandComplete(success);
		}
		if (str == "Undo") {
			m_p->onUndoCommandComplete(success);
		}
	}

private:
	T* m_p;
};
