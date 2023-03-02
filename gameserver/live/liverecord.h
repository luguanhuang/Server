#ifndef __LIVERECORD_H__
#define __LIVERECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;
class LiveInfo;
namespace KKSG
{
	class OneLiveRecordInfo;
	class GetMyWatchRecordRes;
}
class CLiveRecord : public RoleSerializeInterface
{
public:
	CLiveRecord(Role* role);
	virtual ~CLiveRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Change();

	void Update();

	void DebugReset();

	void AddTotalView(int num){m_totalView += num; m_switch.Set();}
	void AddTotalCommend(int num){m_totalCommend += num; m_switch.Set();}

	void UpdateRecord(LiveInfo* info);

	UINT32 GetMyRecord2Client(KKSG::GetMyWatchRecordRes& roRes);

	UINT32 ChangeVisible(bool visible);
	bool IsVisible(){return m_liveVisibleSetting;}

    void GetMaxCommendViewNum(UINT32& maxCommadNum, UINT32& maxWatchNum);
private:
	Role* m_pRole;
	Switch m_switch;

	bool m_liveVisibleSetting;
	std::list<KKSG::OneLiveRecordInfo*> m_recentLives;
	KKSG::OneLiveRecordInfo* m_mostCommendLive;
	KKSG::OneLiveRecordInfo* m_mostViewLive;
	UINT32 m_totalView;
	UINT32 m_totalCommend;
};

#endif