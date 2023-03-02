#ifndef __GROUPCHAT_MGR_H__
#define __GROUPCHAT_MGR_H__

#include "pb/project.pb.h"
#include "role/rolesummary.h"
#include "util/uniquequeue.h"

class CRole;
struct RoleIssueInfo
{
    UINT64 roleid;
    UINT64 index;
    UINT32 showStageID;
    UINT32 showFighting;
    UINT32 showFightType;
    UINT32 showTime;
    UINT32 state;
    UINT32 issueTime;
    RoleIssueInfo(const KKSG::GroupChatFindRoleInfo& roleinfo, UINT32 missuetime, UINT64 mroleid, UINT64 mindex);
    void FillProtoc(KKSG::GroupChatFindRoleInfo& roleinfo);
    RoleIssueInfo(const KKSG::DBGroupChatSaveRoleIssue& pDBInfo);
    void FillDBProtoc(KKSG::DBGroupChatSaveRoleIssue& pDBInfo);
};

struct GroupIssueInfo
{
    UINT64 groupID;
    UINT64 index;
    UINT32 showStageID;
    UINT32 showFighting;
    UINT32 showFightType;
    UINT32 showTime;
    UINT32 state;
    UINT32 issueTime;
    std::set<UINT64> zmApplyRoleidList;

    GroupIssueInfo(const KKSG::GroupChatFindTeamInfo& teaminfo, UINT32 missuetime, UINT64 mgroupid, UINT64 mindex);
    void FillProtoc(UINT64 leaderid, UINT32 mstate, bool isSelfInGroup, KKSG::GroupChatFindTeamInfo& teaminfo);
    void FillApplyProtoc(KKSG::GroupChatLeaderReviewListS2C& roRes);
    GroupIssueInfo(const KKSG::DBGroupChatSaveGroupIssue& pDBInfo);
    void FillDBProtoc(KKSG::DBGroupChatSaveGroupIssue& pDBInfo);
};

struct GroupChatRoleInfo
{
    UINT64 roleid;
    UINT32 timeLastClear;
    std::set<UINT64> groupIDList;
    std::map<UINT64, UINT32> zmRoleIssueList;
    std::map<UINT64, UINT32> zmGroupIssueList;

    GroupChatRoleInfo(UINT64 mRoleid)
        : roleid(mRoleid), timeLastClear(0)
    {
    }
    GroupChatRoleInfo(const KKSG::DBGroupChatSaveRole& pDBInfo);
    void FillDBProtoc(KKSG::DBGroupChatSaveRole& pDBInfo);
};

struct GroupChatGroupInfo
{
    UINT64 groupID;
    std::string groupName;
    UINT32 createtype;
    UINT32 createtime;
    UINT32 lastchattime;
    UINT64 leaderRoleID;
    std::map<UINT64, UINT32> memberRoleIDList;

    GroupChatGroupInfo(UINT64 mGroupID, const std::string& mGroupName, UINT32 mCreateType, UINT32 mCreateTime, UINT64 mLeaderRoleID); 
    void FillProtoc(KKSG::GroupChatTeamInfo* teaminfo);
    GroupChatGroupInfo(const KKSG::DBGroupChatSaveGroup& pDBInfo);
    void FillDBProtoc(KKSG::DBGroupChatSaveGroup& pDBInfo);
};

class GroupChatMgr : public ITimer
{
    GroupChatMgr();
    ~GroupChatMgr();
    DECLARE_SINGLETON(GroupChatMgr);
public:
    bool Init();
    void Uninit();

    virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
    bool UpdateOnDayPass();

    UINT32 GetTime();
    UINT32 GetTodayBeginTime();
    bool IsSameDay(UINT32 time1, UINT32 time2);

private:
    UINT32 m_dayChange;

public: 
    bool IsGroupLeader(UINT64 groupID, UINT64 roleid);
    bool IsGroupMember(UINT64 groupID, UINT64 roleid);
    UINT32 GetGroupMemberJoinTime(UINT64 groupID, UINT64 roleid);
    void FillRoleInfo(KKSG::GroupChatPlayerInfo* pRoleInfo, CRoleSummary* pRoleSummary, UINT32 mTime);
    void NewDelMemberNotify(UINT64 groupID, UINT64 roleid, bool isNew);
    void SendGroupChat(UINT64 groupID, const KKSG::ChatInfo& chatinfo);
    bool RpcRoleGetGroupInfo(CRole* pRole, UINT64 groupID, KKSG::GroupChatGetGroupInfoS2C& roRes);
    bool RpcChangeGroupName(CRole* pRole, const KKSG::GroupChatChangeNameC2S& roArg, KKSG::GroupChatChangeNameS2C& roRes);
    void PtcDismissGroup(CRole* ptcRole, GroupChatGroupInfo* pGroupInfo, bool isSystemDismiss);
    void PtcDelRole(CRole* ptcRole, GroupChatGroupInfo* pGroupInfo, UINT64 delRoleid, bool isSelfQuit);
    void PtcAddRole(CRole* ptcRole, GroupChatGroupInfo* pGroupInfo, CRoleSummary* pAddRoleSummary, UINT32 mTime);
    void PtcIssueCount(CRole* ptcRole);
    void PtcApplyToLeader(UINT64 roleid);
    void LoginRedPoint(UINT64 roleid);

