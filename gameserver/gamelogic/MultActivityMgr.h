#ifndef __MULTACTIVITYMGR_H__
#define __MULTACTIVITYMGR_H__

#include "table/MultiActivityList.h"
#include "table/MultActivityMgrBase.h"

class Role;
namespace KKSG
{
	class MulActivityRes; 
	class MulActivitInfo;
}

// ��Ҫ�ӿ���ȥMultActivityMgrBase��ѯ�� ��ӵ�ʱ�����MS��GS��ͨ�þͼӵ�MultActivityMgrBase��
class MultActivityMgr : public MultActivityMgrBase
{
	MultActivityMgr();
	virtual ~MultActivityMgr();
	DECLARE_SINGLETON(MultActivityMgr);

public:
	virtual bool Init();
	virtual void Uninit();

	virtual void OnActivityStateChange(UINT32 id, KKSG::MulActivityTimeState lastState, KKSG::MulActivityTimeState curState);
	virtual void OnActivitysChange(UINT32 opencount, std::vector<UINT32>& changeIds);

	bool CanJoinMulitActivity(Role* pRole, int maid);
	int GetJoinCount(Role* pRole, int maid);
	void FillAllMulActivityInfo(Role* pRole, int amid, KKSG::MulActivityRes &roRes, const KKSG::GetMulActOpenInfoRes* res = NULL);
	void GmTestCalcWeek();
	void FillOneMulActivityInfo(Role* pRole, int confid, KKSG::MulActivitInfo* info, const KKSG::GetMulActOpenInfoRes* res);
};

#endif