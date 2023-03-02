#ifndef __SERVERCROSSDATAMGR_H__
#define __SERVERCROSSDATAMGR_H__

#include "util/utilsingleton.h"
#include "cmdline.h"
#include "serverinfo/servercrossdatamgrbase.h"

class ServerCrossDataMgr : public ServerCrossDataMgrBase, public Singleton<ServerCrossDataMgr> 
{
public:
	bool Init();
	void Uninit();

	virtual KKSG::ServerCrossData* GetData(UINT32 serverID);
	KKSG::ServerCrossData* GetDataByRoleID(UINT64 roleID);

    UINT32 GetSealTypeByRoleID(UINT64 roleID);
	bool IsIdipClosed(UINT64 roleID, UINT32 sysID);
};

#endif
