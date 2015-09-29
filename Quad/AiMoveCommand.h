#pragma once

template <class T>
class AiMoveCommand :public ICommandEx
{
public:
	AiMoveCommand(T* p) throw() :m_pvm(p) {}

	virtual void SetParameter(const RefPtr<ICommandParameter>& param) {}

	RefPtr<ICommandEx> getAiMoveCommandRef() throw() {
		return RefPtrHelper::TypeCast<AiMoveCommand<T>, ICommandEx>(RefPtr<AiMoveCommand<T>>(this));
	}

	virtual void Exec() {
		m_pvm->aiMove();
	}

private:
	T* m_pvm;
};
