#include "pch.h"
#include "mentorrelationlist.h"
#include "mentoroneside.h"
#include <time.h>
#include "mentormgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "mentortask.h"
#include "util/XCommon.h"
#include "mentormsg.h"
#include "mentorrelationmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "mentorconfig.h"
#include "define/itemdef.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "loghelper/tlogr.h"

#include "foreach.h"

bool IsValidPos(int pos)
{
	if (pos <=0 || pos >= KKSG::EMentorPosMax)
	{
		return false;
	}

	return true;
}

bool IsValidStatus(int status)
{
	if (status <= 0 || status >= KKSG::MentorRelationMax)
	{
		return false;
	}

	return true;
}

void OneMentorRelation::InitCurStatus()
{
	int time = 0;
	foreach(i in m_statusMap)
	{
		if (i->second.m_time > time)
		{
			time = i->second.m_time;
			m_curStatus = i->second.m_status;
		}
	}
}

int OneMentorRelation::GetCurStatus() const
{
	return m_curStatus;
}

bool OneMentorRelation::IsRelationInStatus() const
{
	if (m_curStatus == KKSG::MentorRelationIn || m_curStatus == KKSG::MentorRelationBreakApply)
	{
		return true;
	}

	return false;
}

int OneMentorRelation::GetPos() const
{
	return m_pos;
}

int OneMentorRelation::GetInheritTime() 
{
	return m_inheritTime;
}

const STMentorRelationStatus* OneMentorRelation::GetStatusData(int status) const
{
	auto find = m_statusMap.find(status);
	if (find == m_statusMap.end())
	{
		return NULL;
	}

	return &find->second;
}

bool OneMentorRelation::Convert2ClientOneRelation(MentorShipOneSide* self, KKSG::OneMentorRelationInfo2Client& relation)
{
	int now = GameTime::GetTime();
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleID);
	if (!summary)
	{
		return false;
	}
	if (!IsValidStatus(m_curStatus))
	{
		return false;
	}
	KKSG::RoleBriefInfo* rolebrief = relation.mutable_roleinfo();
	rolebrief->set_roleid(m_roleID);
	rolebrief->set_type((KKSG::RoleType)summary->GetProfession());
	rolebrief->set_name(summary->GetName());
	rolebrief->set_level(summary->GetLevel());

	relation.set_pos(m_pos);
	relation.set_curstatus((KKSG::MentorRelationStatus)m_curStatus);
	if(XCommon::IsDaySame(GetInheritTime(), now))
	{
		relation.set_inheritstatus(KKSG::EMentorTask_ConfirmReport);
	}
	else
	{
		if (XCommon::IsDaySame(m_ApplyHeritTime, now))
		{
			relation.set_inheritstatus(KKSG::EMentorTask_AlreadyReport);
			relation.set_inheritapplyroleid(m_applyInheritRoleID);
		}
		else
		{
			relation.set_inheritstatus(KKSG::EMentorTask_CanReport);
		}
	}

	auto statusData = relation.add_relationlist();
	statusData->set_status(KKSG::MentorRelationIn);
	statusData->set_time(m_statusMap[KKSG::MentorRelationIn].m_time);

	auto findBreakApply = m_statusMap.find(KKSG::MentorRelationBreakApply);
	if (findBreakApply != m_statusMap.end())
	{
		statusData = relation.add_relationlist();
		statusData->set_status(findBreakApply->first);
		statusData->set_time(findBreakApply->second.m_time);
		relation.set_breakapplyroleid(findBreakApply->second.m_applierID);
	}

	if (m_pos == KKSG::EMentorPosStudent)
	{
		MentorShipOneSide* pOneSide = MentorMgr::Instance()->GetMentorShip(m_roleID);
		if (!pOneSide)
		{
			return false;
		}

		//pOneSide->GetMentorTask()->Convert2OneRelationClient(relation, m_roleID);
		pOneSide->GetMentorTask()->Convert2OneRelationClient(relation, self->GetOwnerID());
	}

	return true;
}

bool OneMentorRelation::CanInherit() const
{
	if (!IsRelationInStatus())
	{
		return false;
	}
	int now = GameTime::GetTime();
	if (XCommon::IsDaySame(m_ApplyHeritTime, now))
	{
		return false;
	}

	return true;
}

