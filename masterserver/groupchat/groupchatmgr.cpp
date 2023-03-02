#include "pch.h"
#include "groupchatmgr.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "pb/project.pb.h"
#include "timeutil.h"
#include "groupchat/ptcm2c_groupchatmanager.h"
#include "groupchat/ptcm2c_groupchatdismiss.h"
#include "groupchat/ptcm2c_groupchatquit.h"
#include "groupchat/ptcm2c_groupchatissuecount.h"
#include "groupchat/ptcm2c_groupchatapply.h"
#include "chat/chatmgr.h"
#include "table/StringTableMgr.h"
#include "table/globalconfig.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "db/mysqlmgr.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelrolentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelroleissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupntf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaverolentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupntf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaveroleissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupissuentf.h"
#include "network/dblink.h"
#include "tsssdk/MsTssHandler.h"
#include "loghelper/tlogr.h"

INSTANCE_SINGLETON(GroupChatMgr);

GroupChatMgr::GroupChatMgr(){}

GroupChatMgr::~GroupChatMgr(){}

std::string GroupChatGetRoleName(UINT64 roleid)
{
    CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
    if(NULL == pRoleSummary) return "";
    return pRoleSummary->GetName();
}

std::string GroupChatGetGroupName(UINT64 groupID)
{
    GroupChatGroupInfo* groupinfo = GroupChatMgr::Instance()->GetGroupInfo(groupID);
    if(NULL == groupinfo) return "";
    return groupinfo->groupName;
}

bool GroupChatMgr::Init()
{
    m_timeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1*1000, -1, __FILE__, __LINE__);
    if(m_timeHandler == INVALID_HTIMER)
    {
        return false;
    }
    TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 5);
    m_dayChange = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&GroupChatMgr::UpdateOnDayPass, this));

    MaxMemberCountInGroup = GetGlobalConfig().GetInt("GroupChatMaxMemberCountInGroup", 10);
    MaxGroupCountForRole = GetGlobalConfig().GetInt("GroupChatMaxGroupCountForRole", 5);
    MaxZMGroupIssue = GetGlobalConfig().GetInt("GroupChatMaxZMGroupIssue", 5);
    MaxZMRoleIssue = GetGlobalConfig().GetInt("GroupChatMaxZMRoleIssue", 10);
    MaxGroupLifeTime = 24*3600*GetGlobalConfig().GetInt("GroupChatMaxLiftTime", 1);
    MaxGroupApplyCount = GetGlobalConfig().GetInt("GroupChatMaxApplyCount", 50);

    m_saveToDBCount = 0;
    LogDebug("GroupChat MaxMemberCountInGroup=%d, MaxGroupCountForRole=%d, MaxZMGroupIssue=%d, MaxZMRoleIssue=%d\n", MaxMemberCountInGroup, MaxGroupCountForRole, MaxZMGroupIssue, MaxZMRoleIssue);

    QueryDBGroupIssue();
    QueryDBRoleIssue();
    QueryDBGroup();
    QueryDBRole();
    return true;
}

void GroupChatMgr::Uninit()
{
    if(m_timeHandler != INVALID_HTIMER)
    {
        CTimerMgr::Instance()->KillTimer(m_timeHandler);
    }
}

const static UINT32 MAX_SAVE_NUM_ONCE_GROUP = 10;
const static UINT32 MAX_SAVE_NUM_ONCE_ROLE = 10;
const static UINT32 MAX_SAVE_NUM_ONCE_GROUPISSUE = 10;
const static UINT32 MAX_SAVE_NUM_ONCE_ROLEISSUE = 10;
void GroupChatMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
    UINT64 begin = TimeUtil::GetMilliSecond();
    string ss;
    bool isUpdate = false;
    m_saveToDBCount = (m_saveToDBCount + 1) % 4;

    //每1*4秒更新每一个
    if (0 == m_saveToDBCount)
    {	
        UINT32 count = 0;
        while(!m_isChangedGroup.empty() && count < MAX_SAVE_NUM_ONCE_GROUP)
        {
            UINT64 index = m_isChangedGroup.front();
            m_isChangedGroup.pop();
            count++;

            SaveDBGroupID(index);
            isUpdate = true;
        }
        if (!m_isChangedGroup.empty())
        {
            LogInfo("group need save num:%u", m_isChangedGroup.size());
        }
        ss = "group";
    }
    else if (1 == m_saveToDBCount)
    {
        UINT32 count = 0;
        while(!m_isChangedRole.empty() && count < MAX_SAVE_NUM_ONCE_ROLE)
        {
            UINT64 index = m_isChangedRole.front();
            m_isChangedRole.pop();
            count++;

            SaveDBRoleID(index);
            isUpdate = true;
        }
        if(!m_isChangedRole.empty())
        {
            LogInfo("role need save num:%u", m_isChangedRole.size());
        }
        ss = "role";
    }
    else if (2 == m_saveToDBCount)
    {
        UINT32 count = 0;
        while(!m_isChangedGroupIssue.empty() && count < MAX_SAVE_NUM_ONCE_GROUPISSUE)
        {
            UINT64 index = m_isChangedGroupIssue.front();
            m_isChangedGroupIssue.pop();
            count++;

            SaveDBGroupIssueID(index);
            isUpdate = true;
        }
        if(!m_isChangedGroupIssue.empty())
        {
            LogInfo("GroupIssue still need save:%u", m_isChangedGroupIssue.size());
        }
        ss = "GroupIssue";
    }
    else if (3 == m_saveToDBCount)
    {
        UINT32 count = 0;
        while(!m_isChangedRoleIssue.empty() && count < MAX_SAVE_NUM_ONCE_ROLEISSUE)
        {
            UINT64 index = m_isChangedRoleIssue.front();
            m_isChangedRoleIssue.pop();
            count++;

            SaveDBRoleIssueID(index);
            isUpdate = true;
        }
        if(!m_isChangedRoleIssue.empty())
        {
            LogInfo("RoleIssue still need save num:%u", m_isChangedRoleIssue.size());
        }
        ss = "RoleIssue";
    }
    UINT64 end = TimeUtil::GetMilliSecond();
    if (isUpdate)
    {
        LogInfo("GroupChatMgr %s save to db use time: %llu ms", ss.c_str(), end - begin);
    }
}

bool GroupChatMgr::UpdateOnDayPass()
{
    mTeamIssueList.clear();
    ClearDBGroupIssue();

    mRoleIssueList.clear();
    ClearDBRoleIssue();

    UINT32 nowtime = GetTime();
    for(auto itor = m_TeamInfoList.begin(); itor != m_TeamInfoList.end();)
    {
        if(itor->second.lastchattime + MaxGroupLifeTime < nowtime)
        {
            DismissGroup(itor->second.groupID, true);
            m_TeamInfoList.erase(itor++);
        }else{
            itor++;
        }
    }
    ClearDBGroup();

    for(auto itor = m_RoleInfoList.begin(); itor != m_RoleInfoList.end();itor++)
    {
        bool slag = false;
        if(!itor->second.zmRoleIssueList.empty() || !itor->second.zmGroupIssueList.empty())
        {
            slag = true;
        }
        itor->second.zmRoleIssueList.clear();
        itor->second.zmGroupIssueList.clear();
        if(slag)
        {
            m_isChangedRole.push(itor->first);
        }
    }

    return true;
}

