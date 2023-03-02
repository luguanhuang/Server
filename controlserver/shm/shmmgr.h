#ifndef _SHM_MGR_H_
#define _SHM_MGR_H_

#include "util/utilsingleton.h"
#include "sharememory/shmmap.h"
#include "scene/scene.h"
#include "role/role.h"
#include "account/accountsession.h"

class ShmMgr : public Singleton<ShmMgr>
{
public:
	bool Init();
	void Uninit();

	ShareMemoryMap<CShmScene>& GetSceneShm() { return m_shmScene; }
	ShareMemoryMap<CShmRole>& GetRoleShm() { return m_shmRole; }
	ShareMemoryMap<CShmAccountSession>& GetAccountShm() { return m_shmAccount; }

private:
	ShareMemoryMap<CShmScene> m_shmScene;
	ShareMemoryMap<CShmRole> m_shmRole;
	ShareMemoryMap<CShmAccountSession> m_shmAccount;
};

#endif
