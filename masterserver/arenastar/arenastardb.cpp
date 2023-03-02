#include "pch.h"
#include "arenastardb.h"
#include "arenastardata.h"
#include "arenastarmgr.h"
#include "arenastar/rpcm2d_arenastarroledatasave.h"
#include "network/dblink.h"
#include "db/mysqlmgr.h"
#include "util.h"

bool ArenaStarDbH::Read()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from arenastarroledata";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table arenastarroledata failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;

		UINT64 roleID = 0;
		std::string data;

		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			roleID = convert<UINT64>(pVal);
		}
		else
		{
			SSError << " read roleid failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			data.assign(pVal, dwLen);
		}
		else
		{
			SSError << " read data failed " << END;
			return false;
		}

		KKSG::ArenaStarRoleData dbData;
		if(!dbData.ParseFromString(data))
		{
			SSError << " parse dbdata err roleid = " << roleID << END;
			return false;
		}

		if(dbData.roleid() != roleID)
		{
			SSError << " roleid err roleid = " << roleID << " roleid = " << dbData.roleid() << END;
			return false;
		}

		ArenaStarMgr::Instance()->AddDataFromDB(dbData);
	}

	return true;
}

void ArenaStarDbH::Update(UINT32 wirteNumLlimit)
{
	if(WaitSize() <= 0)
	{
		return;
	}

	RpcM2D_ArenaStarRoleDataSave* rpc = RpcM2D_ArenaStarRoleDataSave::CreateRpc();

	int sendNum = wirteNumLlimit;
	for(auto it = m_waitRoles.begin(); it != m_waitRoles.end();)
	{
		UINT64 roleID = (*it);	
		m_waitRoles.erase(it++);
		ASRoleData* pRoleData = ArenaStarMgr::Instance()->GetRoleData(roleID);
		if(pRoleData)
		{
			*rpc->m_oArg.add_roledata() = pRoleData->Data(); 
		}

		if((--sendNum) <= 0)
		{
			break;
		}
	}

	SSInfo << " send count = " << rpc->m_oArg.roledata_size() << END;
	DBLink::Instance()->SendTo(*rpc);
}

void ArenaStarDbH::AddChangeRole(UINT64 roleID)
{
	m_waitRoles.insert(roleID);
}