UINT32 GroupChatMgr::GetTime()
{
    return (UINT32)GameTime::GetTime();
}

UINT32 GroupChatMgr::GetTodayBeginTime()
{
    return (UINT32)GameTime::GetTodayBeginTime();
}

bool GroupChatMgr::IsSameDay(UINT32 time1, UINT32 time2)
{
    return GameTime::IsInSameDay((time_t)time1, (time_t)time2, true);
}

GroupChatGroupInfo::GroupChatGroupInfo(UINT64 mGroupID, const std::string& mGroupName, UINT32 mCreateType, UINT32 mCreateTime, UINT64 mLeaderRoleID) 
    : groupID(mGroupID), groupName(mGroupName), createtype(mCreateType), createtime(mCreateTime), lastchattime(mCreateTime), leaderRoleID(mLeaderRoleID)
{
    memberRoleIDList[mLeaderRoleID] = mCreateTime;
}

GroupChatGroupInfo::GroupChatGroupInfo(const KKSG::DBGroupChatSaveGroup& pDBInfo)
{
    groupID = pDBInfo.index();
    groupName = pDBInfo.groupname();
    createtype = pDBInfo.createtype();
    createtime = pDBInfo.createtime();
    lastchattime = pDBInfo.time();
    leaderRoleID = pDBInfo.leaderroleid();
    for(int i = 0; i < pDBInfo.memberroleidlist().size(); ++i)
    {
        memberRoleIDList[pDBInfo.memberroleidlist(i).id()] = pDBInfo.memberroleidlist(i).time();
    }
}

void GroupChatGroupInfo::FillDBProtoc(KKSG::DBGroupChatSaveGroup& pDBInfo)
{
    pDBInfo.set_index(groupID);
    pDBInfo.set_groupname(groupName);
    pDBInfo.set_createtype(createtype);
    pDBInfo.set_createtime(createtime);
    pDBInfo.set_time(lastchattime);
    pDBInfo.set_leaderroleid(leaderRoleID);
    for(auto itor = memberRoleIDList.begin(); itor != memberRoleIDList.end(); ++itor)
    {
        auto tmp = pDBInfo.add_memberroleidlist();
        tmp->set_id(itor->first);
        tmp->set_time(itor->second);
    }
}

void GroupChatGroupInfo::FillProtoc(KKSG::GroupChatTeamInfo* teaminfo)
{
    if(NULL == teaminfo) 
    {
        LogInfo("GroupChat groupchatinfo is error. groupchatid = %llu\n", groupID);
        return;
    }
    teaminfo->set_groupchatid(groupID);
    teaminfo->set_leaderroleid(leaderRoleID);
    teaminfo->set_groupchatname(groupName);
    teaminfo->set_createtype(createtype);
    teaminfo->set_groupcreatetime(createtime);
    teaminfo->set_rolecount(memberRoleIDList.size());
}

bool GroupChatMgr::IsGroupLeader(UINT64 groupID, UINT64 roleid)
{
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo) return false;
    return pGroupInfo->leaderRoleID == roleid;
}

bool GroupChatMgr::IsGroupMember(UINT64 groupID, UINT64 roleid)
{
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo) return false;
    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(roleid);
    if(NULL == pRoleInfo) return false;

    bool slag1 = (pGroupInfo->memberRoleIDList.end() != pGroupInfo->memberRoleIDList.find(roleid));
    bool slag2 = (pRoleInfo->groupIDList.end() != pRoleInfo->groupIDList.find(groupID));

    if(slag1 && !slag2)
    {
        pRoleInfo->groupIDList.insert(groupID);
        m_isChangedRole.push(roleid);
    }
    if(!slag1 && slag2)
    {
        pRoleInfo->groupIDList.erase(groupID);
        m_isChangedRole.push(roleid);
    }
    return slag1;
}

UINT32 GroupChatMgr::GetGroupMemberJoinTime(UINT64 groupID, UINT64 roleid)
{
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo) return (UINT32)-1;
    auto itor = pGroupInfo->memberRoleIDList.find(roleid);
    if(pGroupInfo->memberRoleIDList.end() == itor) return (UINT32)-1;
    return itor->second;
}

void GroupChatMgr::FillRoleInfo(KKSG::GroupChatPlayerInfo* pRoleInfo, CRoleSummary* pRoleSummary, UINT32 mTime)
{
    if(NULL == pRoleInfo) return;
    if(NULL == pRoleSummary) return;
    pRoleInfo->set_roleid(pRoleSummary->GetID());
    pRoleInfo->set_rolename(pRoleSummary->GetName());
    pRoleInfo->set_profession(pRoleSummary->GetProfession());
    pRoleInfo->set_title(pRoleSummary->GetTitleID());
    pRoleInfo->set_level(pRoleSummary->GetLevel());
    pRoleInfo->set_fighting(pRoleSummary->GetPowerPoint());
    Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRoleSummary->GetID());
    if(pGuild)pRoleInfo->set_guild(pGuild->GetName());
    pRoleInfo->set_joingrouptime(mTime);
    pRoleInfo->set_uid(pRoleSummary->GetNickID());
}

void GroupChatMgr::NewDelMemberNotify(UINT64 groupID, UINT64 roleid, bool isNew)
{
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo) return;
    KKSG::ChatInfo chatinfo;
    chatinfo.set_channel(KKSG::GroupChatChannel);
    if(isNew){
        chatinfo.set_info(StringTableMgr::Instance()->GetText("GroupChatNewPlayer").c_str());
    }else{
        chatinfo.set_info(StringTableMgr::Instance()->GetText("GroupChatDelPlayer").c_str());
    }
    chatinfo.set_time(GetTime());
    pGroupInfo->FillProtoc(chatinfo.mutable_groupchatinfo());

    CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
    if(NULL == pRoleSummary) 
    {
        LogInfo("GroupChat NewDel Error. roleid = %llu\n", roleid);
        return;
    }

    KKSG::ChatSource* pNewRole = chatinfo.mutable_groupchatnewrole();
    if(NULL == pNewRole) return;
    pNewRole->set_roleid(roleid);
    pNewRole->set_name(pRoleSummary->GetName());

    KKSG::ChatSource* pSource = chatinfo.mutable_source();
    if(NULL == pSource) return;
    pSource->CopyFrom(*pNewRole);

    CChatMgr::Instance()->AddChat(groupID, chatinfo);
    CChatMgr::Instance()->SendGroupChat(pGroupInfo->memberRoleIDList, chatinfo);
}

void GroupChatMgr::SendGroupChat(UINT64 groupID, const KKSG::ChatInfo& chatinfo)
{
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo) return;
    pGroupInfo->lastchattime = GetTime();
    CChatMgr::Instance()->SendGroupChat(pGroupInfo->memberRoleIDList, chatinfo);
    m_isChangedGroup.push(groupID);
}

