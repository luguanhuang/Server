#ifndef __ROLEPARTCALLBACK_H__
#define __ROLEPARTCALLBACK_H__

#include <queue>

#define PARTROLE_SLICECOUNT 500

typedef std::function<void(UINT64)> FuncRolePartCallBack;

enum ERolePartState
{
	eCopyRoleManager = 0,
	ePartCallBack = 1,
};

struct TimeSpecific;
class CRolePartCallBack
{
public:
	CRolePartCallBack();
	virtual ~CRolePartCallBack();

	bool Init(const TimeSpecific& time, FuncRolePartCallBack funcCallBack);

	void Uninit();
	
	bool CallBack();

private:
	UINT32 m_dwState;
	UINT32 m_dwSliceCount;
	UINT32 m_dwTimeSpecificHandle;
	FuncRolePartCallBack m_funcCallBack;
	std::deque<UINT64> m_queRoleId;
};

#endif