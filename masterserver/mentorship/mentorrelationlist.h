#ifndef __MENTORRELATIONLIST_H__
#define __MENTORRELATIONLIST_H__

#define MENTORRELATION_CURMAXNUM 2
#define MENTORRELATION_OFFLINEUNPUNISHTIME 3*86400
#define MENTORRELATION_PUNISHTIME (86400*GetGlobalConfig().MentorPunishDay)
#define MENTORRELATION_INTERVALTIME 86400*GetGlobalConfig().MentorPeriodDay
#define MENTORRELATION_INTERVALTIMENUM 2
#define MENTORRELATION_BREAKCONFIRMTIME GetGlobalConfig().MentorBreakConfirmTime

class MentorShipOneSide;
namespace KKSG
{
	class OneMentorRelationInfo2Client;
}

bool IsValidPos(int pos);

struct STMentorRelationStatus
{
	STMentorRelationStatus()
	{
		memset(this, 0, sizeof(*this));
	}

	int m_status;
	int m_time;
	int m_beApplyRoleOnlineTime;//break时计算breakapply 时间，超过3天则为0，否则为真实值
	UINT64 m_applierID;
};

struct OneMentorRelation
{
	OneMentorRelation()
	{
		m_roleID = 0;
		m_pos = 0;
		m_curStatus = 0;
		m_inheritTime = 0;
		m_ApplyHeritTime = 0;
		m_applyInheritRoleID = 0;
		m_logoutTime = 0;
	}

	void InitCurStatus();
	int GetCurStatus() const;
	int GetPos() const;
	bool IsRelationInStatus() const;
	int GetInheritTime();

	const STMentorRelationStatus* GetStatusData(int status) const;

	bool Convert2ClientOneRelation(MentorShipOneSide* self, KKSG::OneMentorRelationInfo2Client& relation);

	bool CanInherit() const;
	bool ApplyInherit(UINT64 applierID);
	bool Inherit(int time);

	void DebugClearInherit();

	UINT64 m_roleID;
	int m_pos;//该m_roleID的位置
	int m_curStatus;
	int m_inheritTime;
	int m_ApplyHeritTime;
	int m_logoutTime;
	UINT64 m_applyInheritRoleID;
	std::map<int, STMentorRelationStatus> m_statusMap;
};

class MentorRelationList
{
public:
	MentorRelationList(MentorShipOneSide* pOneSide);
	~MentorRelationList();

public:
	bool Init();
	void Uninit();

	bool Load(const KKSG::MentorAllData2Db& data);
	bool Save(KKSG::MentorAllData2Db& data);

	bool IsInMasterStudent(UINT64 roleID);

	const OneMentorRelation* GetRelation(UINT64 roleID);
	int GetSelfPos();
	int GetMentorRelationCurNum(int pos);
	int GetMentorRelationTotalNum(int pos);

	int GetRelationApplyStatus(UINT64 roleID);
	int GetRelationBuildTime(UINT64 roleID);
	//惩罚期 间隔期
	int GetPunishPeriod();
	int GetIntervalPeriod();

	int GetOppositePos();
	int GetRolePos(UINT64 roleID);
	int GetAlreadyMasterStudentTime(UINT64 roleID);
	int GetCurStatus(UINT64 roleID);

	bool AddRelation(UINT64 roleID, UINT64 applierID, int opposepos);

	UINT32 ApplyBreak(UINT64 destRoleID, UINT64 applierID, int time);
	UINT32 ApplyBreakCancel(UINT64 destRoleID, UINT64 applierID);
	UINT32 Complete(UINT64 destRoleID, int time, int type=1);
	UINT32 Break(UINT64 destRoleID, int time);
	UINT32 ApplyInherit(MentorShipOneSide* pOneSide, UINT64 applierID);
	UINT32 Inherit(MentorShipOneSide* pOneSide);

	bool CanInherit();

	UINT32 Convert2ClientRelation(KKSG::GetMyMentorInfoRes& roRes);

	void DebugClearInherit(UINT64 roleID = 0);
	const std::map<UINT64, OneMentorRelation>& GetRelationList(){return m_mentorRelationList;}
	void DoTxLog(UINT64 roleID, UINT64 RoleId2, int opposepos, bool isbreak=false, bool iscom=false);
private:
	void _GiveCompleteReward(int pos,  int type);
	void _CompleteAutoReport(UINT64 destRoleID);
	void _CheckChangeToNoRelation();
	void _CheckAfterBreak(UINT64 destRoleID);
	void _OnMasterComplete(UINT64 destRoleID);
	void _OnStudentComplete(UINT64 destRoleID);
private:
	MentorShipOneSide* m_pMentorOneSide;
	int m_selfPos;
	//int m_periodTime;
	//int m_punishTime;
	std::map<UINT64, OneMentorRelation> m_mentorRelationList;
};

#endif // 