    KKSG::ErrorCode CreateGroup(CRole* pRole, const std::string& mGroupName, UINT32 mCreateType, UINT64& outGroupID);
    KKSG::ErrorCode AddRole(CRole* pRole, UINT64 groupID, UINT64 addroleid);
    KKSG::ErrorCode DismissGroup(UINT64 groupID, bool isSystemDismiss);
    KKSG::ErrorCode QuitGroup(CRole* pRole, UINT64 groupID);
    KKSG::ErrorCode SubRole(CRole* pRole, UINT64 groupID, UINT64 subroleid);
    KKSG::ErrorCode _GroupDelRole(UINT64 groupID, UINT64 delRoleid, bool isSelfQuit);
    KKSG::ErrorCode ClearGroupChat(CRole* pRole);
    GroupChatRoleInfo& NewOrGetRoleInfo(UINT64 roleid);
    GroupChatRoleInfo* GetRoleInfo(UINT64 roleid);
    GroupChatGroupInfo* GetGroupInfo(UINT64 groupID);
    bool IsFullRole(UINT64 roleid);
    std::set<UINT64>& GetRoleGroupList(GroupChatRoleInfo& RoleInfo);
	void DoGroupChatLog(UINT64 groupID, UINT32 op, UINT64 leaderID, UINT64 roleid=0);
private:
    UINT64 GenGroupID();
    std::unordered_map<UINT64, GroupChatRoleInfo> m_RoleInfoList;
    std::unordered_map<UINT64, GroupChatGroupInfo> m_TeamInfoList;
    UINT32 MaxMemberCountInGroup;
    UINT32 MaxGroupCountForRole;

public:
    void RpcZMFindTeamInfoList(CRole* pRole, const KKSG::GroupChatFindTeamInfoListC2S& roArg, KKSG::GroupChatFindTeamInfoListS2C& roRes);
    void RpcZMLeaderIssueInfo(CRole* pRole, const KKSG::GroupChatLeaderIssueInfoC2S& roArg, KKSG::GroupChatLeaderIssueInfoS2C& roRes);
    void RpcZMLeaderGetReviewList(CRole* pRole, KKSG::GroupChatLeaderReviewListS2C& roRes);
    void RpcZMLeaderReview(CRole* pRole, const KKSG::GroupChatLeaderReviewC2S& roArg, KKSG::GroupChatLeaderReviewS2C& roRes);
    void RpcZMRoleApply(CRole* pRole, const KKSG::GroupChatPlayerApplyC2S& roArg, KKSG::GroupChatPlayerApplyS2C& roRes);

    void RpcZMFindRoleInfoList(CRole* pRole, const KKSG::GroupChatFindRoleInfoListC2S& roArg, KKSG::GroupChatFindRoleInfoListS2C& roRes);
    void RpcZMRoleIssueInfo(CRole* pRole, const KKSG::GroupChatPlayerIssueInfoC2S& roArg, KKSG::GroupChatPlayerIssueInfoS2C& roRes);
    void RpcZMLeadAddRole(CRole* pRole, const KKSG::GroupChatLeaderAddRoleC2S& roArg, KKSG::GroupChatLeaderAddRoleS2C& roRes);

    GroupIssueInfo* GetZMGroupInfo(UINT64 index);
    RoleIssueInfo* GetZMRoleInfo(UINT64 index);

    UINT32 GetCountZMGroup(UINT64 roleid);
    UINT32 GetCountZMRole(UINT64 roleid);
    bool IsFullZMGroup(UINT64 roleid);
    bool IsFullZMRole(UINT64 roleid);

	void DoIssueLog(UINT64 roleid, const KKSG::GroupChatFindTeamInfo& teaminfo);

private:
    UINT32 MaxZMGroupIssue;
    UINT32 MaxZMRoleIssue;
    std::map<UINT64, GroupIssueInfo> mTeamIssueList;
    std::map<UINT64, RoleIssueInfo> mRoleIssueList;

public:
    bool QueryDBGroupIssue();
    bool QueryDBRoleIssue();
    bool QueryDBGroup();
    bool QueryDBRole();

    bool SaveDBGroupIssueID(UINT64 index);
    bool SaveDBRoleIssueID(UINT64 index);
    bool SaveDBGroupID(UINT64 groupID);
    bool SaveDBRoleID(UINT64 roleid);

    bool SaveDBGroupIssue(GroupIssueInfo& info);
    bool SaveDBRoleIssue(RoleIssueInfo& info);
    bool SaveDBGroup(GroupChatGroupInfo& info);
    bool SaveDBRole(GroupChatRoleInfo& info);

    bool DelDBGroupIssue(UINT64 index);
    bool DelDBRoleIssue(UINT64 index);
    bool DelDBGroup(UINT64 groupID);
    bool DelDBRole(UINT64 roleid);

    bool ClearDBGroupIssue();
    bool ClearDBRoleIssue();
    bool ClearDBGroup();

private:
    UINT32 MaxGroupLifeTime;
    UINT32 MaxGroupApplyCount;

private:
    UniqueQueue<UINT64> m_isChangedGroup;
    UniqueQueue<UINT64> m_isChangedRole;
    UniqueQueue<UINT64> m_isChangedGroupIssue;
    UniqueQueue<UINT64> m_isChangedRoleIssue;
    UINT32 m_saveToDBCount;
    HTIMER m_timeHandler;
};

#endif
