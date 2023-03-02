#ifndef __PAYRECORD_H__
#define __PAYRECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;

class PayRecord : public RoleSerializeInterface
{
public:

	PayRecord(Role *pRole);
	~PayRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

private:
	Role  *m_pRole;
	Switch m_isModified;
};

#endif // __PAYRECORD_H__