bool GroupChatMgr::RpcRoleGetGroupInfo(CRole* pRole, UINT64 groupID, KKSG::GroupChatGetGroupInfoS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return false;
    }
    if(!IsGroupMember(groupID, pRole->GetID()))
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
        return false;
    }

    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo) 
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
        return false;
    }

    for(auto itor = pGroupInfo->memberRoleIDList.begin(); itor != pGroupInfo->memberRoleIDList.end(); ++itor)
    {
        CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(itor->first);
        FillRoleInfo(roRes.add_playerlist(), pRoleSummary, itor->second);
    }
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    return true;
}

bool GroupChatMgr::RpcChangeGroupName(CRole* pRole, const KKSG::GroupChatChangeNameC2S& roArg, KKSG::GroupChatChangeNameS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return false;
    }
    if(roArg.newname().size() < 1 || roArg.newname().size() > 100)
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return false;
    }
    roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(roArg.groupchatid());
    if(NULL == pGroupInfo) return false;
    if(pGroupInfo->leaderRoleID != pRole->GetID()) return false;
    KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(roArg.newname());
    if(nErrCode != KKSG::ERR_SUCCESS)
    {
        roRes.set_errorcode(nErrCode);
        return false;
    }
    pGroupInfo->groupName = roArg.newname();
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    m_isChangedGroup.push(roArg.groupchatid());
    return true;
}

void GroupChatMgr::PtcDismissGroup(CRole* ptcRole, GroupChatGroupInfo* pGroupInfo, bool isSystemDismiss)
{
    if(NULL == ptcRole)return;
    if(NULL == pGroupInfo)return;
    PtcM2C_GroupChatDismiss ptc;
    ptc.m_Data.set_groupchatid(pGroupInfo->groupID);
    if(!isSystemDismiss)
    {
        ptc.m_Data.set_roleid(pGroupInfo->leaderRoleID);
    }
    ptcRole->Send(ptc);
}

void GroupChatMgr::PtcDelRole(CRole* ptcRole, GroupChatGroupInfo* pGroupInfo, UINT64 delRoleid, bool isSelfQuit)
{
    if(NULL == ptcRole)return;
    if(NULL == pGroupInfo)return;
    if(isSelfQuit)
    {
        PtcM2C_GroupChatQuit ptc;
        ptc.m_Data.set_groupchatid(pGroupInfo->groupID);
        ptc.m_Data.set_roleid(delRoleid);
        ptcRole->Send(ptc);
    }else{
        PtcM2C_GroupChatManager ptc;
        ptc.m_Data.set_groupchatid(pGroupInfo->groupID);
        ptc.m_Data.add_subrolelist(delRoleid);
        ptcRole->Send(ptc);
    }
}

void GroupChatMgr::PtcAddRole(CRole* ptcRole, GroupChatGroupInfo* pGroupInfo, CRoleSummary* pAddRoleSummary, UINT32 mTime)
{
    if(NULL == ptcRole)return;
    if(NULL == pGroupInfo)return;
    if(NULL == pAddRoleSummary)return;
    PtcM2C_GroupChatManager ptc;
    ptc.m_Data.set_groupchatid(pGroupInfo->groupID);
    FillRoleInfo(ptc.m_Data.add_addrolelist(), pAddRoleSummary, mTime);
    ptcRole->Send(ptc);
}

void GroupChatMgr::PtcIssueCount(CRole* ptcRole)
{
    if(NULL == ptcRole) return;
    PtcM2C_GroupChatIssueCount ptc;
    ptc.m_Data.set_groupcount(GetCountZMGroup(ptcRole->GetID()));
    ptc.m_Data.set_rolecount(GetCountZMRole(ptcRole->GetID()));
    ptcRole->Send(ptc);
}

void GroupChatMgr::PtcApplyToLeader(UINT64 roleid)
{
    CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
    if(NULL == pRole) return;
    PtcM2C_GroupChatApply ptc;
    pRole->Send(ptc);
}

void GroupChatMgr::LoginRedPoint(UINT64 roleid)
{
    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(roleid);
    if(NULL == pRoleInfo) 
    {
        return;
    }
    for(auto itor = pRoleInfo->zmGroupIssueList.begin(); itor != pRoleInfo->zmGroupIssueList.end(); ++itor)
    {
        GroupIssueInfo* pZMGroupInfo = GetZMGroupInfo(itor->first);
        if(NULL == pZMGroupInfo) continue;
        if(pZMGroupInfo->zmApplyRoleidList.size()>0)
        {
            PtcApplyToLeader(roleid);
            return;
        }
    }
}

