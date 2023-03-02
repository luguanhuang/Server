#ifndef __ROLENETHANDLER_H__
#define __ROLENETHANDLER_H__

#include "role/ptcg2c_levelchangentf.h"

class Role;
class Scene;

struct RoleAttrChangeNotify
{
	Role* role;
	UINT32 lastlevel;
	PtcG2C_LevelChangeNtf ntf;

	RoleAttrChangeNotify(Role* temprole);
	void Send();
};

class RoleNetSender
{
public:
	RoleNetSender(Role *pRole);
	~RoleNetSender();

	void SendExpAndLevelNtf();
	void SendLeaveScene();
	void SendEnterScene(Scene *pScene, bool isforce = false);
	void SendAchivementNtf(int aid, int state);
	void SendRoleDataToDB(UINT32 dwParam = 0, bool bSaveAll = false);

private:
	Role *m_pRole;
};

#endif // __ROLENETHANDLER_H__