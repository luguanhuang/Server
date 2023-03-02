#include "pch.h"
#include "servercrossdatamgr.h"
#include "common/ptcm2k_servercrossdatam2w.h"
#include "config.h"
#include "network/worldlink.h"
#include "global/GlobalConfigDBBuild.h"
#include "levelseal/levelsealMgr.h"
#include "table/OpenSystemMgr.h"
#include "pay/payconsumemgr.h"
#include "nestweek/nestweekmgr.h"
#include "serverinfo/servercrossdatamgrbase.h"

void ServerCrossDataSyn::SynDataToWorld(UINT32 connID/* = 0*/, int synType/* = SCDST_ALL*/)
{
	PtcM2K_ServerCrossDataM2W ptc;	
	ptc.m_Data.set_syntype(synType);
	FillData(*ptc.m_Data.add_serverdata());

	bool ret = true;
	if (connID)
	{
		ret = WorldLink::Instance()->Send(connID, ptc);
	}
	else
	{
		ret = WorldLink::Instance()->SendToAllConn(ptc);
	}

	if (!ret)
	{
		SSError << " send server cross data failed connID = " << connID << END;
	}
	SSInfo << " Server Cross Data Syn To World connID = " << connID << " synType = " << synType << END;
	ServerCrossDataMgrBase::InfoPrint(m_lastSynData);
}

void ServerCrossDataSyn::FillData(KKSG::ServerCrossData& data)
{
	data.set_serverid(MSConfig::Instance()->GetServerID());
	data.set_serveropentime(MSConfig::Instance()->GetGameServerOpenTimeStamp());
	std::string wl = CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel);
	data.set_worldlevel(convert<UINT32>(wl));
	data.set_sealtype(CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType);
	data.set_seallevel(CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel);
	data.set_isbackflow(MSConfig::Instance()->IsBackFlowServer());
	data.set_backflowlevel(MSConfig::Instance()->GetBackFlowLevel());
	const auto& idipClose = OpenSystemMgr::Instance()->GetIdipSystemClose();
	for(auto it = idipClose.begin(); it != idipClose.end(); ++it)
	{
		data.add_idipclosesys(*it);
	}
	CPayConsumeMgr::Instance()->BuildData(data.mutable_rebate());
	auto& teamdata = *data.mutable_teamdata();
	FillTeamData(teamdata);

	m_lastSynData = data;//记录最后同步数据
}

void ServerCrossDataSyn::FillTeamData(KKSG::ServerCrossTeamData& teamdata)
{
	teamdata.set_weeknextexpid(NestWeekMgr::Instance()->GetCurExp());
	std::string value = CGlobalConfigDBBuild::Instance()->GetConfigValue(eDragonGlobal);
	KKSG::GlobalDragonInfo info;
	if(info.ParseFromString(value))
	{
		*teamdata.mutable_globaldragoninfo() = info;
	}
	else
	{
		SSError << " parse dragon global data failed " << END;
	}
}

bool ServerCrossDataSyn::IsDragonSceneFininshed(UINT32 sceneID)
{
	if(!m_lastSynData.has_teamdata())
	{
		SSWarn << " will not run here " << END;
		static KKSG::ServerCrossData tmpData;
		FillData(tmpData);
	}
	return ServerCrossDataMgrBase::IsDragonSceneFinished(m_lastSynData.teamdata(), sceneID);
}

KKSG::ServerCrossData ServerCrossDataSyn::m_lastSynData;

