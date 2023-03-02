#ifndef _H_TLogMgr_H__
#define _H_TLogMgr_H__

#include "timer.h"
#include "unit/role.h"

//������ˮ��������¼��־��
class TTag
{
public:
	TTag();
	~TTag();
	DECLARE_SINGLETON(TTag);

public:
	bool Init();
	void Uninit();
	std::string GetTagString();

private:
	INT32 m_iUniqueSeq;
};

class TLogMgr
{
	TLogMgr();
	~TLogMgr();
	DECLARE_SINGLETON(TLogMgr);
public:
	bool Init();
	void Uninit();

	void Logout(Role* pRole);
	void Test();
private:
	class TStateLogTimer : public ITimer
	{
	public:
		TStateLogTimer() {}
		virtual ~TStateLogTimer() {}

		virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	};

	HTIMER			m_hLogTimer;
	TStateLogTimer	m_oLogTimer;
};

#endif