KKSG::ErrorCode GroupChatMgr::CreateGroup(CRole* pRole, const std::string& mGroupName, UINT32 mCreateType, UINT64& outGroupID)
{
    outGroupID = 0;
    if(NULL == pRole)return KKSG::ERR_INVALID_REQUEST;
    if(mGroupName.size() < 1 || mGroupName.size() > 100)
    {
        return KKSG::ERR_INVALID_REQUEST;
    }
    KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(mGroupName);
    if(nErrCode != KKSG::ERR_SUCCESS)
    {
        return nErrCode;
    }
    GroupChatRoleInfo& roleInfo = NewOrGetRoleInfo(pRole->GetID());
    if(IsFullRole(pRole->GetID()))
    {
        return KKSG::ERR_GROUPCHAT_FULLGROUP;
    }
    outGroupID = GenGroupID();
    UINT32 createtime = GetTime();
    m_TeamInfoList.insert(std::make_pair(outGroupID, GroupChatGroupInfo(outGroupID, mGroupName, mCreateType, createtime, pRole->GetID())));
    roleInfo.groupIDList.insert(outGroupID);
    LogInfo("GroupChat Create groupid=%llu, groupname=%s, roleid=%llu.\n", outGroupID, mGroupName.c_str(), pRole->GetID());
    NewDelMemberNotify(outGroupID, pRole->GetID(), true);
    m_isChangedGroup.push(outGroupID);
    m_isChangedRole.push(pRole->GetID());

	DoGroupChatLog(outGroupID, TGroupChatType1, pRole->GetID(), pRole->GetID());
	DoGroupChatLog(outGroupID, TGroupChatType2, pRole->GetID(), pRole->GetID());
    return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode GroupChatMgr::AddRole(CRole* pRole, UINT64 groupID, UINT64 addroleid)
{
    if(NULL == pRole) return KKSG::ERR_INVALID_REQUEST;
    if(pRole->GetID() == addroleid)
    {
        return KKSG::ERR_SUCCESS;
    }
    if(IsGroupMember(groupID, addroleid))
    {
        return KKSG::ERR_SUCCESS;
    }

    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo)return KKSG::ERR_GROUPCHAT_TIMEOUT;
    if(pGroupInfo->leaderRoleID != pRole->GetID()) 
    {
        return KKSG::ERR_GROUPCHAT_NO_MANAGER;
    }
    if(pGroupInfo->memberRoleIDList.size() >= MaxMemberCountInGroup)
    {
        return KKSG::ERR_GROUPCHAT_GROUPFULLMEMBERS;
    }
    GroupChatRoleInfo& addRoleInfo = NewOrGetRoleInfo(addroleid);
    if(IsFullRole(addroleid))
    {
        return KKSG::ERR_GROUPCHAT_FULLGROUP_OTHER;
    }

    CRoleSummary* pAddRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(addroleid);
    if(NULL == pAddRoleSummary)
    {
        return KKSG::ERR_INVALID_REQUEST;
    }

    UINT32 tmptime = GetTime();
    pGroupInfo->memberRoleIDList[addroleid] = tmptime;
    addRoleInfo.groupIDList.insert(groupID);
    NewDelMemberNotify(groupID, addroleid, true);

    for(auto itor = pGroupInfo->memberRoleIDList.begin(); itor != pGroupInfo->memberRoleIDList.end(); itor++)
    {
        CRole* pTmpRole = CRoleManager::Instance()->GetByRoleID(itor->first);
        PtcAddRole(pTmpRole, pGroupInfo, pAddRoleSummary, itor->second);
    }
    m_isChangedGroup.push(groupID);
    m_isChangedRole.push(addroleid);

	DoGroupChatLog(groupID, TGroupChatType2, pGroupInfo->leaderRoleID, addroleid);
    return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode GroupChatMgr::DismissGroup(UINT64 groupID, bool isSystemDismiss)
{
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo)return KKSG::ERR_GROUPCHAT_TIMEOUT;

	DoGroupChatLog(groupID, TGroupChatType5, pGroupInfo->leaderRoleID);
    for(auto itor = pGroupInfo->memberRoleIDList.begin(); itor != pGroupInfo->memberRoleIDList.end(); itor++)
    {
        GroupChatRoleInfo* pTmpRoleInfo = GetRoleInfo(itor->first);
        if(NULL == pTmpRoleInfo)continue;
        pTmpRoleInfo->groupIDList.erase(groupID);
        //NewDelMemberNotify(groupID, itor->first, false);

        CRole* pTmpRole = CRoleManager::Instance()->GetByRoleID(itor->first);
        PtcDismissGroup(pTmpRole, pGroupInfo, isSystemDismiss);
        m_isChangedRole.push(itor->first);
    }

    if(!isSystemDismiss)
    {
        m_TeamInfoList.erase(groupID);
        DelDBGroup(groupID);
    }
    return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode GroupChatMgr::QuitGroup(CRole* pRole, UINT64 groupID)
{
    if(NULL == pRole) return KKSG::ERR_INVALID_REQUEST;
    return _GroupDelRole(groupID, pRole->GetID(), true);
}

KKSG::ErrorCode GroupChatMgr::SubRole(CRole* pRole, UINT64 groupID, UINT64 subroleid)
{
    if(NULL == pRole) return KKSG::ERR_INVALID_REQUEST;
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo)return KKSG::ERR_GROUPCHAT_TIMEOUT;
    if(pGroupInfo->leaderRoleID != pRole->GetID()) 
    {
        return KKSG::ERR_GROUPCHAT_NO_MANAGER;
    }
    return _GroupDelRole(groupID, subroleid, false);
}

KKSG::ErrorCode GroupChatMgr::_GroupDelRole(UINT64 groupID, UINT64 delRoleid, bool isSelfQuit)
{
    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(groupID);
    if(NULL == pGroupInfo)return KKSG::ERR_GROUPCHAT_TIMEOUT;

	DoGroupChatLog(groupID, isSelfQuit?TGroupChatType3:TGroupChatType4, pGroupInfo->leaderRoleID, delRoleid);
    if(pGroupInfo->leaderRoleID == delRoleid) 
    {
        return DismissGroup(pGroupInfo->groupID, false);
    }

    GroupChatRoleInfo* pDelRoleInfo = GetRoleInfo(delRoleid);
    if(NULL != pDelRoleInfo)
    {
        pDelRoleInfo->groupIDList.erase(groupID);
    }

    for(auto itor = pGroupInfo->memberRoleIDList.begin(); itor != pGroupInfo->memberRoleIDList.end(); itor++)
    {
        CRole* pTmpRole = CRoleManager::Instance()->GetByRoleID(itor->first);
        PtcDelRole(pTmpRole, pGroupInfo, delRoleid, isSelfQuit);
    }

    pGroupInfo->memberRoleIDList.erase(delRoleid);
    NewDelMemberNotify(groupID, delRoleid, false);

    m_isChangedGroup.push(groupID);
    m_isChangedRole.push(delRoleid);
    return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode GroupChatMgr::ClearGroupChat(CRole* pRole)
{
    if(NULL == pRole) return KKSG::ERR_INVALID_REQUEST;
    UINT64 roleid = pRole->GetID();

    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(roleid);
    if(NULL == pRoleInfo)return KKSG::ERR_GROUPCHAT_TIMEOUT;
    pRoleInfo->timeLastClear = GetTime();
    m_isChangedRole.push(pRole->GetID());
    return KKSG::ERR_SUCCESS;
}

UINT64 GroupChatMgr::GenGroupID()
{
    // gid 64 位结构
    // [服ID]   [时间戳]  [自增长ID]
    //  FFFF   FFFFFFFF    FFFF
    UINT64 serverID = MSConfig::Instance()->GetServerID();
    UINT32 ServerTime = TimeUtil::GetTime();
    static UINT16 GROUPCHATID = 0;

    serverID = ((serverID << 32) | ServerTime);
    serverID = ((serverID << 16) | (++GROUPCHATID));

    LogInfo("GroupChatMgr newid: %llu, %u, %u", serverID, ServerTime, GROUPCHATID);
    return serverID;
}

GroupChatRoleInfo& GroupChatMgr::NewOrGetRoleInfo(UINT64 roleid)
{
    auto itor = m_RoleInfoList.find(roleid);
    if(m_RoleInfoList.end() == itor)
    {
        m_RoleInfoList.insert(std::make_pair(roleid, GroupChatRoleInfo(roleid)));
    }
    itor = m_RoleInfoList.find(roleid);
    return itor->second;
}

GroupChatRoleInfo* GroupChatMgr::GetRoleInfo(UINT64 roleid)
{
    auto itor = m_RoleInfoList.find(roleid);
    if(m_RoleInfoList.end() == itor)
    {
        return NULL;
    }
    return &(itor->second);
}

GroupChatGroupInfo* GroupChatMgr::GetGroupInfo(UINT64 groupID)
{
    auto itor = m_TeamInfoList.find(groupID);
    if(m_TeamInfoList.end() == itor)
    {
        return NULL;
    }
    return &(itor->second);
}

bool GroupChatMgr::IsFullRole(UINT64 roleid)
{
    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(roleid);
    if(NULL == pRoleInfo)
    {
        return false;
    }

    return GetRoleGroupList(*pRoleInfo).size() >= MaxGroupCountForRole;
}

std::set<UINT64>& GroupChatMgr::GetRoleGroupList(GroupChatRoleInfo& RoleInfo)
{
    for(auto itor = RoleInfo.groupIDList.begin(); itor != RoleInfo.groupIDList.end();)
    {
        bool delslag = false;
        GroupChatGroupInfo* pGroupInfo = GetGroupInfo(*itor);
        if(NULL == pGroupInfo)
        {
            delslag = true;
        }else{
            delslag = (pGroupInfo->memberRoleIDList.end() == pGroupInfo->memberRoleIDList.find(RoleInfo.roleid));
        }
        if(delslag) 
        {
            RoleInfo.groupIDList.erase(itor++);
            m_isChangedRole.push(RoleInfo.roleid);
        }else{
            itor++;
        }
    }
    return RoleInfo.groupIDList;
}

void GroupChatMgr::RpcZMFindTeamInfoList(CRole* pRole, const KKSG::GroupChatFindTeamInfoListC2S& roArg, KKSG::GroupChatFindTeamInfoListS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }

    UINT32 count = 0;
    for(auto itor = mTeamIssueList.rbegin(); itor != mTeamIssueList.rend(); ++itor)
    {
        bool tmpslag = false;
        if(0 == roArg.type())tmpslag = true;
        //if(0 == itor->second.showFightType)tmpslag = true;
        if(roArg.type() == itor->second.showFightType)tmpslag = true;
        if(!tmpslag)continue;
        if(itor->second.zmApplyRoleidList.size() >= MaxGroupApplyCount)
        {
            continue;
        }

        GroupChatGroupInfo* pGroupInfo = GetGroupInfo(itor->second.groupID);
        if(NULL == pGroupInfo)
        {
            continue;
        }
        bool isSelfInGroup = IsGroupMember(itor->second.groupID, pRole->GetID());
        bool mstate = (itor->second.zmApplyRoleidList.end() != itor->second.zmApplyRoleidList.find(pRole->GetID()));
        itor->second.FillProtoc(pGroupInfo->leaderRoleID, mstate?1:0, isSelfInGroup, *roRes.add_teamlist());
        count++;
        if(count > 20)
        {
            break;
        }
    }
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    LogInfo("GroupChatZM: Group Issue Num %d\n", count);
    PtcIssueCount(pRole);
}

