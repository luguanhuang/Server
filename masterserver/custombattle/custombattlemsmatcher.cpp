#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "scene/sceneswitch.h"
#include "scene/scenemanager.h"
#include "network/worldlink.h"
#include "util/gametime.h"
#include "custombattlemsmatcher.h"
#include "table/globalconfig.h"
#include "custombattle/ptcm2k_custombattledelmatch.h"
#include "custombattle/ptcm2k_transcustombattlematch.h"
#include "other/matchhandler.h"

INSTANCE_SINGLETON(CustomBattleMsMatcher);

CustomBattleMsMatcher::CustomBattleMsMatcher()
{

}

CustomBattleMsMatcher::~CustomBattleMsMatcher()
{

}

void CustomBattleMsMatcher::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	std::map<UINT64, KKSG::CustomBattleMatchInfo*> timeout;
	for (auto i = m_index2info.begin(); i != m_index2info.end(); ++i)
	{
		if (i->second.battle().issystem() && (i->second.role().timestamp() + GetGlobalConfig().CustomBattleMatchExpireTime <= GameTime::GetTime()))
		{
			timeout[i->first] = &i->second;
		}
	}
	for (auto j = timeout.begin(); j != timeout.end(); ++j)
	{
		// trans to world
		KKSG::CustomBattleMatchInfo* info = j->second;
		PtcM2K_TransCustomBattleMatch ptc;
		*ptc.m_Data.mutable_info() = *info;
		if (WorldLink::Instance()->SendTo(ptc))
        {
		    SSInfo<<"trans roleid:"<<info->role().roleid()<<" uid:"<<info->battle().battleuid()<<" to world"<<END;
		    DelSystemMatcher(j->first);
		    m_roleid2index[j->first].iscross = true;
        }
        else
        {
            info->mutable_role()->set_timestamp((UINT32)GameTime::GetTime());
		    SSInfo<<"trans roleid:"<<info->role().roleid()<<" uid:"<<info->battle().battleuid()<<" to world failed"<<END;
        }
	}
	CustomBattleMatcherMgr::OnTimer(dwID, dwCount);
}

void CustomBattleMsMatcher::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	// notice ms to change scene
	if (KKSG::ERR_SUCCESS != res.result())
	{
		SSWarn<<"create scene failed, error:"<<res.result()<<END;
		return;
	}
	if (NULL == scene)
	{
		SSWarn<<"scene is NULL"<<END;
		return;
	}

	SSInfo<<"create scene mapid:"<<scene->GetMapID()<<" sceneid:"<<scene->GetID()<<" gs:"<<scene->GetLine()<<END;

	if (!roParam.has_custombattle())
	{
		SSWarn<<"param has no custombattle, mapid:"<<scene->GetMapID()<<" sceneid:"<<scene->GetID()<<" gs:"<<scene->GetLine()<<END;
		return;
	}

	const KKSG::CustomBattleParam& customparam = roParam.custombattle();
	// one
	if (customparam.has_infoa())
	{
		KKSG::SceneSwitchData oData;
		CRole* role = CRoleManager::Instance()->GetByRoleID(customparam.infoa().role().roleid());
		if (role != NULL)
		{
			CSceneSwitch::EnterScene(role, scene->GetID(), oData);
			SSInfo<<"change scene:"<<customparam.infoa().role().roleid()<<" serverid:"<<customparam.infoa().role().serverid()<<END;
		}
		else
		{
			SSWarn<<"role is NULL, roleid:"<<customparam.infoa().role().roleid()<<END;
		}
	}
	else
	{
		SSWarn<<"param has no infoa"<<END;
	}
	// two
	if (customparam.has_infob())
	{
		KKSG::SceneSwitchData oData;
		CRole* role = CRoleManager::Instance()->GetByRoleID(customparam.infob().role().roleid());
		if (role != NULL)
		{
			CSceneSwitch::EnterScene(role, scene->GetID(), oData);
			SSInfo<<"change scene:"<<customparam.infob().role().roleid()<<" serverid:"<<customparam.infob().role().serverid()<<END;
		}
		else
		{
			SSWarn<<"role is NULL, roleid:"<<customparam.infob().role().roleid()<<END;
		}
	}
	else
	{
		SSWarn<<"param has no infob"<<END;
	}
}

void CustomBattleMsMatcher::DoBattlePair(CustomBattleMatchSceneInfo& info, const std::vector<UINT64>& matchone, const std::vector<UINT64>& matchtwo)
{
	std::stringstream ss;
	KKSG::CreateBattleParam param;
	KKSG::CustomBattleParam* customparam = param.mutable_custombattle();
	if (matchone.size() == 1)
	{
		auto k = m_index2info.find(matchone[0]);
		if (k != m_index2info.end())
		{
			*param.mutable_custombattle()->mutable_infoa() = k->second;
			ss<<k->first<<", ";

			m_index2info.erase(k);
		}
	}
	if (matchtwo.size() == 1)
	{
		auto k = m_index2info.find(matchtwo[0]);
		if (k != m_index2info.end())
		{
			*param.mutable_custombattle()->mutable_infob() = k->second;
			ss<<k->first<<", ";

			m_index2info.erase(k);
		}
	}

	SSInfo<<"make pair, is system:"<<info.issystem<<" is fair:"<<info.isfaire<<" map id:"<<info.mapid<<" roles:"<<ss.str()<<END;

	if (CSceneCreator::Instance()->CreateBattleScene(info.mapid, this, param))
	{
		LogInfo("Match Create One Game");
	}
	else
	{
		LogError("Match Create One Game Failed");
	}
}

void CustomBattleMsMatcher::Add(UINT64 roleid, bool iscross)
{
	m_roleid2index[roleid].iscross = iscross;

	///> 取消其他匹配
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL != role)
	{
		HMatchHandler handler;
		handler.CheckRoleStop(role);
	}
}

void CustomBattleMsMatcher::Cancel(UINT64 roleid)
{
	auto i = m_roleid2index.find(roleid);
	if (i == m_roleid2index.end())
	{
		return;
	}
	else
	{
		if (i->second.iscross)
		{
			// send to world
			PtcM2K_CustomBattleDelMatch ptc;
			ptc.m_Data.set_roleid(roleid);
			WorldLink::Instance()->SendTo(ptc);
		}
		else
		{
			TryDel(roleid);			
		}
		m_roleid2index.erase(i);
	}
}
