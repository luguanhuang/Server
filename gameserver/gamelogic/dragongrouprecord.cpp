#include "pch.h"
#include "dragongroupmgr.h"
#include "dragongrouprecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "util/gametime.h"
#include "weekend4v4mgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "titlerecord.h"

DragonGroupRecord::DragonGroupRecord(Role* role)
    :m_pRole(role), m_ismodify()
{
}

DragonGroupRecord::~DragonGroupRecord()
{
}

DragonGroupRole::DragonGroupRole(Role& role)
{
    roleid = role.GetRoleID();
    rolename = role.GetName();
    profession = role.GetProfession();
}

DragonGroupRole::DragonGroupRole(const KKSG::DragonGroupRoleInfo& roleinfo)
{
    roleid = roleinfo.roleid();
    rolename = roleinfo.rolename();
    profession = roleinfo.profession();
}

DragonGroupRoleList::DragonGroupRoleList(UINT64 roleid, std::vector<Role*>& rolelist)
{
    for(auto itor = rolelist.begin(); itor != rolelist.end(); ++itor)
    {
        if(NULL == *itor) continue;
        if((*itor)->GetRoleID() == roleid)continue;
        roleinfolist.push_back(DragonGroupRole(**itor));
    }
}

DragonGroupRoleList::DragonGroupRoleList(const KKSG::DragonGroupRecordInfoList& record)
{
    stageid = record.stageid();
    time = record.time();
    costtime = record.costtime();
    iswin = record.iswin();
    isFirstPass = record.isfirstpass();
    isServerFirstPass = record.isserverfirstpass();
    commadNum = record.commendnum();
    watchNum = record.watchnum();
    for(int i = 0; i< record.roleinfo().size(); i++)
    {
        roleinfolist.push_back(DragonGroupRole(record.roleinfo().Get(i)));
    }
}

bool DragonGroupRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
    teamerlist.clear();
    recordlist.clear();
    const KKSG::DragonGroupDB& info = poRoleAllInfo->dragongroupdb();
    auto& tmprolelist = info.rolelist().rolelist();
    for(int i = 0; i < tmprolelist.size(); i++)
    {
        teamerlist[tmprolelist.Get(i).roleid()].CopyFrom(tmprolelist.Get(i));
    }

    const auto& tmprecord = info.record().recordlist();
    for(int i = 0; i < tmprecord.size(); i++)
    {
        recordlist.push_back(DragonGroupRoleList(tmprecord.Get(i)));
    }
    fixRecordFirstPass();
    clearRecord();
    return true;
}

void DragonGroupRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
    if (!m_ismodify.TestAndReset())
    {
    	return;
    }
    clearRecord();
    KKSG::DragonGroupDB& info = *poRoleAllInfo->mutable_dragongroupdb();
    info.Clear();
    FillProtocRoleList(*info.mutable_rolelist());
    FileProtocRecordList(*info.mutable_record(), true, 0, 0);
	roChanged.insert(&info);
}


void DragonGroupRecord::GMResetRecord()
{
    teamerlist.clear();
    recordlist.clear();
    m_ismodify.Set();
}

UINT32 DragonGroupRecord::GetTime()
{
    return (UINT32)GameTime::GetTime();
}

void DragonGroupRecord::FillDragonGroupRoleInfo(KKSG::DragonGroupRoleInfo& roleinfo, Role* pRole)
{
    if(NULL == pRole) return;
    roleinfo.set_roleid(pRole->GetRoleID());
    roleinfo.set_rolename(pRole->GetName());
    roleinfo.set_profession(pRole->GetProfession());
    roleinfo.set_title(pRole->Get<TitleRecord>()->GetTitle());
    roleinfo.set_level(pRole->GetLevel());
    roleinfo.set_fighting((UINT32)(pRole->GetAttr(TOTAL_POWERPOINT)));
    CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID());
    if(guild) roleinfo.set_guild(guild->GetGuildName());
    roleinfo.set_uid(pRole->GetNickId());
}

