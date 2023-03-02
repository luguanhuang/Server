#include "pch.h"
#include "custombattlerolemgr.h"
#include "util.h"
#include "timeutil.h"
#include "db/mysqlmgr.h"
#include "network/dblink.h"
#include "table/globalconfig.h"
#include "custombattle/ptcm2d_updatecustombattleroleondb.h"

#define CUSTOMBATTLE_ROLEUPDATE (GetGlobalConfig().CustomBattleMsRoleUpdate)
#define CUSTOMBATTLE_ROLEUPDATE_LIMIT (GetGlobalConfig().CustomBattleMsRoleUpdateLimit)

INSTANCE_SINGLETON(CustomBattleRoleMgr);

CustomBattleRoleMgr::CustomBattleRoleMgr()
{

}

CustomBattleRoleMgr::~CustomBattleRoleMgr()
{

}

bool CustomBattleRoleMgr::Init()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select _id, system, custom from custombattlerole";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table custombattlerole failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// roleid
		UINT64 roleid;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			roleid = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		// system
		std::string system;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			system.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'system' column failed");
			return false;
		}

		// custom 
		std::string custom;
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			custom.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'custom' column failed");
			return false;
		}

		KKSG::CustombattleRoleSystem tempsystem;
		if(!tempsystem.ParseFromString(system))
		{
			LogError("Parse system failed, roleid:%llu", roleid);
			return false;
		}
		KKSG::CustomBattleRole tempcustom;
		if(!tempcustom.ParseFromString(custom))
		{
			LogError("Parse custom failed, roleid:%llu", roleid);
			return false;
		}
		
		SCustomBattleRole* roledata = new SCustomBattleRole(roleid);
		roledata->ToThis(tempsystem, tempcustom);
		AddInfo(roleid, roledata);
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all custombattlerole data from db use time: %llu ms", qwTimeUsed);

	m_timehandle = CTimerMgr::Instance()->SetTimer(this ,0, CUSTOMBATTLE_ROLEUPDATE, -1, __FILE__, __LINE__);

	return true;
}

void CustomBattleRoleMgr::Uninit()
{
	for (auto i = m_roleid2info.begin(); i != m_roleid2info.end(); ++i)
	{
		if (NULL != i->second)
		{
			delete i->second;
			i->second = NULL;
		}
	}
}

void CustomBattleRoleMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 countlimit = m_isend ? (UINT32)(-1) : CUSTOMBATTLE_ROLEUPDATE_LIMIT;
	UINT32 limit = 0;
	for (auto i = m_roleid2info.begin(); i != m_roleid2info.end(); ++i)
	{
		if (i->second->IsChanged())
		{
			if (++limit > countlimit)
			{
				break;
			}

			// save to db...
			PtcM2D_UpdateCustomBattleRoleOnDB ntf;
			UINT64 roleid = i->first;
			ntf.m_Data.set_roleid(roleid);
			i->second->ToKKSG(*ntf.m_Data.mutable_system(), *ntf.m_Data.mutable_custom());
			DBLink::Instance()->SendTo(ntf);

			i->second->ResetChanged();
		}
	}
}

SCustomBattleRole* CustomBattleRoleMgr::GetInfo(UINT64 roleid)
{
	auto i = m_roleid2info.find(roleid);
	if (i == m_roleid2info.end())
	{
		SCustomBattleRole* info = new SCustomBattleRole(roleid);
		m_roleid2info[roleid] = info;
		return info;
	}
	else
	{
		return i->second;
	}
}

SCustomBattleRole* CustomBattleRoleMgr::TryGetInfo(UINT64 roleid)
{
	auto i = m_roleid2info.find(roleid);
	if (i == m_roleid2info.end())
	{
		return NULL;
	}
	else
	{
		return i->second;
	}
}


void CustomBattleRoleMgr::AddInfo(UINT64 roleid, SCustomBattleRole* info)
{
	auto i = m_roleid2info.find(roleid);
	if (i != m_roleid2info.end())
	{
		delete info;
		SSWarn<<"roleid:"<<roleid<<" is exist"<<END;
	}
	else
	{
		m_roleid2info[roleid] = info;
	}
}
