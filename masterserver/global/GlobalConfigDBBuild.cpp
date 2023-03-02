#include "pch.h"
#include "GlobalConfigDBBuild.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "common/ptcm2d_msupdateglobalconfig.h"
#include "network/dblink.h"

INSTANCE_SINGLETON(CGlobalConfigDBBuild);

CGlobalConfigDBBuild::CGlobalConfigDBBuild()
{

}

CGlobalConfigDBBuild::~CGlobalConfigDBBuild()
{

}

bool CGlobalConfigDBBuild::Init()
{
	if (!m_oTable.LoadFile("serveronly/GlobalConfigToDb.txt"))
	{
		SSWarn<<"LoadFile serveronly/GlobalConfigToDb.txt failed"<<END;
		return false;
	}
	for (auto iter = m_oTable.Table.begin(); iter != m_oTable.Table.end(); ++iter)
	{
		GlobalToDbTable::RowData *pData= *iter;
		m_mapGlobalConfig[pData->index] = pData->value;
	}
	if(!ReadDataFromDB()) return false;

	// dump eDragonGlobal
	std::string value = GetConfigValue(eDragonGlobal);
	KKSG::GlobalDragonInfo info;
	info.ParseFromString(value);

	bool fillDragonWeak = false;
	for (int i = 0; i < info.sceneid_size(); ++i)
	{
	//	SSInfo<<"dragon sceneid:"<<info.sceneid(i)<<" timestamp:"<<info.timestamp(i)<<END;
		bool find = false;
		for (int j = 0; j < info.weakinfos_size(); ++j)
		{
			if (info.sceneid(i) == info.weakinfos(j).sceneid())
			{
				find = true;
				break;
			}
		}
		if (!find)
		{
			KKSG::GlobalDragonWeakInfo* weakInfo = info.add_weakinfos();
			weakInfo->set_sceneid(info.sceneid(i));
			if (i < info.timestamp_size())
			{
				weakInfo->set_opentime(info.timestamp(i));
			}
			fillDragonWeak = true;
		}
	}
	if (fillDragonWeak)
	{
		SetConfigValue(eDragonGlobal, info.SerializeAsString());
	}

	//dump loginactivity
	value = GetConfigValue(eLoginActivity);
	KKSG::GlobalLoginActivityInfo actinfo;
	actinfo.ParseFromString(value);
	SSInfo<<"loginactivity worldlevel " << actinfo.worldlv()<<END;
	return true;
}
void CGlobalConfigDBBuild::Uninit()
{
}
std::string CGlobalConfigDBBuild::GetConfigValue(UINT32 dwId)
{
	if (eConfigDummy <= (EGlobalConfigToDb)dwId)
	{
		return INVALID_CONFIGVALUE;
	}
	return m_mapGlobalConfig[dwId];
}

void CGlobalConfigDBBuild::SetConfigValue(UINT32 dwId, const std::string& strData)
{
	if (eConfigDummy <= (EGlobalConfigToDb)dwId)
	{
		SSWarn<<"set config value error, type:"<<dwId<<" value:"<<strData.c_str()<<END;
		return;
	}
	m_mapGlobalConfig[dwId] = strData;
}
bool CGlobalConfigDBBuild::ReadDataFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	std::stringstream ss;
	ss<< "select _id,data from globalconfig";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table mail failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT32 ID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			ID = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'data' column failed");
			return false;
		}
		SetConfigValue(ID, strData);
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all globalConfig data from db use time: %llu ms", qwTimeUsed);
	return true;
}

bool CGlobalConfigDBBuild::UpdateToDb(EGlobalConfigToDb eConfig)
{
	std::string strData = GetConfigValue(eConfig);
	if (INVALID_CONFIGVALUE == strData)
	{
		SSWarn<<"update global value failed, eConfig:"<<eConfig<<END;
		return false;
	}
	
	PtcM2D_MsUpdateGlobalConfig stPtc;
	stPtc.m_Data.set_configid(eConfig);
	stPtc.m_Data.set_configvalue(strData);
	return DBLink::Instance()->SendTo(stPtc);
}


bool CGlobalConfigDBBuild::SaveData(EGlobalConfigToDb eConfig,const std::string &refData)
{
	if (eConfigDummy <= (EGlobalConfigToDb)eConfig)
	{
		SSWarn<<"save config value error, type:"<<eConfig<<" value:"<<refData<<END;
		return false;
	}
	m_mapGlobalConfig[eConfig] = refData;
	return UpdateToDb(eConfig);
}