bool OneMentorRelation::ApplyInherit(UINT64 applierID)
{
	if (!IsRelationInStatus())
	{
		return false;
	}
	int now = GameTime::GetTime();
	if (XCommon::IsDaySame(m_ApplyHeritTime, now))
	{
		return false;
	}

	m_ApplyHeritTime = now;
	m_applyInheritRoleID = applierID;

	return true;
}

bool OneMentorRelation::Inherit(int time)
{
	if (!IsRelationInStatus())
	{
		return false;
	}

	if (XCommon::IsDaySame(GetInheritTime(), time))
	{
		return false;
	}

	m_inheritTime = time;

	return true;
}

void OneMentorRelation::DebugClearInherit()
{
	m_inheritTime = 0;
	m_applyInheritRoleID = 0;
	m_ApplyHeritTime = 0;
}

MentorRelationList::MentorRelationList(MentorShipOneSide* pOneSide):m_pMentorOneSide(pOneSide)
{
	m_selfPos = 0;
	//m_periodTime = 0;
	//m_punishTime = 0;
}

MentorRelationList::~MentorRelationList()
{

}

bool MentorRelationList::Init()
{
	return true;
}

void MentorRelationList::Uninit()
{

}

bool MentorRelationList::Load(const KKSG::MentorAllData2Db& data)
{
	m_selfPos = data.selfpos();
	for (int i = 0; i < data.relationrolelist_size(); ++i)
	{
		auto relation = data.relationrolelist(i);
		OneMentorRelation temp;
		temp.m_roleID = relation.roleid();
		temp.m_pos = relation.pos();
		temp.m_inheritTime = relation.inherittime();
		temp.m_ApplyHeritTime = relation.applyinherittime();
		temp.m_applyInheritRoleID = relation.applyinheritroleid();
		temp.m_curStatus = relation.curstatus();
		for (int j = 0; j < relation.relationstatelist_size(); ++j)
		{
			STMentorRelationStatus status;
			status.m_status = relation.relationstatelist(j).status();
			status.m_time = relation.relationstatelist(j).time();
			status.m_applierID = relation.relationstatelist(j).applierid();
			status.m_beApplyRoleOnlineTime = relation.relationstatelist(j).beapplyroleonlinetime();
			temp.m_statusMap[status.m_status] = status;
		}
		m_mentorRelationList[temp.m_roleID] = temp;
		m_mentorRelationList[temp.m_roleID].InitCurStatus();
	}

	return true;
}

bool MentorRelationList::Save(KKSG::MentorAllData2Db& data)
{
	data.set_selfpos(m_selfPos);
	for (auto iter = m_mentorRelationList.begin(); iter != m_mentorRelationList.end(); ++iter)
	{
		auto relation = data.add_relationrolelist();
		relation->set_roleid(iter->first);
		relation->set_pos(iter->second.m_pos);
		relation->set_inherittime(iter->second.m_inheritTime);
		relation->set_applyinherittime(iter->second.m_ApplyHeritTime);
		relation->set_applyinheritroleid(iter->second.m_applyInheritRoleID);
		relation->set_curstatus(iter->second.m_curStatus);
		for (auto j = iter->second.m_statusMap.begin(); j != iter->second.m_statusMap.end(); ++j)
		{
			auto status = relation->add_relationstatelist();
			status->set_status(j->first);
			status->set_time(j->second.m_time);
			status->set_applierid(j->second.m_applierID);
			status->set_beapplyroleonlinetime(j->second.m_beApplyRoleOnlineTime);
		}
	}

	return true;
}

int MentorRelationList::GetSelfPos()
{
	return m_selfPos;
}

int MentorRelationList::GetMentorRelationCurNum(int pos)
{
	if (!IsValidPos(pos))
	{
		return 0;
	}
	if (pos == m_selfPos)
	{
		return 0;
	}
	int num = 0;
	foreach(i in m_mentorRelationList)
	{
		if(i->second.m_pos == pos && i->second.IsRelationInStatus())
			++num;
	}

	return num;
}

int MentorRelationList::GetMentorRelationTotalNum(int pos)
{
	int num = 0;
	foreach(i in m_mentorRelationList)
	{
		if(i->second.m_pos == pos)
			++num;
	}

	return num;
}

