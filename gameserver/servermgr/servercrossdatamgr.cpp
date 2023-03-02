#include "pch.h"
#include "servercrossdatamgr.h"
#include "config.h"
#include "servermgr.h"
#include "idutil.h"

static bool ScdCmdHandler(const CmdLineArgs &args, std::string &outputMessage)
{
	ServerCrossDataMgr::Instance()->InfoPrint();
	return true;
}

bool ServerCrossDataMgr::Init()
{
	CmdLine::RegistCmd("scd", &ScdCmdHandler);

	return true;
}

void ServerCrossDataMgr::Uninit()
{

}

KKSG::ServerCrossData* ServerCrossDataMgr::GetData(UINT32 serverID)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		SSError << " normal gs have not cross data serverid = " << serverID << END;
		return NULL;
	}

	//¼æÈÝºÏ·þ
	return ServerCrossDataMgrBase::GetData(ServerMgr::Instance()->GetRealServerId(serverID));	
}

KKSG::ServerCrossData* ServerCrossDataMgr::GetDataByRoleID(UINT64 roleID)
{
	return GetData(GetServerIDByRoleID(roleID));
}

UINT32 ServerCrossDataMgr::GetSealTypeByRoleID(UINT64 roleID)
{
    KKSG::ServerCrossData* pData = GetDataByRoleID(roleID);
    if(NULL == pData)
    {
        SSError << " get server cross data failed roleid = " << roleID << END;
        return 0;
    }
    return pData->sealtype();
}

bool ServerCrossDataMgr::IsIdipClosed(UINT64 roleID, UINT32 sysID)
{
    KKSG::ServerCrossData* pData = GetDataByRoleID(roleID);
    if(NULL == pData)
    {
        SSError << " get server cross data failed roleid = " << roleID << END;
		return false;
    }
	for(int i = 0; i < pData->idipclosesys_size(); ++i)
	{
		if(sysID == pData->idipclosesys(i))
		{
			return true;
		}
	}
	return false;
}
