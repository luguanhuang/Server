#ifndef __COMPETEDRAGONRECORD_H__
#define __COMPETEDRAGONRECORD_H__

#include "unit/roleserilizeinterface.h"

namespace KKSG
{
	class GetCompeteDragonInfoRes;
}

class Role;

class CCompeteDragonRecord : public RoleSerializeInterface
{
public:
	CCompeteDragonRecord(Role* role);
	virtual ~CCompeteDragonRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Change();

	void Update();

	void Reset(int time);
	void DebugReset();
	void DebugSetThrough(UINT32 count);

	bool CheckRoleCondition(int expID);

	void AddThroughCount();

	UINT32 GetReward();

	UINT32 FillCompeteDragonInfo2Client(KKSG::GetCompeteDragonInfoRes& roRes);

	void HintNotify();

	void OnLogin();
private:
	Role* m_pRole;
	Switch m_switch;

	int m_updateDay;
	UINT32 m_throughCount;
	UINT32 m_getRewardCount;
};

#endif