const OneMentorRelation* MentorRelationList::GetRelation(UINT64 roleID)
{
	auto find = m_mentorRelationList.find(roleID);
	if (find == m_mentorRelationList.end())
	{
		return NULL;
	}

	return &find->second;
}

int MentorRelationList::GetRelationBuildTime(UINT64 roleID)
{
	auto data = GetRelation(roleID);
	if (!data)
	{
		return 0;
	}

	if (!data->IsRelationInStatus())
	{
		return 0;
	}

	auto statusData = data->GetStatusData(KKSG::MentorRelationIn);
	if (!statusData)
	{
		return 0;
	}

	return statusData->m_time;
}

int MentorRelationList::GetPunishPeriod()
{
	int latestTime = 0;
	UINT64 ownerID = m_pMentorOneSide->GetOwnerID();
	for (auto iter = m_mentorRelationList.begin(); iter != m_mentorRelationList.end(); ++iter)
	{
		int curStatus = iter->second.m_curStatus;
		if (curStatus == KKSG::MentorRelationBreak)
		{
			auto data = iter->second.GetStatusData(curStatus);
			if (data && data->m_applierID == ownerID && data->m_beApplyRoleOnlineTime && data->m_time > latestTime)
			{
				latestTime = data->m_time;
			}
		}
	}
	if (latestTime)
	{
		int now = GameTime::GetTime();
		if (now - latestTime > MENTORRELATION_PUNISHTIME)
		{
			return 0;
		}
		return latestTime + MENTORRELATION_PUNISHTIME - now; 
	}
	return 0;
}

int MentorRelationList::GetIntervalPeriod()
{
	if (m_selfPos != KKSG::EMentorPosMaster)
	{
		return 0;
	}

	int totalStudentNum = GetMentorRelationTotalNum(KKSG::EMentorPosStudent);
	if (totalStudentNum < MENTORRELATION_INTERVALTIMENUM)
	{
		return 0;
	}

	int latestTime = 0;
	for (auto iter = m_mentorRelationList.begin(); iter != m_mentorRelationList.end(); ++iter)
	{
		auto data = iter->second.GetStatusData(KKSG::MentorRelationIn);
		if (data && data->m_time > latestTime)
		{
			latestTime = data->m_time;
		}
	}
	int now = GameTime::GetTime();
	if (now - latestTime > MENTORRELATION_INTERVALTIME)
	{
		return 0;
	}
	return latestTime + MENTORRELATION_INTERVALTIME - now;
}

bool MentorRelationList::IsInMasterStudent(UINT64 roleID)
{
	auto data = GetRelation(roleID);
	if (!data)
	{
		return false;
	}
	int status = data->GetCurStatus();
	if (status == KKSG::MentorRelationIn || status == KKSG::MentorRelationBreakApply)
	{
		return true;
	}

	return false;
}

int MentorRelationList::GetAlreadyMasterStudentTime(UINT64 roleID)
{
	if (!IsInMasterStudent(roleID))
	{
		return 0;
	}

	auto& ref = m_mentorRelationList[roleID];

	return GameTime::GetTime() - ref.m_statusMap[KKSG::MentorRelationIn].m_time;
}

int MentorRelationList::GetCurStatus(UINT64 roleID)
{
	auto data = GetRelation(roleID);
	if (!data)
	{
		return KKSG::MentorRelationMax;
	}
	int status = data->GetCurStatus();

	return status;
}

int MentorRelationList::GetOppositePos()
{
	if (m_selfPos == KKSG::EMentorPosMaster)
	{
		return KKSG::EMentorPosStudent;
	}
	else if (m_selfPos == KKSG::EMentorPosStudent)
	{
		return KKSG::EMentorPosMaster;
	}
	
	return KKSG::EMentorPosMax;
}

int MentorRelationList::GetRolePos(UINT64 roleID)
{
	auto data = GetRelation(roleID);
	if (!data)
	{
		return KKSG::EMentorPosMax;
	}

	return data->m_pos;
}

