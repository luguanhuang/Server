#ifndef _SHM_KEY_MGR_H_
#define _SHM_KEY_MGR_H_

enum ShmType
{
	ShmType_NS_SceneManager = 1,
	ShmType_NS_RoleManager	= 2,
	ShmType_NS_Account		= 3,
	ShmType_MS_FmRole		= 4,
};

UINT32 GetShmKey(ShmType type, UINT32 offset = 0);

#endif
