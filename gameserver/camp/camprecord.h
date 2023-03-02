#ifndef __CAMPRECORD_H__
#define __CAMPRECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;
class CCampTask;

class CCampRecord : public RoleSerializeInterface
{
	friend class CCampTask;
public:
	CCampRecord(Role* role);
	virtual ~CCampRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Reset();
	inline void SetLastCampID(UINT32 campID){m_lastCampID = campID;}

private:
	Role* m_pRole;
	Switch m_switch;
	UINT32 m_lastCampID;
};

#endif