bool MentorRelationList::AddRelation(UINT64 roleID, UINT64 applierID, int pos)
{
	OneMentorRelation relation;
	relation.m_roleID = roleID;
	relation.m_pos = pos;
	relation.m_curStatus = KKSG::MentorRelationIn;

	STMentorRelationStatus status;
	status.m_applierID = applierID;
	status.m_status = KKSG::MentorRelationIn;
	status.m_time = GameTime::GetTime();

	relation.m_statusMap[status.m_status] = status;

	m_mentorRelationList[roleID] = relation;

	m_selfPos = MentorRelationMgr::Instance()->GetOppositePos(pos);

	//MentorMgr::Instance()->QuickSave(m_pMentorOneSide);

	LogInfo("%llu, build relation:%llu, selfpos:%u", m_pMentorOneSide->GetOwnerID(), roleID, m_selfPos);

	DoTxLog(m_pMentorOneSide->GetOwnerID(), roleID, m_selfPos);

	return true;
}

UINT32 MentorRelationList::ApplyBreak(UINT64 destRoleID, UINT64 applierID, int time)
{
	if (!IsInMasterStudent(destRoleID))
	{
		return KKSG::ERR_MENTOR_CUR_STATUS_CANNOT_BREAK;
	}

	auto& ref = m_mentorRelationList[destRoleID];
	if (ref.GetCurStatus() == KKSG::MentorRelationBreakApply)
	{
		return KKSG::ERR_MENTOR_OTHER_APPLY_BREAK;
	}

	auto& statusRef = ref.m_statusMap[KKSG::MentorRelationBreakApply];
	statusRef.m_applierID = applierID;
	statusRef.m_status = KKSG::MentorRelationBreakApply;
	statusRef.m_time = time;

	ref.m_curStatus = KKSG::MentorRelationBreakApply;

	if (m_pMentorOneSide->GetOwnerID() == applierID)
	{
		CRole* destRole = CRoleManager::Instance()->GetByRoleID(destRoleID);
		if (destRole)
		{
			statusRef.m_beApplyRoleOnlineTime = GameTime::GetTime();
		}
		else
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(destRoleID);
			if (summary)
			{
				statusRef.m_beApplyRoleOnlineTime = summary->LogoutTime();
			}
		}
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorRelationList::ApplyBreakCancel(UINT64 destRoleID, UINT64 applierID)
{
	auto find = m_mentorRelationList.find(destRoleID);
	if (find == m_mentorRelationList.end())
	{
		return KKSG::ERR_FAILED;
	}

	auto relation = &find->second;
	if (relation->m_curStatus != KKSG::MentorRelationBreakApply)
	{
		return KKSG::ERR_FAILED;
	}
	if (relation->m_statusMap[relation->m_curStatus].m_applierID != applierID)
	{
		return KKSG::ERR_FAILED;
	}

	relation->m_statusMap.erase(relation->m_curStatus);
	relation->m_curStatus = KKSG::MentorRelationIn;

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorRelationList::Complete(UINT64 destRoleID, int time, int type/*=1*/)
{
	auto find = m_mentorRelationList.find(destRoleID);
	if (find == m_mentorRelationList.end())
	{
		return KKSG::ERR_FAILED;
	}

	auto relation = &find->second;
	auto status = &relation->m_statusMap[KKSG::MentorRelationComplete];
	status->m_status = KKSG::MentorRelationComplete;
	status->m_time = time;
	relation->m_curStatus = status->m_status;
	relation->m_statusMap.erase(KKSG::MentorRelationBreakApply);

	_GiveCompleteReward(m_selfPos, type);
	if (m_selfPos == KKSG::EMentorPosMaster)
	{
		_OnMasterComplete(destRoleID);
		/*if (GetMentorRelationTotalNum(KKSG::EMentorPosStudent) > MENTORRELATION_INTERVALTIMENUM)
		{
			//需要修改 间隔期计算问题
			//m_periodTime = time;
		}*/

	}
	else if (m_selfPos == KKSG::EMentorPosStudent)
	{
		_OnStudentComplete(destRoleID);
		DoTxLog(m_pMentorOneSide->GetOwnerID(), destRoleID, m_selfPos, false, true);
	}

	LogInfo("%llu,selfpos:%u, %llu, complete relation, type:%u", m_pMentorOneSide->GetOwnerID(), m_selfPos, destRoleID, type);

	_CheckChangeToNoRelation();

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorRelationList::Break(UINT64 destRoleID, int time)
{
	if (GetCurStatus(destRoleID) != KKSG::MentorRelationBreakApply)
	{
		return KKSG::ERR_FAILED;
	}

	auto& ref = m_mentorRelationList[destRoleID];
	auto& statusRef = ref.m_statusMap[KKSG::MentorRelationBreakApply];

	STMentorRelationStatus status;
	status.m_applierID = statusRef.m_applierID;
	status.m_status = KKSG::MentorRelationBreak;
	status.m_time = time;

	if(status.m_applierID == m_pMentorOneSide->GetOwnerID())
	{
		////CRole* destRole = CRoleManager::Instance()->GetByRoleID(destRoleID);
		////if (!destRole)
		//{
		//	//CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(destRoleID);
		//	//if (summary && statusRef.m_time < (summary->LogoutTime() + MENTORRELATION_OFFLINEUNPUNISHTIME))
		//	//{
		//	//	//status.m_beApplyRoleOnlineTime = summary->LogoutTime();
		//	//	//m_punishTime = time;
		//	//}
		//}
		if (statusRef.m_time < statusRef.m_beApplyRoleOnlineTime + MENTORRELATION_OFFLINEUNPUNISHTIME)
		{
			status.m_beApplyRoleOnlineTime = statusRef.m_beApplyRoleOnlineTime;
		}
	}
	ref.m_statusMap[KKSG::MentorRelationBreak] = status;
	ref.m_curStatus = KKSG::MentorRelationBreak;

	LogInfo("%llu, break relation:%llu, selfpos:%u", m_pMentorOneSide->GetOwnerID(), destRoleID, m_selfPos);
	DoTxLog(m_pMentorOneSide->GetOwnerID(), destRoleID, m_selfPos, true);

	//_CheckChangeToNoRelation();
	_CheckAfterBreak(destRoleID);

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorRelationList::Convert2ClientRelation(KKSG::GetMyMentorInfoRes& roRes)
{
	int oppositePos = GetOppositePos();
	if (oppositePos == KKSG::EMentorPosMax)
	{
		return KKSG::ERR_FAILED;
	}

	foreach(i in m_mentorRelationList)
	{
		if (!i->second.IsRelationInStatus())
		{
			continue;
		}
		if (i->second.GetPos() != oppositePos)
		{
			continue;
		}

		KKSG::OneMentorRelationInfo2Client temp;
		if (i->second.Convert2ClientOneRelation(m_pMentorOneSide, temp))
		{
			roRes.add_mentorrelationlist()->CopyFrom(temp);
		}
	}

	return KKSG::ERR_SUCCESS;
}

void MentorRelationList::DebugClearInherit(UINT64 roleID)
{
	if (!roleID)
	{
		for (auto iter = m_mentorRelationList.begin(); iter != m_mentorRelationList.end(); ++iter)
		{
			iter->second.DebugClearInherit();
		}
	}
	else
	{
		auto find = m_mentorRelationList.find(roleID);
		if (find != m_mentorRelationList.end())
		{
			find->second.DebugClearInherit();
		}
	}
}

UINT32 MentorRelationList::ApplyInherit(MentorShipOneSide* pOneSide, UINT64 applierID)
{
	if (!pOneSide)
	{
		return KKSG::ERR_FAILED;
	}

	/*if (m_selfPos != KKSG::EMentorPosStudent)
	{
		return KKSG::ERR_FAILED;
	}*/

	auto find = m_mentorRelationList.find(pOneSide->GetOwnerID());
	if (find == m_mentorRelationList.end())
	{
		return KKSG::ERR_FAILED;
	}

	int now = GameTime::GetTime();
	if (!find->second.ApplyInherit(applierID))
	{
		return KKSG::ERR_MENTOR_OTHER_ALREADY_APPLY_INHERIT;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorRelationList::Inherit(MentorShipOneSide* pOneSide)
{
	if (!pOneSide)
	{
		return KKSG::ERR_FAILED;
	}

	auto find = m_mentorRelationList.find(pOneSide->GetOwnerID());
	if (find == m_mentorRelationList.end())
	{
		return KKSG::ERR_FAILED;
	}

	int now = GameTime::GetTime();
	if (find->second.Inherit(now))
	{
		//pOneSide->GetMentorMsg()->AddBeApplyMsg(m_pMentorOneSide->GetOwnerID(), KKSG::MentorMsgApplyInherit);
		//MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());
	}
	else
	{
		return KKSG::ERR_FAILED;
	}

	LogInfo("%llu, %llu, complete inheirt", m_pMentorOneSide->GetOwnerID(), pOneSide->GetOwnerID());

	return KKSG::ERR_SUCCESS;
}

bool MentorRelationList::CanInherit()
{
	bool can = false;
	auto& relationList = GetRelationList();
	for (auto iter = relationList.begin(); iter != relationList.end(); ++iter)
	{
		if (iter->second.CanInherit())
		{
			can = true;
			break;
		}
	}

	return can;
}

void MentorRelationList::_GiveCompleteReward(int pos, int type)
{
	auto data = MentorConfig::Instance()->GetCompleteRewardData(type);
	if (!data)
	{
		return;
	}
	std::vector<ItemDesc> items;
	if (pos == KKSG::EMentorPosMaster)
	{
		for (auto i = data->MasterReward.begin(); i != data->MasterReward.end(); ++i)
		{
			ItemDesc item(i->seq[0], i->seq[1]);
			items.push_back(item);
		}
	}
	else if (pos == KKSG::EMentorPosStudent)
	{
		for (auto i = data->StudentReward.begin(); i != data->StudentReward.end(); ++i)
		{
			ItemDesc item(i->seq[0], i->seq[1]);
			items.push_back(item);
		}
	}
	if (items.empty())
	{
		return;
	}
	MailData mailData( GetGlobalConfig().Mentor_CompleteRewardTitle, GetGlobalConfig().Mentor_CompleteRewardContent, items, MAIL_EXPIRE_TIME);
	mailData.SetReason(ItemFlow_MentorComplete);
	CMailMgr::Instance()->SendMail(m_pMentorOneSide->GetOwnerID(), mailData);
}

void MentorRelationList::_CompleteAutoReport(UINT64 destRoleID)
{
	if (m_pMentorOneSide->GetRelationList()->GetSelfPos() != KKSG::EMentorPosStudent)
	{
		return;
	}
	
	m_pMentorOneSide->GetMentorTask()->CompleteAutoReport(destRoleID);
}

void MentorRelationList::_CheckChangeToNoRelation()
{
	if (!GetMentorRelationCurNum(GetOppositePos()))
	{
		m_selfPos = 0;
	}
}

void MentorRelationList::_CheckAfterBreak(UINT64 destRoleID)
{
	if (GetSelfPos() == KKSG::EMentorPosStudent)
	{
		m_pMentorOneSide->GetMentorTask()->RemoveReportTask(destRoleID);
	}
	else
		m_pMentorOneSide->GetMentorMsg()->OverRemoveMsg(destRoleID);
	_CheckChangeToNoRelation();

	std::vector<ItemDesc> items;
	std::string tempstring = "xxx";
	std::string content = GetGlobalConfig().MentorBreakContent;
	std::string::size_type pos = 0;
	std::string name = "";
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(destRoleID); 
	if (summary)
	{
		name = summary->GetName();
	}
	pos = content.find(tempstring);
	if (pos != std::string::npos)
	{
		content.replace(pos, tempstring.size(), name);
	}
	MailData mailData( GetGlobalConfig().MentorBreakTitle, content, items, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(m_pMentorOneSide->GetOwnerID(), mailData);
}

void MentorRelationList::_OnMasterComplete(UINT64 destRoleID)
{
	m_pMentorOneSide->AddMentorCompleteNum(1);
	m_pMentorOneSide->GetMentorMsg()->OverRemoveMsg(destRoleID);
}

void MentorRelationList::_OnStudentComplete(UINT64 destRoleID)
{
	m_pMentorOneSide->SetSelfCompleteMentor(true);
	_CompleteAutoReport(destRoleID);	
}

void MentorRelationList::DoTxLog(UINT64 roleID, UINT64 RoleId2, int opposepos, bool isbreak, bool iscom)
{
	TMentorFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleID;
	oLog.m_RoleID2 = RoleId2;
	if (iscom)//出师;
	{
		oLog.m_op = Tx_MentorType5;
	}
	else if (opposepos == KKSG::EMentorPosMaster)
	{
		oLog.m_op = isbreak?Tx_MentorType3:Tx_MentorType1;
	}
	else if (opposepos == KKSG::EMentorPosStudent)
	{
		oLog.m_op = isbreak?Tx_MentorType2:Tx_MentorType0;
	}
	oLog.Do();
}
