#pragma once

template <class T>
class GameStartCommand :public ICommandEx
{
public:
	GameStartCommand(T* p) throw() :m_pvm(p) {}

	RefPtr<ICommandEx> getGameStartCommandRef() throw() {
		return RefPtrHelper::TypeCast<GameStartCommand<T>, ICommandEx>(RefPtr<GameStartCommand<T>>(this));
	}

	virtual void SetParameter(const RefPtr<ICommandParameter>& param) {
		m_param = param;
	}

	virtual void Exec() {
		RefPtr<SingleParameter<GameMode>> p = RefPtrHelper::TypeCast<ICommandParameter, SingleParameter<GameMode>>(m_param);
		m_pvm->gameStart(p.Deref().get_val());
	}

private:
	T* m_pvm;
	RefPtr<ICommandParameter> m_param;
};
