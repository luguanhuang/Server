#ifndef __SWITCH_H__
#define __SWITCH_H__


class Switch
{
public:
	Switch()
	{
		m_flag = false;
	}

	~Switch()
	{

	}

	void Set()
	{
		m_flag = true;
	}

	bool TestAndReset()
	{
		bool ret = m_flag;
		m_flag = false;
		return ret;
	}

private:
	bool m_flag;
};

#endif