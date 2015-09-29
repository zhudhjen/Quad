#pragma once

template <class T>
class PlayerMoveCommand :public ICommandEx
{
public:
	PlayerMoveCommand(T* p) throw() :m_pvm(p) {}

	RefPtr<ICommandEx> getPlayerMoveCommandRef() throw() {
		return RefPtrHelper::TypeCast<PlayerMoveCommand<T>, ICommandEx>(RefPtr<PlayerMoveCommand<T>>(this));
	}
	
	virtual void SetParameter(const RefPtr<ICommandParameter>& param) {
		m_param = param;
	}
	
	virtual void Exec() {
		RefPtr<SingleParameter<Position>> p = RefPtrHelper::TypeCast<ICommandParameter, SingleParameter<Position>>(m_param);
		m_pvm->playerMove(p.Deref().get_val());
	}

private:
	T* m_pvm;
	RefPtr<ICommandParameter> m_param;
};