void GroupChatMgr::RpcZMLeaderIssueInfo(CRole* pRole, const KKSG::GroupChatLeaderIssueInfoC2S& roArg, KKSG::GroupChatLeaderIssueInfoS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }

    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(pRole->GetID());
    if(NULL == pRoleInfo)
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
        return;
    }

    if(!IsGroupLeader(roArg.teaminfo().groupchatid(), pRole->GetID()))
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_NO_MANAGER);
        return;
    }
    
    if(IsFullZMGroup(pRole->GetID()))
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_LEADERNOCOUNT);
        return;
    }

    UINT64 index = GenGroupID();
    UINT32 issuetime = GetTime();
    mTeamIssueList.insert(std::make_pair(index, GroupIssueInfo(roArg.teaminfo(), issuetime, roArg.teaminfo().groupchatid(), index)));
    pRoleInfo->zmGroupIssueList[index] = issuetime;
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    m_isChangedGroupIssue.push(index);
    m_isChangedRole.push(pRole->GetID());

	DoIssueLog(pRole->GetID(), roArg.teaminfo());
}

void GroupChatMgr::RpcZMLeaderGetReviewList(CRole* pRole, KKSG::GroupChatLeaderReviewListS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }
    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(pRole->GetID());
    if(NULL == pRoleInfo) 
    {
        roRes.set_errorcode(KKSG::ERR_SUCCESS);
        return;
    }
    GetCountZMGroup(pRole->GetID());    //去掉过期的
    for(auto itor = pRoleInfo->zmGroupIssueList.begin(); itor != pRoleInfo->zmGroupIssueList.end();++itor)
    {
        GroupIssueInfo* pissueinfo = GetZMGroupInfo(itor->first);
        if(pissueinfo) pissueinfo->FillApplyProtoc(roRes);
    }
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void GroupChatMgr::RpcZMLeaderReview(CRole* pRole, const KKSG::GroupChatLeaderReviewC2S& roArg, KKSG::GroupChatLeaderReviewS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }

    if(!IsGroupLeader(roArg.groupchatid(), pRole->GetID()))
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_NO_MANAGER);
        return;
    }

    GroupIssueInfo* pZMGroupInfo = GetZMGroupInfo(roArg.issueindex());
    if(NULL == pZMGroupInfo)
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
        return;
    }

    auto itor = pZMGroupInfo->zmApplyRoleidList.find(roArg.roleid());
    if(pZMGroupInfo->zmApplyRoleidList.end() == itor)
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
        return;
    }

    KKSG::ErrorCode ec = KKSG::ERR_SUCCESS;
    if(roArg.isagree())
    {
        ec = AddRole(pRole, roArg.groupchatid(), roArg.roleid());
    }
    if(KKSG::ERR_SUCCESS == ec)
    {
        pZMGroupInfo->zmApplyRoleidList.erase(itor);
        m_isChangedGroupIssue.push(roArg.groupchatid());
    }
    roRes.set_errorcode(ec);
}

void GroupChatMgr::RpcZMRoleApply(CRole* pRole, const KKSG::GroupChatPlayerApplyC2S& roArg, KKSG::GroupChatPlayerApplyS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }

    GroupIssueInfo* pZMGroupInfo = GetZMGroupInfo(roArg.issueindex());
    if(NULL == pZMGroupInfo)
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
        return;
    }

    if(pZMGroupInfo->zmApplyRoleidList.size() >= MaxGroupApplyCount)
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_TIMEOUT);
        return;
    }

    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    pZMGroupInfo->zmApplyRoleidList.insert(pRole->GetID());
    m_isChangedGroupIssue.push(roArg.issueindex());

    GroupChatGroupInfo* pGroupInfo = GetGroupInfo(pZMGroupInfo->groupID);
    if(pGroupInfo)
    {
        PtcApplyToLeader(pGroupInfo->leaderRoleID);
    }
}

void GroupChatMgr::RpcZMFindRoleInfoList(CRole* pRole, const KKSG::GroupChatFindRoleInfoListC2S& roArg, KKSG::GroupChatFindRoleInfoListS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }

    UINT32 count = 0;
    for(auto itor = mRoleIssueList.rbegin(); itor != mRoleIssueList.rend();itor++)
    {
        bool tmpslag = false;
        if(0 == roArg.type())tmpslag = true;
        //if(0 == itor->second.showFightType)tmpslag = true;
        if(roArg.type() == itor->second.showFightType)tmpslag = true;
        if(!tmpslag)continue;
        //if(pRole->GetID() == itor->second.roleid)
        //{
        //    continue;
        //}
        itor->second.FillProtoc(*roRes.add_rolelist());
        count++;
        if(count > 20)
        {
            break;
        }
    }
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    LogInfo("GroupChatZM: Role Issue Num %d\n", count);
    PtcIssueCount(pRole);
}

