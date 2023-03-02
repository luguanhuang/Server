#ifndef __MULTACTIVITYMGR_H__
#define __MULTACTIVITYMGR_H__

#include "table/MultiActivityList.h"
#include "table/MultActivityMgrBase.h"

// ��Ҫ�ӿ���ȥMultActivityMgrBase��ѯ�� ��ӵ�ʱ�����MS��GS��ͨ�þͼӵ�MultActivityMgrBase��
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
	INT32 GetWeekCount(UINT32 id);//���ܿ����Ĵ���;
};

#endif