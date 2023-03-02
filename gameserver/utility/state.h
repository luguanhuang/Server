#ifndef __STATE_H__
#define __STATE_H__

template<class T, class E>
class State
{
public:
	State()
	:m_type(-1)
	{

	};
	virtual ~State(){};
public:
	virtual void OnEnter(T* holder){};
	virtual void OnLeave(T* holder){};
	virtual void OnTick(T* holder){};
	virtual const bool OnEvent(T* holder, const E& event)
	{
		return false;
	}

	virtual int GetStateType()
	{
		return m_type;
	}
	virtual void SetStateType(int type)
	{
		m_type = type;
	}
private:
	int m_type;
};

template<class T, class E>
class StateMgr
{
public:
	StateMgr()
	: m_currentstate(NULL)
	, m_prestate(NULL)
	, m_holder(NULL)
	{

	};
	virtual ~StateMgr(){};
public:
	const bool Init(T* holder, State<T,E>* initstate)
	{
		m_holder = holder;
		m_currentstate = initstate;
		return true;
	}

	const void SetCurrentState(State<T,E>* state)
	{
		m_currentstate = state;
	}
	State<T,E>* GetCurrentState() const
	{
		return m_currentstate;
	}

	const void SetPreState(State<T,E>* state)
	{
		m_prestate = state;
	}
	State<T,E>* GetPreState() const
	{
		return m_prestate;
	}

	const bool SetState(State<T,E>* state)
	{
		if (NULL == state)
			return false;

		m_currentstate->OnLeave(m_holder);
		SetPreState(m_currentstate);

		SetCurrentState(state);
		m_currentstate->OnEnter(m_holder);
		return true;
	};

	const bool OnEvent(const E& event) const
	{
		if (NULL != m_currentstate && m_currentstate->OnEvent(m_holder, event))
			return true;
		return false;
	}

	void Update()
	{
		if (NULL != m_currentstate)
		{
			m_currentstate->OnTick(m_holder);
		}
	}
private:
	State<T,E>*  m_currentstate;
	State<T,E>*  m_prestate;
	T* m_holder;
};
#endif