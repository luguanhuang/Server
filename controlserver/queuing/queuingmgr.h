#ifndef __QUEUINGMGR_H__
#define __QUEUINGMGR_H__

#include "cmdline.h"

struct WaitInfo 
{
	std::string accountid;
	UINT32 timestamp;
	UINT32 count;
	WaitInfo()
	{
		timestamp = 0;
		count = 0;
	}
};

struct WaitRes
{
	UINT32 rolecount;
	UINT32 timeleft;

	WaitRes()
	{
		rolecount = 0;
		timeleft = 0;
	}
};

class QueuingMgr : public ITimer
{
	QueuingMgr();
	~QueuingMgr();
	DECLARE_SINGLETON(QueuingMgr);
public:
	bool Init();
	void Uninit();

	bool LoadFile();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool IsFull();

	bool IsWaiting(const std::string& account);
	//bool IsReady(const std::string& account);
	//void PopReady(const std::string& account);

	void PushWait(const std::string& account);
	void PopWait(const std::string& account, UINT32 rolecount);

	const WaitInfo* GetWaitInfo(const std::string& account);
	void GetWaitRes(const WaitInfo* info, WaitRes& res);
	UINT32 GetWaitPreCount(const std::string& accountid);

	UINT32 GetWaittingCount() const { return m_id2wait.size(); }
	static bool CmdQueue(const CmdLineArgs &args, std::string &outputMessage);
private:
	UINT32 m_toqueuingcount;
	UINT32 m_queuingtime;
	UINT32 m_readyexpiretime;

	///> 等待队列
	std::deque<WaitInfo> m_waitingqueue;
	std::map<std::string, WaitInfo> m_id2wait; 
	///> 就绪队列
	//std::map<std::string, WaitInfo> m_id2ready; 
};

#endif