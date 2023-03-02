#ifndef __TITLERECORD_H__
#define __TITLERECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;
class CombatAttribute;
class TitleRecord : public RoleSerializeInterface
{
public:
	TitleRecord(Role* pRole);
	~TitleRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	UINT32 GetTitle();

	KKSG::ErrorCode TitleLevelUp();	

	void ApplyEffect(CombatAttribute* combatAttr); 

	void DebugSetTitle(UINT32 title);
private:
	void TitleLevelUpNotice();

private:
	Role* m_pRole;
	UINT32 m_titleID;
	Switch m_isModify;
};

#endif