#pragma once

#include "common.h"

template <class T>
class TitleViewSink : public INotification
{
public:
	explicit TitleViewSink(T* view) : m_p(view) {}

	RefPtr<INotification> get_Notification() throw() {
		return RefPtr<INotification>(static_cast<INotification*>(this));
	}

	virtual void OnPropertyChange(const std::string& str) {
		// Title view has no property needed
	}

	virtual void OnCommandComplete(const std::string& str, bool success) {
		if (str == "GameStart") {
			m_p->onGameStartCommandComplete(success);
		}
	}

private:
	T* m_p;
};
