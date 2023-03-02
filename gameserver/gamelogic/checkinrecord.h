#ifndef __CHECKINRECORD_H__
#define __CHECKINRECORD_H__

#include "unit/roleserilizeinterface.h"

class CCheckinRecord : public RoleSerializeInterface
{
public:
	CCheckinRecord();
	void Reset();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	
	void Update();

	UINT32 GetCheckInfo();
	void SetCheckinInfo(UINT32 dwCheckinDay, bool IsCheckLast = false);

	UINT32 CheckinDays();

	UINT32 GetCheckinCount();

	UINT32 GetFirstUnCheckinDay(UINT32 dwStartDay);

	bool IsDayChecked(UINT32 dwCheckinDay);

private:
	Switch m_bIsModify;
	UINT32 m_dwCheckinInfo;
	UINT32 m_dwCheckinTime;
	UINT32 m_dwCheckinCount;
};

#endif