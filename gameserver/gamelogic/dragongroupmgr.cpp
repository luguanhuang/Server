#include "pch.h"
#include "dragongroupmgr.h"
#include "dragongrouprecord.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "util/timespecificmgr.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "dragonrecord.h"
#include "dragonconfig.h"
#include "DragonNestTable.h"
#include "live/livemanager.h"
#include "live/liveinfo.h"
#include "live/liverecord.h"
#include "stagemgr.h"
#include "scene/sceneteam.h"

INSTANCE_SINGLETON(DragonGroupMgr);

DragonGroupMgr::DragonGroupMgr(){}

DragonGroupMgr::~DragonGroupMgr(){}

void DragonGroupMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{

}


bool DragonGroupMgr::Init()
{
    DragonGroupMaxTime = 24*3600*GetGlobalConfig().GetInt("DragonGroupMaxTime", 7);
    DragonGroupMaxCount = GetGlobalConfig().GetInt("DragonGroupMaxCount", 50);

	return true;
}

void DragonGroupMgr::Uninit()
{
}

void DragonGroupMgr::RpcGetDragonGroupRoleList(Role* pRole, const KKSG::DragonGroupRoleListC2S &roArg, KKSG::DragonGroupRoleListS2C &roRes)
{
    if(NULL == pRole)return;
    pRole->Get<DragonGroupRecord>()->FillProtocRoleList(roRes);
}

void DragonGroupMgr::RpcGetDragonGroupRecord(Role* pRole, const KKSG::DragonGroupRecordC2S &roArg, KKSG::DragonGroupRecordS2C &roRes)
{
    if(NULL == pRole)return;
    UINT32 maxCommadNum = 0; 
    UINT32 maxWatchNum = 0;
    pRole->Get<CLiveRecord>()->GetMaxCommendViewNum(maxCommadNum, maxWatchNum);
    pRole->Get<DragonGroupRecord>()->FileProtocRecordList(roRes, false, maxCommadNum, maxWatchNum);
}

void DragonGroupMgr::RecordDragonGroup(const std::vector<UINT64>& roleidlist, Scene* pScene, bool iswin)
{
    if(NULL == pScene) return;
    SceneTeam* pSceneTeam = pScene->GetSceneTeam();
    if(NULL == pSceneTeam) return;
    UINT32 stageid = (UINT32)pSceneTeam->GetExpID();
    UINT32 stagetime = pScene->GetTimeSpan();
    std::vector<Role*> rolelist;
    for(UINT32 i = 0; i < roleidlist.size(); ++i)
    {
        Role* pRole = RoleManager::Instance()->FindByRoleID(roleidlist[i]);
        if(pRole)rolelist.push_back(pRole);
    }
    UINT32 time = GameTime::GetTime();
    bool isServerFirstPass = IsServerFirstPass(stageid);
    UINT32 commadnum = 0;
    UINT32 watchnum = 0;
    LiveInfo* liveinfo = LiveManager::Instance()->GetLiveInfo(KKSG::LIVE_RECOMMEND, pScene->GetLiveID());
    if(liveinfo)
    {
        commadnum = liveinfo->GetCommendNum();
        watchnum = liveinfo->GetWatchNum();
    }
    for(UINT32 i = 0; i < roleidlist.size(); ++i)
    {
        Role* pRole = pScene->FindSceneRole(roleidlist[i]);
        if(NULL == pRole) continue;
        bool isFirstPass = IsSelfFirstStageID(stageid, pRole);
        pRole->Get<DragonGroupRecord>()->addRecord(time, stageid, stagetime, iswin, isFirstPass, isServerFirstPass, commadnum, watchnum, rolelist);
    }
}

bool DragonGroupMgr::IsLastStageID(UINT32 stageid)
{
    const DragonNestTable::RowData* data = DragonConfig::Instance()->GetDragonTableData(stageid);
    if(NULL == data)
    {
        return false;
    }
    int type = data->DragonNestType;
    int hard = data->DragonNestDifficulty;
    int wave = data->DragonNestWave;

    const DragonNestTable::RowData* nextdata = DragonConfig::Instance()->GetDragonTableData(type, hard, wave+1);
    if(NULL != nextdata)
    {
        return false;
    }
    return true;
}

bool DragonGroupMgr::IsSelfFirstStageID(UINT32 stageid, Role* pRole)
{
    if(!IsLastStageID(stageid))
    {
        return false;
    }
    return 0 == pRole->Get<StageMgr>()->GetDneCount(stageid);
}

bool DragonGroupMgr::IsServerFirstPass(UINT32 stageid)
{
    if(!IsLastStageID(stageid))
    {
        return false;
    }
    return DragonConfig::Instance()->IsAllServerNotPass(stageid);
}
