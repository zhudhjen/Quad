#pragma once

#include <assert.h>
#include <vector>
#include <string>

enum WINDOW {
	TITLE = 0,
	GAME
};

enum COLOR {
	BLACK = 0,
	WHITE
};
enum LEVEL {
	EASY = 0,
	NORMAL,
	HARD
};

enum GAME_STATE {
	PLAYING = 0,
	WIN,
	DRAW
};

class Position
{
public:
	explicit Position(int x=0, int y=0) throw() : m_px(x), m_py(y) {}

	Position(const Position& src) throw() : m_px(src.m_px), m_py(src.m_py) {}

	~Position() throw() {}

	int getX() const throw() {
		return m_px;
	}

	void setX(const int x) {
		m_px = x;
	}

	int getY() const throw() {
		return m_py;
	}

	void setY(const int y) {
		m_py = y;
	}

	operator int() const {
		return m_px * 4 + m_py;
	}

private:
	int m_px, m_py;
};

class GameMode
{
public:
	explicit GameMode(bool pvp=false, LEVEL level=EASY) throw() : m_ppvp(pvp), m_plevel(level) {}

	GameMode(const GameMode& src) throw() : m_ppvp(src.m_ppvp), m_plevel(src.m_plevel) {}

	~GameMode() throw() {}

	bool getPvp() const throw() {
		return m_ppvp;
	}

	void setPvp(const bool pvp) {
		m_ppvp = pvp;
	}

	LEVEL getLevel() const throw() {
		return m_plevel;
	}

	void setLevel(const LEVEL level) {
		m_plevel = level;
	}

private:
	bool m_ppvp;
	LEVEL m_plevel;
};

template <typename T>
class RefPtr
{
public:
	explicit RefPtr(T* p = NULL) throw() : m_p(p)
	{
	}

	RefPtr(const RefPtr<T>& src) throw() : m_p(src.m_p)
	{
	}

	RefPtr(RefPtr&& src) throw()
	{
		m_p = src.m_p;
		src.m_p = NULL;
	}

	~RefPtr() throw()
	{
	}

	void Release() throw()
	{
		m_p = NULL;
	}

	RefPtr<T>& operator=(const RefPtr<T>& src) throw()
	{
		if (this != &src) {
			m_p = src.m_p;
		}
		return *this;
	}
	RefPtr<T>& operator=(RefPtr<T>&& src) throw()
	{
		if (this != &src) {
			m_p = src.m_p;
			src.m_p = NULL;
		}
		return *this;
	}

	const T& Deref() const throw()
	{
		assert(m_p != NULL);
		return *m_p;
	}
	T& Deref() throw()
	{
		assert(m_p != NULL);
		return *m_p;
	}

private:
	T* m_p;
};

class RefPtrHelper
{
public:
	template <class T, class TBase>
	static RefPtr<TBase> TypeCast(const RefPtr<T>& t) throw()
	{
		return RefPtr<TBase>((TBase*)static_cast<const TBase*>(&(t.Deref())));
	}
};

template <class T>
class NotificationImpl
{
public:
	std::vector<RefPtr<T>>& GetNotificationArray() throw() {
		return m_array;
	}
	void AddNotification(const RefPtr<T>& p)
	{
		m_array.push_back(p);
	}

private:
	std::vector<RefPtr<T>> m_array;
};

class INotification
{
public:
	virtual void OnPropertyChange(const std::string& str) = 0;
	virtual void OnCommandComplete(const std::string& str, bool bOK) = 0;
};

template <class T>
class Proxy_Notification : public NotificationImpl<INotification>
{
public:
	void Fire_OnPropertyChange(const std::string& str)
	{
		std::vector<RefPtr<INotification>>& vec = GetNotificationArray();
		auto iter(vec.begin());
		for (; iter != vec.end(); ++iter) {
			(*iter).Deref().OnPropertyChange(str);
		}
	}
	void Fire_OnCommandComplete(const std::string& str, bool bOK)
	{
		std::vector<RefPtr<INotification>>& vec = GetNotificationArray();
		auto iter(vec.begin());
		for (; iter != vec.end(); ++iter) {
			(*iter).Deref().OnCommandComplete(str, bOK);
		}
	}
};

class ICommandParameter
{
};

template <class T>
class SingleParameter : public ICommandParameter
{
public:
	explicit SingleParameter(const T t) throw() : m_val(t) {}

	void set_val(const T t)
	{
		m_val = t;
	}
	const T get_val() const throw()
	{
		return m_val;
	}

private:
	T m_val;
};

class ICommandEx
{
public:
	virtual void SetParameter(const RefPtr<ICommandParameter>& param) = 0;
	virtual void Exec() = 0;
};
