#ifndef __MENTORONESIDE_H__
#define __MENTORONESIDE_H__

#include "role/switch.h"

class MentorMsg;
class MentorRelationList;
class MentorTask;

namespace KKSG
{
	class GetMyMentorInfoRes;
	class UpdateMentorApplyStudentInfoArg;
}

class MentorShipOneSide
{
public:
	MentorShipOneSide(UINT64 roleID);
	~MentorShipOneSide();

	void Load(const KKSG::MentorAllData2Db& data);
	void Save(KKSG::MentorAllData2Db& data);
	void SetChanged();

	//void ImmediatelySave();

	void Init();
	bool IsOpen();
	void DoOpen();
	void OnRoleLogin();
	bool HasCompleteMentor();

	UINT32 ClientGetMentorInfo(KKSG::GetMyMentorInfoRes& roRes);
	UINT32 ClientReqestRelationOp(const KKSG::MentorRelationOpArg& arg);
	UINT32 UpdateSelfInfo(const KKSG::UpdateMentorApplyStudentInfoArg& arg);

	MentorRelationList* GetRelationList(){return m_mentorRelation;}
	MentorMsg* GetMentorMsg(){return m_mentorMsg;}
	MentorTask* GetMentorTask(){return m_mentorTask;}

	//bool GetIsOnlineNotify(){return m_isOnlineNotify;}
	bool GetIsNeedMentor(){return m_isNeedStudent;}
	bool CanOnlinNotifyApply();
	UINT64 GetOwnerID(){return m_roleID;}
	UINT64 GetAudioID(){return m_mentorAudioID;}
	std::string& GetMentorWords(){return m_mentorWords;}

	void AddMentorCompleteNum(int num);
	void SetSelfCompleteMentor(bool complete);

private:
	//save
	UINT64 m_roleID;
	UINT32 m_updateTime;
	UINT64 m_mentorAudioID;
	std::string m_mentorWords;
	bool m_isNeedStudent;
	bool m_isOnlineNotify;
	bool m_hasCompleteMentor;
	UINT32 m_menteeCompleteNum;

	MentorRelationList* m_mentorRelation;
	MentorTask* m_mentorTask;
	MentorMsg* m_mentorMsg;

	Switch m_switch;
};

#endif // 
