#include "pch.h"
#include "global/GlobalConfigDBBuild.h"
#include "network/gslink.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "common/ptcm2g_broadcastglobalvalue.h"
#include "other/servercrossdatamgr.h"

// generate by ProtoGen at date: 2017/4/23 22:29:10

void PtcG2M_UpdateGlobalValue::Process(UINT32 dwConnID)
{
	if (m_Data.id() == eDragonGlobal)
	{
		std::string value = CGlobalConfigDBBuild::Instance()->GetConfigValue(eDragonGlobal);
		KKSG::GlobalDragonInfo info;
		info.ParseFromString(value);
		bool isWeakIn = false;
		bool isin = false;
		if (m_Data.has_dragoninfo() && m_Data.dragoninfo().weakinfos_size())
		{
			const KKSG::GlobalDragonWeakInfo& weakInfo = m_Data.dragoninfo().weakinfos(0);
			for (int i = 0; i < info.weakinfos_size(); ++i)
			{
				if (info.weakinfos(i).sceneid() == weakInfo.sceneid())
				{
					isWeakIn = true;
					break;
				}
			}
			if (!isWeakIn)
			{
				KKSG::GlobalDragonWeakInfo* addWeak = info.add_weakinfos();
				addWeak->set_sceneid(weakInfo.sceneid());
				addWeak->set_opentime(weakInfo.opentime());
				SSInfo<<"update dragon global, sceneid:"<<weakInfo.sceneid() << " opentime:" << weakInfo.opentime()<<END;
			}

		}
		if (m_Data.has_dragoninfo() && m_Data.dragoninfo().sceneid_size() > 0 && m_Data.dragoninfo().timestamp_size() > 0)
		{
			UINT32 sceneid = m_Data.dragoninfo().sceneid(0);
			UINT32 timestamp = m_Data.dragoninfo().timestamp(0);

			for (int i = 0; i < info.sceneid_size(); ++i)
			{
				if (info.sceneid(i) == sceneid)
				{
					isin = true;
					break;
				}
			}
			if (!isin)
			{
				info.add_sceneid(sceneid);
				info.add_timestamp(timestamp);

				SSInfo<<"update dragon global, sceneid:"<<sceneid<<" time:"<<timestamp<<END;

			}
		}
		if (!isin || !isWeakIn)
		{
			CGlobalConfigDBBuild::Instance()->SetConfigValue(eDragonGlobal, info.SerializeAsString());
			CGlobalConfigDBBuild::Instance()->UpdateToDb(eDragonGlobal);

			// send to all line
			PtcM2G_BroadCastGlobalValue globalvalue;
			globalvalue.m_Data.set_id(eDragonGlobal);
			globalvalue.m_Data.set_value(CGlobalConfigDBBuild::Instance()->GetConfigValue(eDragonGlobal));
			GSLink::Instance()->SendToAllLine(globalvalue);

			//syn to ts/cross_gs
			ServerCrossDataSyn::SynDataToWorld(0, SCDST_ALL);
		}

	}
	else if (m_Data.id() == eLoginActivity && m_Data.has_loginactivity())
	{
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eLoginActivity,  m_Data.loginactivity().SerializeAsString());
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eLoginActivity);
	}
	else if (m_Data.id() == eCompeteDragonInfo && m_Data.has_competedragoninfo())
	{
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eCompeteDragonInfo,  m_Data.competedragoninfo().SerializeAsString());
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eCompeteDragonInfo);
	}
}