void GroupChatMgr::RpcZMRoleIssueInfo(CRole* pRole, const KKSG::GroupChatPlayerIssueInfoC2S& roArg, KKSG::GroupChatPlayerIssueInfoS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }
    if(IsFullZMRole(pRole->GetID()))
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_ROLENOCOUNT);
        return;
    }

    UINT64 index = GenGroupID();
    UINT32 issuetime = GetTime();
    mRoleIssueList.insert(std::make_pair(index, RoleIssueInfo(roArg.roleinfo(), issuetime, pRole->GetID(), index)));
    GroupChatRoleInfo& roleinfo = NewOrGetRoleInfo(pRole->GetID());
    roleinfo.zmRoleIssueList[index] = issuetime;
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    m_isChangedRoleIssue.push(index);
    m_isChangedRole.push(pRole->GetID());
}

void GroupChatMgr::RpcZMLeadAddRole(CRole* pRole, const KKSG::GroupChatLeaderAddRoleC2S& roArg, KKSG::GroupChatLeaderAddRoleS2C& roRes)
{
    if(NULL == pRole) 
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }

    if(!IsGroupLeader(roArg.groupchatid(), pRole->GetID()))
    {
        roRes.set_errorcode(KKSG::ERR_GROUPCHAT_NO_MANAGER);
        return;
    }

    KKSG::ErrorCode ec = AddRole(pRole, roArg.groupchatid(), roArg.roleid());
    if(KKSG::ERR_SUCCESS == ec)
    {
        mRoleIssueList.erase(roArg.roleissueindex());
        DelDBRoleIssue(roArg.roleissueindex());
    }
    roRes.set_errorcode(ec);
}

GroupIssueInfo* GroupChatMgr::GetZMGroupInfo(UINT64 index)
{
    auto itor = mTeamIssueList.find(index);
    if(mTeamIssueList.end() == itor)
    {
        return NULL;
    }
    return &(itor->second);
}

RoleIssueInfo* GroupChatMgr::GetZMRoleInfo(UINT64 index)
{
    auto itor = mRoleIssueList.find(index);
    if(mRoleIssueList.end() == itor)
    {
        return NULL;
    }
    return &(itor->second);
}

UINT32 GroupChatMgr::GetCountZMGroup(UINT64 roleid)
{
    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(roleid);
    if(NULL == pRoleInfo)
    {
        return false;
    }
    UINT32 nowtime = GetTime();
    for(auto itor = pRoleInfo->zmGroupIssueList.begin(); itor != pRoleInfo->zmGroupIssueList.end();)
    {
        if(!IsSameDay(nowtime, itor->second))
        {
            pRoleInfo->zmGroupIssueList.erase(itor++);
            m_isChangedRole.push(roleid);
        }else{
            itor++;
            break;
        }
    }
    return pRoleInfo->zmGroupIssueList.size();
}

UINT32 GroupChatMgr::GetCountZMRole(UINT64 roleid)
{
    GroupChatRoleInfo* pRoleInfo = GetRoleInfo(roleid);
    if(NULL == pRoleInfo)
    {
        return false;
    }
    UINT32 nowtime = GetTime();
    for(auto itor = pRoleInfo->zmRoleIssueList.begin(); itor != pRoleInfo->zmRoleIssueList.end();)
    {
        if(!IsSameDay(nowtime, itor->second))
        {
            pRoleInfo->zmRoleIssueList.erase(itor++);
            m_isChangedRole.push(roleid);
        }else{
            itor++;
            break;
        }
    }
    return pRoleInfo->zmRoleIssueList.size();
}

bool GroupChatMgr::IsFullZMGroup(UINT64 roleid)
{
    return GetCountZMGroup(roleid) >= MaxZMGroupIssue;
}

bool GroupChatMgr::IsFullZMRole(UINT64 roleid)
{
    return GetCountZMRole(roleid) >= MaxZMRoleIssue;
}

RoleIssueInfo::RoleIssueInfo(const KKSG::GroupChatFindRoleInfo& roleinfo, UINT32 missuetime, UINT64 mroleid, UINT64 mindex)
{
    roleid = mroleid;
    index = mindex;
    showStageID = roleinfo.stageid();
    showFighting = roleinfo.fighting();
    showFightType = roleinfo.type();
    showTime = roleinfo.time();
    state = roleinfo.state();
    issueTime = missuetime;
}

RoleIssueInfo::RoleIssueInfo(const KKSG::DBGroupChatSaveRoleIssue& pDBInfo)
{
    roleid = pDBInfo.roleid();
    index = pDBInfo.index();
    showStageID = pDBInfo.showstageid();
    showFighting = pDBInfo.showfighting();
    showFightType = pDBInfo.showtype();
    showTime = pDBInfo.showtime();
    state = pDBInfo.state();
    issueTime = pDBInfo.time();
}

void RoleIssueInfo::FillDBProtoc(KKSG::DBGroupChatSaveRoleIssue& pDBInfo)
{
    pDBInfo.set_roleid(roleid);
    pDBInfo.set_index(index);
    pDBInfo.set_showstageid(showStageID);
    pDBInfo.set_showfighting(showFighting);
    pDBInfo.set_showtype(showFightType);
    pDBInfo.set_showtime(showTime);
    pDBInfo.set_state(state);
    pDBInfo.set_time(issueTime);
}

void RoleIssueInfo::FillProtoc(KKSG::GroupChatFindRoleInfo& roleinfo)
{
    roleinfo.set_stageid(showStageID);
    roleinfo.set_fighting(showFighting);
    roleinfo.set_type(showFightType);
    roleinfo.set_time(showTime);
    roleinfo.set_state(state);
    roleinfo.set_issuetime(issueTime);

    roleinfo.set_roleid(roleid);
    roleinfo.set_issueindex(index);

    CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
    if(pRoleSummary)
    {
        roleinfo.set_rolename(pRoleSummary->GetName());
        roleinfo.set_roleprofession(pRoleSummary->GetProfession());
    }
}

GroupIssueInfo::GroupIssueInfo(const KKSG::GroupChatFindTeamInfo& teaminfo, UINT32 missuetime, UINT64 mgroupid, UINT64 mindex)
{
    groupID = mgroupid;
    index = mindex;
    showStageID = teaminfo.stageid();
    showFighting = teaminfo.fighting();
    showFightType = teaminfo.type();
    showTime = teaminfo.time();
    state = teaminfo.state();
    issueTime = missuetime;
}

GroupIssueInfo::GroupIssueInfo(const KKSG::DBGroupChatSaveGroupIssue& pDBInfo)
{
    groupID = pDBInfo.groupid();
    index = pDBInfo.index();
    showStageID = pDBInfo.showstageid();
    showFighting = pDBInfo.showfighting();
    showFightType = pDBInfo.showfighttype();
    showTime = pDBInfo.showtime();
    state = pDBInfo.state();
    issueTime = pDBInfo.time();
    for(int i = 0; i < pDBInfo.zmapplyroleidlist().size(); ++i)
    {
        zmApplyRoleidList.insert(pDBInfo.zmapplyroleidlist(i));
    }
}

