#ifndef _SHM_MS_MGR_H_
#define _SHM_MS_MGR_H_

#include "util/utilsingleton.h"
#include "sharememory/shmmap.h"
#include "chat/fmmgr.h"

class ShmMsMgr : public Singleton<ShmMsMgr>
{
public:
	bool Init();
	void Uninit();

	ShareMemoryMap<CShmFMRole>& GetFmRoleShm() { return m_shmFmRole; }

private:
	ShareMemoryMap<CShmFMRole> m_shmFmRole;
};

#endif
