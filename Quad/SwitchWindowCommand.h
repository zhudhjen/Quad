#pragma once

template <class T>
class SwitchWindowCommand :public ICommandEx
{
public:
	SwitchWindowCommand(T* p) throw() :m_app(p) {}

	RefPtr<ICommandEx> getSwitchWindowCommandRef() throw() {
		return RefPtrHelper::TypeCast<SwitchWindowCommand<T>, ICommandEx>(RefPtr<SwitchWindowCommand<T>>(this));
	}

	virtual void SetParameter(const RefPtr<ICommandParameter>& param) {
		m_param = param;
	}

	virtual void Exec() {
		RefPtr<SingleParameter<WINDOW>> p = RefPtrHelper::TypeCast<ICommandParameter, SingleParameter<WINDOW>>(m_param);
		m_app->switchWindow(p.Deref().get_val());
	}

private:
	T* m_app;
	RefPtr<ICommandParameter> m_param;
};