void GroupIssueInfo::FillDBProtoc(KKSG::DBGroupChatSaveGroupIssue& pDBInfo)
{
    pDBInfo.set_groupid(groupID);
    pDBInfo.set_index(index);
    pDBInfo.set_showstageid(showStageID);
    pDBInfo.set_showfighting(showFighting);
    pDBInfo.set_showfighttype(showFightType);
    pDBInfo.set_showtime(showTime);
    pDBInfo.set_state(state);
    pDBInfo.set_time(issueTime);
    for(auto itor = zmApplyRoleidList.begin(); itor != zmApplyRoleidList.end(); ++itor)
    {
        pDBInfo.add_zmapplyroleidlist(*itor);
    }
}

void GroupIssueInfo::FillProtoc(UINT64 leaderid, UINT32 mstate, bool isSelfInGroup, KKSG::GroupChatFindTeamInfo& teaminfo)
{
    teaminfo.set_stageid(showStageID);
    teaminfo.set_fighting(showFighting);
    teaminfo.set_type(showFightType);
    teaminfo.set_time(showTime);
    teaminfo.set_state(mstate);
    teaminfo.set_issuetime(issueTime);

    teaminfo.set_groupchatid(groupID);
    teaminfo.set_groupchatname(GroupChatGetGroupName(groupID));
    teaminfo.set_issueindex(index);
    teaminfo.set_leaderroleid(leaderid);
    teaminfo.set_isselfingroup(isSelfInGroup);
}

void GroupIssueInfo::FillApplyProtoc(KKSG::GroupChatLeaderReviewListS2C& roRes)
{
    for(auto itor = zmApplyRoleidList.begin(); itor != zmApplyRoleidList.end(); ++itor)
    {
        // TODO if()continue;
        CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(*itor);
        if(NULL == pRoleSummary) continue;
        KKSG::GroupChatFindRoleInfo* tmp = roRes.add_roleinfolist();
        if(NULL == tmp)
        {
            LogWarn("Out of Memory.\n");
            return;
        }
        tmp->set_roleid(*itor);
        tmp->set_rolename(pRoleSummary->GetName());
        tmp->set_groupchatid(groupID);
        tmp->set_groupchatname(GroupChatGetGroupName(groupID));
        tmp->set_issueindex(index);

        tmp->set_stageid(showStageID);
        tmp->set_fighting(showFighting);
        tmp->set_type(showFightType);
        tmp->set_time(showTime);
        tmp->set_state(state);
        tmp->set_issuetime(issueTime);
    }
}

GroupChatRoleInfo::GroupChatRoleInfo(const KKSG::DBGroupChatSaveRole& pDBInfo)
{
    roleid = pDBInfo.index();
    timeLastClear = pDBInfo.time();
    for(int i = 0; i < pDBInfo.groupidlist().size(); ++i)
    {
        groupIDList.insert(pDBInfo.groupidlist(i));
    }

    for(int i = 0; i < pDBInfo.zmroleissuelist().size(); ++i)
    {
        zmRoleIssueList[pDBInfo.zmroleissuelist(i).id()] = pDBInfo.zmroleissuelist(i).time();
    }
    for(int i = 0; i < pDBInfo.zmgroupissuelist().size(); ++i)
    {
        zmGroupIssueList[pDBInfo.zmgroupissuelist(i).id()] = pDBInfo.zmgroupissuelist(i).time();
    }
}

void GroupChatRoleInfo::FillDBProtoc(KKSG::DBGroupChatSaveRole& pDBInfo)
{
    pDBInfo.set_index(roleid);
    pDBInfo.set_time(timeLastClear);
    for(auto itor = groupIDList.begin(); itor != groupIDList.end(); ++itor)
    {
        pDBInfo.add_groupidlist(*itor);
    }

    for(auto itor = zmRoleIssueList.begin(); itor != zmRoleIssueList.end(); ++itor)
    {
        auto tmp = pDBInfo.add_zmroleissuelist();
        tmp->set_id(itor->first);
        tmp->set_time(itor->second);
    }
    for(auto itor = zmGroupIssueList.begin(); itor != zmGroupIssueList.end(); ++itor)
    {
        auto tmp = pDBInfo.add_zmgroupissuelist();
        tmp->set_id(itor->first);
        tmp->set_time(itor->second);
    }
}

