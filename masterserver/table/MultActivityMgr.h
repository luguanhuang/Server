#ifndef __MULTACTIVITYMGR_H__
#define __MULTACTIVITYMGR_H__

#include "table/MultiActivityList.h"
#include "table/MultActivityMgrBase.h"

// 需要接口先去MultActivityMgrBase查询， 添加的时候如果MS和GS能通用就加到MultActivityMgrBase上
class MultActivityMgr : public MultActivityMgrBase
{
	MultActivityMgr();
	~MultActivityMgr();
	DECLARE_SINGLETON(MultActivityMgr);
public:
	virtual bool Init();
	virtual void Uninit();

	virtual void OnActivityStateChange(UINT32 id, KKSG::MulActivityTimeState lastState, KKSG::MulActivityTimeState curState);
	void GetGuildTask(std::vector<MultiActivityList::RowData* >& vCfg);
	INT32 GetWeekCount(UINT32 id);//本周开启的次数;
};

#endif