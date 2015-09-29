#pragma once

template <class T>
class UndoCommand :public ICommandEx
{
public:
	UndoCommand(T* p) throw() :m_pvm(p) {}

	virtual void SetParameter(const RefPtr<ICommandParameter>& param) {}

	RefPtr<ICommandEx> getUndoCommandRef() throw() {
		return RefPtrHelper::TypeCast<UndoCommand<T>, ICommandEx>(RefPtr<UndoCommand<T>>(this));
	}

	virtual void Exec() {
		m_pvm->undo();
	}

private:
	T* m_pvm;
};
