#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"

void CustomBattleHandleMatch::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	if (!roleinfo->IsJoin(req.uid))
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_NOTJOIN, req.delayid);
		return;
	}
	CustomBattleReq tempreq = req;
	tempreq.matchinfo = new KKSG::CustomBattleMatchInfo();
	CustomBattleMsMatcher::Instance()->Cancel(tempreq.roleid);
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(tempreq.roleid);
	if (NULL != summary)
	{
	    tempreq.matchinfo->mutable_role()->set_roleid(summary->GetID());
	    tempreq.matchinfo->mutable_role()->set_profession(summary->GetProfession());
	    tempreq.matchinfo->mutable_role()->set_rolename(summary->GetName());
	}
    tempreq.matchinfo->mutable_role()->set_serverid(MSConfig::Instance()->GetServerID());
	
	CustomBattle* battle = m_manager->GetBattle(tempreq.uid);
	if (NULL == battle)
	{
        if (CCustomBattleConfig::Instance()->IsSystem((UINT32)tempreq.uid))
		{
			tempreq.matchinfo->mutable_battle()->set_isfair(true);
			///> 时间判断
			int timelimit = GetGlobalConfig().CustomBattleMatcherSystemTime;
			int timebegin = (int)(timelimit/10000);
			int timebeginhour = (int)(timebegin/100);
			int timebeginmin = timebegin - timebeginhour*100;

			int timeend = timelimit - timebegin*10000;
			int timeendhour = (int)(timeend/100);
			int timeendmin = timeend - timeendhour*100;

			if (!GameTime::IsWhithinTime(timebeginhour, timebeginmin, 0, timeendhour, timeendmin, 0))
			{
				ErrorNotify(KKSG::ERR_CUSTOMBATTLE_BOUNTYMODE_NOTOPEN, tempreq.delayid);
				return;
			}

			///> 不能匹配
			if (!roleinfo->CanSystemMatch(tempreq.uid))
			{
				ErrorNotify(KKSG::ERR_CUSTOMBATTLE_MATCHFAILED, tempreq.delayid);
				return;
			}
			roleinfo->FillSystemMatch(tempreq.uid, *tempreq.matchinfo);

            if (!GetGlobalConfig().IsCustomBattleOnlyMatchCross)
            {
			    errorcode = CustomBattleMsMatcher::Instance()->AddSystemMatcher(*tempreq.matchinfo);	
			    if (KKSG::ERR_SUCCESS == errorcode)
			    {
			    	CustomBattleMsMatcher::Instance()->Add(tempreq.roleid, false);
			    	DelayReply(KKSG::ERR_SUCCESS, NULL, tempreq.delayid);
			    }
            }
            else
            {
			    errorcode = m_manager->TransReqToWorld(tempreq);		
            }
		}
		else
		{
			errorcode = m_manager->TransReqToWorld(tempreq);		
		}
	}
	else
	{
		///> 是否能匹配
		errorcode = battle->CanMatch();
		if (KKSG::ERR_SUCCESS == errorcode)
		{
			battle->FillMatch(tempreq.uid, tempreq.roleid, *tempreq.matchinfo);

			errorcode = CustomBattleMsMatcher::Instance()->AddCustomMatcher(*tempreq.matchinfo, battle->IsGM());

			if (KKSG::ERR_SUCCESS == errorcode)
			{
				CustomBattleMsMatcher::Instance()->Add(tempreq.roleid, false);
				DelayReply(KKSG::ERR_SUCCESS, NULL, tempreq.delayid);
			}
		}
	}
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, tempreq.delayid);
	}
	return;
}

void CustomBattleHandleMatch::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS == errorcode)
	{
		CustomBattleMsMatcher::Instance()->Add(req.roleid, true);
	}
	ErrorNotify(errorcode, req.delayid);
}
