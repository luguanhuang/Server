#ifndef __MENTORMSG_H__
#define __MENTORMSG_H__

#define MENTORRECOMMENDLIST_REFRESHTIME 60
#define MENTORRECOMMENDLIST_SIZE 20
#define MENTORREBEAPPLYMSG_REMOVETIME 2*86400
#define MENTOR_MSGSAVENUM 200

class MentorShipOneSide;

namespace KKSG
{
	class MentorMyBeAppliedMsgArg;
	class MentorMyBeAppliedMsgRes;
	class OneMentorBeAppliedMsg;
	class GetMyApplyStudentInfoRes;
	class GetMyApplyMasterInfoRes;
}

struct OneMentorMsg
{
	OneMentorMsg()
	{
		memset(this, 0, sizeof(*this));
	}

	bool operator < (const OneMentorMsg& msg) const
	{
		if (m_type == msg.m_type && m_roleID == msg.m_roleID && m_paramid == msg.m_paramid)
		{
			return false;
		}
		if (m_type != msg.m_type)
		{
			return false;
		}
		if (m_roleID < msg.m_roleID)
		{
			return true;
		}
		else if (m_roleID == msg.m_roleID)
		{
			if (m_paramid < msg.m_paramid)
			{
				return true;
			}
			return false;
		}
		else
			return false;
	}

	bool operator == (const OneMentorMsg& msg) const
	{
		if (m_type == msg.m_type && m_roleID == msg.m_roleID && m_paramid == msg.m_paramid)
		{
			return true;
		}
		
		return false;
	}

	bool Convert2KKSGOneClientMsg(KKSG::OneMentorBeAppliedMsg& msg) const;

	int m_type;
	int m_time;
	UINT64 m_roleID;
	int m_paramid;
	int m_paramcount;
};

struct OneMentorRecommend
{
	OneMentorRecommend()
	{
		m_type = 0;
		m_isApply = false;
		m_roleID = 0;
	}
	int m_type;
	bool m_isApply;
	UINT64 m_roleID;
};

struct MentorRecommendList
{
public:
	MentorRecommendList()
	{
		m_refreshTime = 0;
	}

	int m_refreshTime;
	std::map<UINT64, OneMentorRecommend> m_recommendList;
};

class MentorMsg
{
public:
	MentorMsg(MentorShipOneSide* pOneSide);
	~MentorMsg();

public:
	bool Init();
	void Uninit();
	bool Load(const KKSG::MentorAllData2Db& data);
	void Save(KKSG::MentorAllData2Db& data);
	void OnDayPass();
	void OnLogin();

	UINT32 ClientGetStudentRecommend(KKSG::GetMyApplyStudentInfoRes& res, bool isRefresh = false);
	UINT32 ClientGetMasterRecommend(KKSG::GetMyApplyMasterInfoRes& res, bool isRefresh = false);

	UINT32 AddApplyMsg(UINT64 destRoleID, int type);
	UINT32 AddBeApplyMsg(UINT64 srcID, int type, int paramid = 0);
	UINT32 RemoveApplyMsg(UINT64 destID, int type);
	UINT32 RemoveBeApplyMsg(UINT64 srcID, int type, int paramid = 0);
	UINT32 OperateMsg(const KKSG::MentorMyBeAppliedMsgArg& arg, KKSG::MentorMyBeAppliedMsgRes& res);
	void OverRemoveMsg(UINT64 srcID);

	void DebugClear(UINT64 roleID, int type);
private:
	UINT32 _GetMsg(KKSG::MentorMyBeAppliedMsgRes& res);
	UINT32 _ClearMsg(KKSG::MentorMyBeAppliedMsgRes& res);
	UINT32 _AgreeMsg(const KKSG::MentorMyBeAppliedMsgArg& arg);
	UINT32 _AgreeOneMsg(const OneMentorMsg& arg);
	UINT32 _RejectMsg(const KKSG::MentorMyBeAppliedMsgArg& arg);

	void _RemoveMsg(std::set<OneMentorMsg>::iterator iter);
	UINT32 _HandelMsgAgreed(std::set<OneMentorMsg>::iterator iter);

	bool _HaveApplied(OneMentorMsg& msg);
	bool _RefreshRecommendList(int recomemndType);
	bool _GetRecommendRandResult(int applyStatus, std::vector<MentorShipOneSide*>& vec);

	void _NotifyCanApply();
	void _NotifyMsg();
	void _NotifyStudentHint();
	
	void _LoadBreakApplyMsg(OneMentorMsg& msg);
	void _SetRecommendApply(UINT64 destRoleID, int type);
private:
	MentorShipOneSide* m_pMentorOneSide;
	std::map<int, std::set<OneMentorMsg>> m_msgApplyList;//already apply student master list
	std::map<int, MentorRecommendList> m_recommendList;
	std::map<int, std::set<OneMentorMsg>> m_msgBeAppliedList;//MentorMsgApplyType key
};

#endif // 
