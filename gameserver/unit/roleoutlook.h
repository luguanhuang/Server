#ifndef __ROLEOUTLOOK_H__
#define __ROLEOUTLOOK_H__

#include "pb/project.pb.h"
#include "gamelogic/rolesummarymgr.h"
#include "common/ptcg2c_outlookchangentf.h"

class Role;
class RoleOutLook
{
public:
	RoleOutLook(Role* role);
	~RoleOutLook();

	void Fill(UINT32 filed, void* param);
	void BroadCast();
	void SendToSelf();

	static bool FillEquip(KKSG::OutLook* out, Role* role, void* param);
	static bool FillDesignation(KKSG::OutLook* out, Role* role);
	static bool FillGuild(KKSG::OutLook* out, Role* role);
	static bool FillTitle(KKSG::OutLook* out, Role* role);
	static bool FillOp(KKSG::OutLook* out, Role* role);
	static bool FillSprite(KKSG::OutLook* out, Role* role);
	static bool FillState(KKSG::OutLook* out, Role* role);
	static bool FillMilitaryRank(KKSG::OutLook* out, Role* role);
	static bool FillDisplayFashion(KKSG::OutLook* out, Role* role);
	static bool FillWeddingDisplayFash(KKSG::OutLook* out, Role* role);
	static bool FillPayScore(KKSG::OutLook* out, Role* role);

	void FillDisplayFashion();
	void FillWeddingDisplayFash();

private:
	bool m_ischanged;
	Role* m_role;
	PtcG2C_OutLookChangeNtf m_ntf;
	int m_field;
};

#endif