bool GroupChatMgr::QueryDBGroupIssue()
{
    {
        IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
        std::stringstream ss;
        ss << "delete from groupchatgroupissue where `ctime` < "  << GetTodayBeginTime();
        if(!poMySqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
        {
            LogError("Table groupchatgroupissue Clear failed, errorMsg: %s", poMySqlConn->GetError());
            return false;
        }
    }

    {
        IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
        std::stringstream ss;
        ss << "select `_id`, `data` from groupchatgroupissue";
        IMysqlRecordSet* poRes = NULL;
        if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
        {
            LogError("Table groupchatgroupissue Read failed, errorMsg: %s", poMySqlConn->GetError());
            return false;
        }

        KKSG::DBGroupChatSaveGroupIssue dbData;

        while(poRes->FetchRow())
        {
            char* pVal = NULL;
            UINT32 dwLen = 0;

            UINT64 index;
            if(poRes->GetFieldValue(0, &pVal, dwLen))
            {
                index = convert<UINT64>(pVal);
            }
            else
            {
                LogError("Read 'index' column failed");
                return false;
            }

            if(!poRes->GetFieldValue(1, &pVal, dwLen))
            {
                LogError("Read 'data' column failed");
                return false;
            }

            if(!dbData.ParseFromArray(pVal, dwLen))
            {
                LogError("Parse data failed, index:%d\n", index);
                return false;
            }

            mTeamIssueList.insert(std::make_pair(index, GroupIssueInfo(dbData)));
        }
        poRes->Release();
    }
    return true;
}

bool GroupChatMgr::QueryDBRoleIssue()
{
    {
        IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
        std::stringstream ss;
        ss << "delete from groupchatroleissue where `ctime`<"  << GetTodayBeginTime();
        if(!poMySqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
        {
            LogError("Table groupchatroleissue Clear failed, errorMsg: %s", poMySqlConn->GetError());
            return false;
        }
    }

    {
        IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
        std::stringstream ss;
        ss << "select `_id`, `data` from groupchatroleissue";
        IMysqlRecordSet* poRes = NULL;
        if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
        {
            LogError("Table groupchatroleissue Read failed, errorMsg: %s", poMySqlConn->GetError());
            return false;
        }

        KKSG::DBGroupChatSaveRoleIssue dbData;

        while(poRes->FetchRow())
        {
            char* pVal = NULL;
            UINT32 dwLen = 0;

            UINT64 index;
            if(poRes->GetFieldValue(0, &pVal, dwLen))
            {
                index = convert<UINT64>(pVal);
            }
            else
            {
                LogError("Read 'index' column failed");
                return false;
            }

            if(!poRes->GetFieldValue(1, &pVal, dwLen))
            {
                LogError("Read 'data' column failed");
                return false;
            }

            if(!dbData.ParseFromArray(pVal, dwLen))
            {
                LogError("Parse data failed, index:%d\n", index);
                return false;
            }

            mRoleIssueList.insert(std::make_pair(index, RoleIssueInfo(dbData)));
        }
        poRes->Release();
    }
    return true;
}

bool GroupChatMgr::QueryDBGroup()
{
    {
        IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
        std::stringstream ss;
        ss << "delete from groupchatgroup where `ctime`<"  << (GetTime()-MaxGroupLifeTime);
        if(!poMySqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
        {
            LogError("Table groupchatgroup Clear failed, errorMsg: %s", poMySqlConn->GetError());
            return false;
        }
    }

    {
        IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
        std::stringstream ss;
        ss << "select `_id`, `data` from groupchatgroup";
        IMysqlRecordSet* poRes = NULL;
        if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
        {
            LogError("Table groupchatgroup Read failed, errorMsg: %s", poMySqlConn->GetError());
            return false;
        }

        KKSG::DBGroupChatSaveGroup dbData;

        while(poRes->FetchRow())
        {
            char* pVal = NULL;
            UINT32 dwLen = 0;

            UINT64 index;
            if(poRes->GetFieldValue(0, &pVal, dwLen))
            {
                index = convert<UINT64>(pVal);
            }
            else
            {
                LogError("Read 'index' column failed");
                return false;
            }

            if(!poRes->GetFieldValue(1, &pVal, dwLen))
            {
                LogError("Read 'data' column failed");
                return false;
            }

            if(!dbData.ParseFromArray(pVal, dwLen))
            {
                LogError("Parse data failed, index:%d\n", index);
                return false;
            }

            m_TeamInfoList.insert(std::make_pair(index, GroupChatGroupInfo(dbData)));
        }
        poRes->Release();
    }
    return true;
}

bool GroupChatMgr::QueryDBRole()
{
    {
        IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
        std::stringstream ss;
        ss << "select `_id`, `data` from groupchatrole";
        IMysqlRecordSet* poRes = NULL;
        if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
        {
            LogError("Table groupchatrole Read failed, errorMsg: %s", poMySqlConn->GetError());
            return false;
        }

        KKSG::DBGroupChatSaveRole dbData;

        while(poRes->FetchRow())
        {
            char* pVal = NULL;
            UINT32 dwLen = 0;

            UINT64 index;
            if(poRes->GetFieldValue(0, &pVal, dwLen))
            {
                index = convert<UINT64>(pVal);
            }
            else
            {
                LogError("Read 'index' column failed");
                return false;
            }

            if(!poRes->GetFieldValue(1, &pVal, dwLen))
            {
                LogError("Read 'data' column failed");
                return false;
            }

            if(!dbData.ParseFromArray(pVal, dwLen))
            {
                LogError("Parse data failed, index:%d\n", index);
                return false;
            }

            m_RoleInfoList.insert(std::make_pair(index, GroupChatRoleInfo(dbData)));
        }
        poRes->Release();
    }
    return true;
}

bool GroupChatMgr::SaveDBGroupIssueID(UINT64 index)
{
    GroupIssueInfo* pTmp = GetZMGroupInfo(index);
    if(NULL == pTmp) return false;
    return SaveDBGroupIssue(*pTmp);
}

bool GroupChatMgr::SaveDBRoleIssueID(UINT64 index)
{
    RoleIssueInfo* pTmp = GetZMRoleInfo(index);
    if(NULL == pTmp) return false;
    return SaveDBRoleIssue(*pTmp);
}

bool GroupChatMgr::SaveDBGroupID(UINT64 groupID)
{
    GroupChatGroupInfo* pTmp = GetGroupInfo(groupID);
    if(NULL == pTmp) return false;
    return SaveDBGroup(*pTmp);
}

bool GroupChatMgr::SaveDBRoleID(UINT64 roleid)
{
    GroupChatRoleInfo* pTmp = GetRoleInfo(roleid);
    if(NULL == pTmp) return false;
    return SaveDBRole(*pTmp);
}

bool GroupChatMgr::SaveDBGroupIssue(GroupIssueInfo& info)
{
    PtcM2D_DBGroupChatSaveGroupIssueNtf ptc;
    info.FillDBProtoc(ptc.m_Data);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::SaveDBRoleIssue(RoleIssueInfo& info)
{
    PtcM2D_DBGroupChatSaveRoleIssueNtf ptc;
    info.FillDBProtoc(ptc.m_Data);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::SaveDBGroup(GroupChatGroupInfo& info)
{
    PtcM2D_DBGroupChatSaveGroupNtf ptc;
    info.FillDBProtoc(ptc.m_Data);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::SaveDBRole(GroupChatRoleInfo& info)
{
    PtcM2D_DBGroupChatSaveRoleNtf ptc;
    info.FillDBProtoc(ptc.m_Data);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::DelDBGroupIssue(UINT64 index)
{
    PtcM2D_DBGroupChatDelGroupIssueNtf ptc;
    ptc.m_Data.set_index(index);
    ptc.m_Data.set_isclear(false);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::DelDBRoleIssue(UINT64 index)
{
    PtcM2D_DBGroupChatDelRoleIssueNtf ptc;
    ptc.m_Data.set_index(index);
    ptc.m_Data.set_isclear(false);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::DelDBGroup(UINT64 groupID)
{
    PtcM2D_DBGroupChatDelGroupNtf ptc;
    ptc.m_Data.set_groupid(groupID);
    ptc.m_Data.set_isclear(false);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::DelDBRole(UINT64 roleid)
{
    PtcM2D_DBGroupChatDelRoleNtf ptc;
    ptc.m_Data.set_roleid(roleid);
    ptc.m_Data.set_isclear(false);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::ClearDBGroupIssue()
{
    PtcM2D_DBGroupChatDelGroupIssueNtf ptc;
    ptc.m_Data.set_todaybegintime(GetTodayBeginTime());
    ptc.m_Data.set_isclear(true);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::ClearDBRoleIssue()
{
    PtcM2D_DBGroupChatDelRoleIssueNtf ptc;
    ptc.m_Data.set_todaybegintime(GetTodayBeginTime());
    ptc.m_Data.set_isclear(true);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

bool GroupChatMgr::ClearDBGroup()
{
    PtcM2D_DBGroupChatDelGroupNtf ptc;
    ptc.m_Data.set_todaybegintime(GetTime()-MaxGroupLifeTime);
    ptc.m_Data.set_isclear(true);
    DBLink::Instance()->SendTo(ptc);
    return true;
}

void GroupChatMgr::DoIssueLog(UINT64 roleid, const KKSG::GroupChatFindTeamInfo& teaminfo)
{
	TIssueFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleid;
	oLog.m_GroupID = teaminfo.groupchatid();
	oLog.m_showStageID = teaminfo.stageid();
	oLog.m_showFighting = teaminfo.fighting();
	oLog.m_showFightType = teaminfo.type();
	oLog.m_showTime = teaminfo.time();
	oLog.Do();
}

void  GroupChatMgr::DoGroupChatLog(UINT64 groupID, UINT32 op, UINT64 leaderID, UINT64 roleid)
{
	TChatGroupFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_LeaderID = leaderID;
	oLog.m_uRoleID = roleid;
	oLog.m_GroupID = groupID;
	oLog.m_Op = op;
	oLog.Do();
}