void DragonGroupRecord::addRecord(UINT32 time, UINT32 stageid, UINT32 mcosttime, bool iswin, bool isfirstpass, bool isserverfirstpass, 
    UINT32 commadnum, UINT32 watchnum, std::vector<Role*>& rolelist)
{
    DragonGroupRoleList dragongrouprolelist(m_pRole->GetRoleID(), rolelist);
    dragongrouprolelist.stageid = stageid;
    dragongrouprolelist.time = time;
    dragongrouprolelist.costtime = mcosttime;
    dragongrouprolelist.iswin = iswin;
    dragongrouprolelist.isFirstPass = isfirstpass;
    dragongrouprolelist.isServerFirstPass = isserverfirstpass;
    dragongrouprolelist.commadNum = commadnum;
    dragongrouprolelist.watchNum = watchnum;
    recordlist.push_back(dragongrouprolelist);
    for(auto itor = rolelist.begin(); itor != rolelist.end(); ++itor)
    {
        if(NULL == *itor) continue;
        UINT64 tmproleid = (*itor)->GetRoleID();
        if(m_pRole->GetRoleID() == tmproleid)continue;
        //bool hasrole = (teamerlist.end() != teamerlist.find(tmproleid));
        KKSG::DragonGroupRoleInfo& tmproleinfo = teamerlist[tmproleid];
        UINT32 count = tmproleinfo.stagecount();
        FillDragonGroupRoleInfo(tmproleinfo, *itor);
        tmproleinfo.set_stagecount(count+1);
        tmproleinfo.set_stageid(stageid);
        tmproleinfo.set_stagetime(time);
    }
    clearRecord();
    m_ismodify.Set();
}

void DragonGroupRecord::clearRecord()
{
    UINT32 MaxRecordTime= DragonGroupMgr::Instance()->DragonGroupMaxTime;
    UINT32 MaxRecordCount = DragonGroupMgr::Instance()->DragonGroupMaxCount;
    bool isChanged = false;
    for(auto itor = recordlist.begin(); itor != recordlist.end();)
    {
        if(itor->time + MaxRecordTime < GetTime() || recordlist.size() > MaxRecordCount)
        {
            recordlist.erase(itor++);
            isChanged = true;
        }else{
            itor++;
        }
    }

    if(recordlist.empty())
    {
        if(!teamerlist.empty())
        {
            teamerlist.clear();
            isChanged = true;
        }
        return;
    }

    UINT32 fisttime = recordlist.front().time;

    for(auto itor = teamerlist.begin(); itor != teamerlist.end();)
    {
        if(itor->second.stagetime() < fisttime)
        {
            teamerlist.erase(itor++);
            isChanged = true;
        }else{
            itor++;
        }
    }
    if(isChanged) m_ismodify.Set();
}

void DragonGroupRecord::fixRecordFirstPass()
{
    std::set<UINT32> firstPassStageid;
    for(auto itor = recordlist.begin(); itor != recordlist.end();itor++)
    {
        if(!itor->isFirstPass) continue;
        if(firstPassStageid.end() != firstPassStageid.find(itor->stageid))
        {
            itor->isFirstPass = false;
        }else{
            firstPassStageid.insert(itor->stageid);
        }
    }
}

void DragonGroupRecord::FillProtocRoleList(KKSG::DragonGroupRoleListS2C& rolelist)
{
    for(auto itor = teamerlist.begin(); itor != teamerlist.end();++itor)
    {
        rolelist.add_rolelist()->CopyFrom(itor->second);
    }
    rolelist.set_errorcode(KKSG::ERR_SUCCESS);
}

void DragonGroupRecord::FileProtocRecordList(KKSG::DragonGroupRecordS2C& record, bool isSaveDB, UINT32 maxCommadNum, UINT32 maxWatchNum)
{
    bool slagMostCommend = false;
    bool slagMostWatch = false;
    for(auto itor = recordlist.begin(); itor != recordlist.end();itor++)
    {
        auto ptmp = record.add_recordlist();
        ptmp->set_stageid(itor->stageid);
        ptmp->set_time(itor->time);
        ptmp->set_costtime(itor->costtime);
        ptmp->set_iswin(itor->iswin);
        ptmp->set_isfirstpass(itor->isFirstPass);
        ptmp->set_isserverfirstpass(itor->isServerFirstPass);
        ptmp->set_commendnum(itor->commadNum);
        ptmp->set_watchnum(itor->watchNum);
        if(!isSaveDB)
        {
            if(itor->commadNum >= maxCommadNum)
            {
                ptmp->set_ismostcommendnum(!slagMostCommend);
                slagMostCommend = true;
            }else{
                ptmp->set_ismostcommendnum(false);
            }
            if(itor->watchNum >= maxWatchNum)
            {
                ptmp->set_ismostwatchnum(!slagMostWatch);
                slagMostWatch = true;
            }else{
                ptmp->set_ismostwatchnum(false);
            }
        }
        for(auto itorx = itor->roleinfolist.begin(); itorx != itor->roleinfolist.end(); itorx++)
        {
            auto ptmpx = ptmp->add_roleinfo();
            ptmpx->set_roleid(itorx->roleid);
            ptmpx->set_rolename(itorx->rolename);
            ptmpx->set_profession(itorx->profession);
        }
    }
    record.set_errorcode(KKSG::ERR_SUCCESS);
}

