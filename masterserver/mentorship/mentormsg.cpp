#include "pch.h"
#include <time.h>
#include "mentormsg.h"
#include "mentoroneside.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "mentorrelationmgr.h"
#include "mentormgr.h"
#include "mentorrelationlist.h"
#include "mentortask.h"
#include "util/XRandom.h"
#include "mentorship/ptcm2c_notifymentorapply.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "util/XCommon.h"
#include "levelseal/levelsealMgr.h"
#include "guild/guildinherit.h"
#include "util/gametime.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "define/systemiddef.h"
#include "foreach.h"

bool IsValidMsgType(int type)
{
	if (type < KKSG::MentorMsgApplyMaster || type >= KKSG::MentorMsgApplyMax)
	{
		return false;
	}
	
	return true;
}

bool OneMentorMsg::Convert2KKSGOneClientMsg(KKSG::OneMentorBeAppliedMsg& msg) const
{
	if (!IsValidMsgType(m_type))
	{
		return false;
	}

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleID);
	if (!summary)
	{
		return false;
	}

	msg.set_type((KKSG::MentorMsgApplyType)m_type);
	msg.set_time(m_time);
	msg.set_reporttaskid(m_paramid);
	
	KKSG::RoleBriefInfo* info = msg.mutable_rolebrief();
	info->set_type((KKSG::RoleType)summary->GetProfession());
	info->set_roleid(m_roleID);
	info->set_name(summary->GetName());
	info->set_level(summary->GetLevel());

	return true;
}

MentorMsg::MentorMsg(MentorShipOneSide* pOneSide):m_pMentorOneSide(pOneSide)
{

}

MentorMsg::~MentorMsg()
{

}

bool MentorMsg::Init()
{
	return true;
}

void MentorMsg::Uninit()
{

}

bool MentorMsg::Load(const KKSG::MentorAllData2Db& data)
{
	for (int i = 0; i < data.applymsglist_size(); ++i)
	{
		auto msg = data.applymsglist(i);
		OneMentorMsg temp;
		temp.m_type = msg.type();
		temp.m_roleID = msg.roleid();
		temp.m_time = msg.time();
		temp.m_paramid = msg.paramid();
		temp.m_paramcount = msg.paramcount();
		m_msgApplyList[temp.m_type].insert(temp);
	}
	for (int i = 0; i < data.beappliedmsglist_size(); ++i)
	{
		auto msg = data.beappliedmsglist(i);
		OneMentorMsg temp;
		temp.m_type = msg.type();
		temp.m_roleID = msg.roleid();
		temp.m_time = msg.time();
		temp.m_paramid = msg.paramid();
		temp.m_paramcount = msg.paramcount();
		m_msgBeAppliedList[temp.m_type].insert(temp);

		_LoadBreakApplyMsg(temp);
	}

	return true;
}

void MentorMsg::Save(KKSG::MentorAllData2Db& data)
{
	for (auto iter = m_msgApplyList.begin(); iter != m_msgApplyList.end(); ++iter)
	{
		int num = 0;
		for (auto j = iter->second.begin(); j != iter->second.end() && num < MENTOR_MSGSAVENUM; ++j, ++num)
		{
			auto msg = data.add_applymsglist();
			msg->set_type(j->m_type);
			msg->set_roleid(j->m_roleID);
			msg->set_time(j->m_time);
			msg->set_paramid(j->m_paramid);
			msg->set_paramcount(j->m_paramcount);
		}
	}
	for (auto iter = m_msgBeAppliedList.begin(); iter != m_msgBeAppliedList.end(); ++iter)
	{
		int num = 0;
		for (auto j = iter->second.begin(); j != iter->second.end() && num < MENTOR_MSGSAVENUM; ++j, ++num)
		{
			auto msg = data.add_beappliedmsglist();
			msg->set_type(j->m_type);
			msg->set_roleid(j->m_roleID);
			msg->set_time(j->m_time);
			msg->set_paramid(j->m_paramid);
			msg->set_paramcount(j->m_paramcount);
		}
	}
}

void MentorMsg::OnDayPass()
{
	int now = GameTime::GetTime();
	m_msgApplyList.clear();
	for (auto iter = m_msgBeAppliedList.begin(); iter != m_msgBeAppliedList.end(); ++iter)
	{
		if (iter->first == KKSG::MentorMsgApplyInherit)
		{
			iter->second.clear();
			continue;
		}
		if (iter->first == KKSG::MentorMsgApplyMaster || iter->first == KKSG::MentorMsgApplyStudent)
		{
			for (auto j = iter->second.begin(); j != iter->second.end();)
			{
				if (now - j->m_time >= MENTORREBEAPPLYMSG_REMOVETIME)
				{
					iter->second.erase(j++);
				}
				else
					++j;
			}
		}
	}
}

void MentorMsg::OnLogin()
{
	if (m_pMentorOneSide->GetIsNeedMentor() && m_pMentorOneSide->CanOnlinNotifyApply())
	{
		_NotifyCanApply();
	}

	_NotifyMsg();

	_NotifyStudentHint();
}

UINT32 MentorMsg::ClientGetStudentRecommend(KKSG::GetMyApplyStudentInfoRes& res, bool isRefresh /*= false*/)
{
	if (m_pMentorOneSide->GetRelationList()->GetSelfPos() == KKSG::EMentorPosStudent)
	{
		return KKSG::ERR_FAILED;
	}
	if (m_pMentorOneSide->GetRelationList()->GetMentorRelationCurNum(KKSG::EMentorPosStudent) >= MENTORRELATION_CURMAXNUM)
	{
		return KKSG::ERR_FAILED;
	}
	ApplyOneSideConditon myCondition(m_pMentorOneSide, KKSG::EMentorPosStudent);
	if (!myCondition.IsValid())
	{
		return KKSG::ERR_MENTOR_CUR_STATUS_CANNOT_APPLYSTUDENT;
	}
	if (isRefresh && !_RefreshRecommendList(KKSG::MentorMsgApplyStudent))
	{
		return KKSG::ERR_MENTOR_REFRESHTOOFAST;
	}

	auto list = &m_recommendList[KKSG::MentorApplyStudent];
	if (!isRefresh && !list->m_refreshTime)
	{
		_RefreshRecommendList(KKSG::MentorMsgApplyStudent);
	}
	if (list->m_refreshTime)
	{
		int now = GameTime::GetTime();
		int left = now > list->m_refreshTime + MENTORRECOMMENDLIST_REFRESHTIME ? 0 : (list->m_refreshTime + MENTORRECOMMENDLIST_REFRESHTIME - now);
		res.set_leftrefreshtime(left);
	}
	for (auto iter = list->m_recommendList.begin(); iter != list->m_recommendList.end(); ++iter)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(iter->second.m_roleID);
		if (!summary)
		{
			continue;
		}

		MentorShipOneSide* tempSide = MentorMgr::Instance()->GetMentorShip(iter->second.m_roleID);
		if (!tempSide)
		{
			continue;
		}


		auto show = res.add_canapplylist();
		show->set_hasapply(iter->second.m_isApply);
		show->set_applywords(tempSide->GetMentorWords());

		KKSG::RoleBriefInfo* info = show->mutable_onestudent();
		info->set_type((KKSG::RoleType)summary->GetProfession());
		info->set_roleid(summary->GetID());
		info->set_name(summary->GetName());
		info->set_level(summary->GetLevel());
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorMsg::ClientGetMasterRecommend(KKSG::GetMyApplyMasterInfoRes& res, bool isRefresh /*= false*/)
{
	if (m_pMentorOneSide->GetRelationList()->GetSelfPos() == KKSG::EMentorPosMaster)
	{
		return KKSG::ERR_FAILED;
	}

	if (m_pMentorOneSide->GetRelationList()->GetMentorRelationCurNum(KKSG::EMentorPosMaster) >= MENTORRELATION_CURMAXNUM)
	{
		return KKSG::ERR_FAILED;
	}

	if (m_pMentorOneSide->HasCompleteMentor())
	{
		return KKSG::ERR_MENTOR_ALREADY_COMPLETE_MENTOR;
	}

	if (isRefresh && !_RefreshRecommendList(KKSG::MentorMsgApplyMaster))
	{
		return KKSG::ERR_MENTOR_REFRESHTOOFAST;
	}

	auto list = &m_recommendList[KKSG::MentorApplyMaster];
	if (!isRefresh && !list->m_refreshTime)
	{
		_RefreshRecommendList(KKSG::MentorMsgApplyMaster);
	}
	if (list->m_refreshTime)
	{
		int now = GameTime::GetTime();
		int left = now > list->m_refreshTime + MENTORRECOMMENDLIST_REFRESHTIME ? 0 : (list->m_refreshTime + MENTORRECOMMENDLIST_REFRESHTIME - now);
		res.set_leftrefreshtime(left);
	}
	for (auto iter = list->m_recommendList.begin(); iter != list->m_recommendList.end(); ++iter)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(iter->second.m_roleID);
		if (!summary)
		{
			continue;
		}
		MentorShipOneSide* tempSide = MentorMgr::Instance()->GetMentorShip(iter->second.m_roleID);
		if (!tempSide)
		{
			continue;
		}

		auto show = res.add_canapplymasters();
		show->set_hasapply(iter->second.m_isApply);
		show->set_audioid(tempSide->GetAudioID());
		show->set_applywords(tempSide->GetMentorWords());

		KKSG::RoleBriefInfo* info = show->mutable_onemaster();
		info->set_type((KKSG::RoleType)summary->GetProfession());
		info->set_roleid(summary->GetID());
		info->set_name(summary->GetName());
		info->set_level(summary->GetLevel());
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorMsg::AddApplyMsg(UINT64 destRoleID, int type)
{
	if (!IsValidMsgType(type))
	{
		return KKSG::ERR_FAILED;
	}

	OneMentorMsg temp;
	temp.m_type = type;
	temp.m_roleID = destRoleID;

	m_msgApplyList[type].insert(temp);

	_SetRecommendApply(destRoleID, type);

	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	return KKSG::ERR_SUCCESS;

}

UINT32 MentorMsg::AddBeApplyMsg(UINT64 srcID, int type, int paramid)
{
	if (!IsValidMsgType(type))
	{
		return KKSG::ERR_FAILED;
	}

	int now = GameTime::GetTime();
	OneMentorMsg temp;
	temp.m_type = type;
	temp.m_roleID = srcID;
	temp.m_paramid = paramid;
	temp.m_time = now;

	auto find = m_msgBeAppliedList[type].find(temp);
	if (find == m_msgBeAppliedList[type].end())
	{
		temp.m_paramcount = 1;
		m_msgBeAppliedList[type].insert(temp);
	}
	else
	{
		if (find->m_paramcount >= 3 && XCommon::IsDaySame(find->m_time, now))
		{
			//需要修改error
//#ifdef TEST
//			return KKSG::ERR_SUCCESS;
//#endif
			return KKSG::ERR_MENTOR_ASKMAXTODAY;
		}
		if (!XCommon::IsDaySame(find->m_time, now))
			temp.m_paramcount = 1;
		else
			temp.m_paramcount = find->m_paramcount + 1;

		m_msgBeAppliedList[type].erase(find);
		m_msgBeAppliedList[type].insert(temp);
	}


	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	_NotifyMsg();

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorMsg::RemoveApplyMsg(UINT64 destID, int type)
{
	if (!IsValidMsgType(type))
	{
		return KKSG::ERR_FAILED;
	}

	OneMentorMsg temp;
	temp.m_type = type;
	temp.m_roleID = destID;

	auto find = m_msgApplyList.find(type);
	if (find == m_msgApplyList.end())
	{
		return KKSG::ERR_FAILED;
	}

	auto findmsg = find->second.find(temp);
	if(findmsg == find->second.end())
		return KKSG::ERR_FAILED;

	find->second.erase(findmsg);

	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	return KKSG::ERR_SUCCESS;

}

UINT32 MentorMsg::RemoveBeApplyMsg(UINT64 srcID, int type, int paramid /*= 0*/)
{
	if (!IsValidMsgType(type))
	{
		return KKSG::ERR_FAILED;
	}

	OneMentorMsg temp;
	temp.m_type = type;
	temp.m_roleID = srcID;
	temp.m_paramid = paramid;

	auto find = m_msgBeAppliedList.find(type);
	if (find == m_msgBeAppliedList.end())
	{
		return KKSG::ERR_FAILED;
	}

	auto findmsg = find->second.find(temp);
	//find->second.erase(findmsg);
	if (findmsg == find->second.end())
	{
		return KKSG::ERR_FAILED;
	}
	_RemoveMsg(findmsg);

	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorMsg::OperateMsg(const KKSG::MentorMyBeAppliedMsgArg& arg, KKSG::MentorMyBeAppliedMsgRes& res)
{
	if(!arg.has_operation())
		return KKSG::ERR_FAILED;

	UINT32 error = KKSG::ERR_SUCCESS;
	switch(arg.operation())
	{
	case KKSG::EMentorMsgOp_Get:
		error = _GetMsg(res);
		break;
	case KKSG::EMentorMsgOpType_Clear:
		error = _ClearMsg(res);
		break;
	case KKSG::EMentorMsgOpType_Agree:
		error = _AgreeMsg(arg);
		break;
	case KKSG::EMentorMsgOpType_Reject:
		error = _RejectMsg(arg);
		break;
	default:
		break;
	}

	return error;
}

void MentorMsg::OverRemoveMsg(UINT64 srcID)
{
	for (auto iter = m_msgBeAppliedList.begin(); iter != m_msgBeAppliedList.end(); ++iter)
	{
		for (auto i = iter->second.begin(); i != iter->second.end();)
		{
			if (iter->first != KKSG::MentorMsgApplyInherit && iter->first != KKSG::MentorMsgApplyReportTask && iter->first != KKSG::MentorMsgApplyBreak)
			{
				break;
			}
			if (i->m_roleID == srcID)
			{
				iter->second.erase(i++);
			}
			else
				++i;
		}
	}
	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());
}

//客户端自己排序
UINT32 MentorMsg::_GetMsg(KKSG::MentorMyBeAppliedMsgRes& res)
{
	for (auto iter = m_msgBeAppliedList.begin(); iter != m_msgBeAppliedList.end(); ++iter)
	{
		if (iter->first == KKSG::MentorMsgApplyBreak)
		{
			continue;
		}

		for (auto iterMsg = iter->second.begin(); iterMsg != iter->second.end(); ++iterMsg)
		{
			KKSG::OneMentorBeAppliedMsg temp;
			if (!iterMsg->Convert2KKSGOneClientMsg(temp))
			{
				continue;
			}

			res.add_msglist()->CopyFrom(temp);
		}
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorMsg::_ClearMsg(KKSG::MentorMyBeAppliedMsgRes& res)
{
	for (auto iter = m_msgBeAppliedList.begin(); iter != m_msgBeAppliedList.end();)
	{
		if (iter->first == KKSG::MentorMsgApplyMaster || iter->first == KKSG::MentorMsgApplyStudent)
		{
			m_msgBeAppliedList.erase(iter++);
		}
		else
			++iter;
	}

	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorMsg::_AgreeMsg(const KKSG::MentorMyBeAppliedMsgArg& arg)
{
	if (arg.operatingalltask())
	{
		if (arg.msgtype() != KKSG::MentorMsgApplyReportTask)
		{
			return KKSG::ERR_FAILED;
		}

		auto find = m_msgBeAppliedList.find(KKSG::MentorMsgApplyReportTask);
		if(find == m_msgBeAppliedList.end())
			return KKSG::ERR_FAILED;

		while (!find->second.empty())
		{
			if(_AgreeOneMsg(*find->second.begin()) != KKSG::ERR_SUCCESS)
				break;
		}

		return KKSG::ERR_SUCCESS;
	}
	else
	{
		OneMentorMsg temp;
		temp.m_roleID = arg.roleid();
		temp.m_type = arg.msgtype();
		temp.m_paramid = arg.taskid();
		return _AgreeOneMsg(temp);
	}
}

UINT32 MentorMsg::_AgreeOneMsg(const OneMentorMsg& temp)
{
	int msgType = temp.m_type;

	auto find = m_msgBeAppliedList.find(msgType);
	if (find == m_msgBeAppliedList.end())
	{
		return KKSG::ERR_FAILED;
	}

	auto findMsg = find->second.find(temp);
	if (findMsg == find->second.end())
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 error = _HandelMsgAgreed(findMsg);

	if (msgType == KKSG::MentorMsgApplyBreak)
	{
		return KKSG::ERR_FAILED;
	}
	if (msgType == KKSG::MentorMsgApplyReportTask || msgType == KKSG::MentorMsgApplyInherit)
	{
		if (error != KKSG::ERR_SUCCESS)
		{
			return error;
		}
	}

	_RemoveMsg(findMsg);

	MentorShipOneSide* pSrc = MentorMgr::Instance()->GetMentorShip(temp.m_roleID);
	if (pSrc)
	{
		pSrc->GetMentorMsg()->RemoveApplyMsg(m_pMentorOneSide->GetOwnerID(), msgType);
	}

	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	if (error == KKSG::ERR_SUCCESS)
	{
		MentorMgr::Instance()->QuickSave(m_pMentorOneSide);
		MentorMgr::Instance()->QuickSave(pSrc);
	}

	return error;
}

UINT32 MentorMsg::_RejectMsg(const KKSG::MentorMyBeAppliedMsgArg& arg)
{
	if (!arg.has_roleid() || !arg.has_msgtype() || !arg.has_taskid())
	{
		return KKSG::ERR_FAILED;
	}

	int msgType = arg.msgtype();
	if (msgType != KKSG::MentorMsgApplyMaster && msgType != KKSG::MentorMsgApplyStudent)
	{
		return KKSG::ERR_FAILED;
	}

	auto find = m_msgBeAppliedList.find(msgType);
	if (find == m_msgBeAppliedList.end())
	{
		return KKSG::ERR_FAILED;
	}

	OneMentorMsg temp;
	temp.m_type = msgType;
	temp.m_roleID = arg.roleid();
	temp.m_paramid = arg.taskid();

	auto findMsg = find->second.find(temp);
	if (findMsg == find->second.end())
	{
		return KKSG::ERR_FAILED;
	}

	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	_RemoveMsg(findMsg);

	MentorShipOneSide* pSrc = MentorMgr::Instance()->GetMentorShip(temp.m_roleID);
	if (pSrc)
	{
		pSrc->GetMentorMsg()->RemoveApplyMsg(m_pMentorOneSide->GetOwnerID(), msgType);
	}

	return KKSG::ERR_SUCCESS;
}

void MentorMsg::_RemoveMsg(std::set<OneMentorMsg>::iterator iter)
{
	m_msgBeAppliedList[iter->m_type].erase(iter);
}

UINT32 MentorMsg::_HandelMsgAgreed(std::set<OneMentorMsg>::iterator findMsg)
{
	auto src = MentorMgr::Instance()->GetMentorShip(findMsg->m_roleID);
	int msgType = findMsg->m_type;
	if (!src)
	{
		return KKSG::ERR_FAILED;
	}
	if (msgType == KKSG::MentorMsgApplyStudent || msgType == KKSG::MentorMsgApplyMaster)
	{
		CRole* srcRole = CRoleManager::Instance()->GetByRoleID(src->GetOwnerID());
		if (!srcRole)
		{
			return KKSG::ERR_ROLE_NOT_ONLINE;
		}
		bool success = MentorRelationMgr::Instance()->BuildRelation(src, m_pMentorOneSide, msgType == KKSG::MentorMsgApplyStudent ? KKSG::EMentorPosStudent : KKSG::EMentorPosMaster);
		if (!success)
		{
			return KKSG::ERR_MENTOR_ALREADY_INRELATION;
		}
		return KKSG::ERR_SUCCESS;
	}
	else if (msgType == KKSG::MentorMsgApplyInherit || msgType == KKSG::MentorMsgApplyReportTask)
	{
		bool isin = m_pMentorOneSide->GetRelationList()->IsInMasterStudent(findMsg->m_roleID);
		if (!isin)
		{
			return KKSG::ERR_FAILED;
		}
	}
	if (msgType == KKSG::MentorMsgApplyReportTask)
	{
		bool success = src->GetMentorTask()->ConfirmTask(findMsg->m_paramid, /*findMsg->m_roleID*/m_pMentorOneSide->GetOwnerID());
		if (!success)
		{
			return KKSG::ERR_FAILED;
		}
	}
	else if (msgType == KKSG::MentorMsgApplyInherit)
	{
		auto success_result = CGuildInherit::Instance()->StartTeacherInherit(src->GetOwnerID(), m_pMentorOneSide->GetOwnerID());
		if (success_result != KKSG::ERR_SUCCESS)
		{
			return success_result;
		}
		UINT32 error = m_pMentorOneSide->GetRelationList()->Inherit(src);
		src->GetRelationList()->Inherit(m_pMentorOneSide);
		if (error != KKSG::ERR_SUCCESS)
		{
			return error;
		}
	}

	//MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());
	//MentorMgr::Instance()->AddChangedMentor(findMsg->m_roleID);

	return KKSG::ERR_SUCCESS;
}

bool MentorMsg::_RefreshRecommendList(int recomemndType)
{
	MentorRecommendList* pList = NULL;
	auto find = m_recommendList.find(recomemndType);
	if (find == m_recommendList.end())
	{
		pList = &m_recommendList[recomemndType];
		find = m_recommendList.find(recomemndType);
	}
	else
		pList = &find->second;

	int now = GameTime::GetTime();
	if (pList->m_refreshTime + MENTORRECOMMENDLIST_REFRESHTIME > now)
	{
		return false;
	}

	int applyStatus = KKSG::MentorApplyStatusMax;
	if (recomemndType == KKSG::MentorMsgApplyStudent)
		applyStatus = KKSG::MentorApplyStudent;
	else
		applyStatus = KKSG::MentorApplyMaster;

	std::vector<MentorShipOneSide*> list;
	if(!_GetRecommendRandResult(applyStatus, list))
	{
		return false;
	}
	
	find->second.m_refreshTime = now;
	find->second.m_recommendList.clear();
	foreach (i in list)
	{
		OneMentorRecommend temp;
		temp.m_type = applyStatus;
		temp.m_roleID = (*i)->GetOwnerID();
		find->second.m_recommendList.insert(std::make_pair(temp.m_roleID, temp));
	}

	//是否需要存库
	//MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	return true;
}

bool MentorMsg::_GetRecommendRandResult(int applyStatus, std::vector<MentorShipOneSide*>& vec)
{
	std::vector<MentorShipOneSide*> tempRecommendLists;
	for (auto iter = CRoleSummaryMgr::Instance()->Begin(); iter != CRoleSummaryMgr::Instance()->End(); ++iter)
	{
		OneMentorMsg temp;
		temp.m_roleID = iter->second->GetID();
		temp.m_type = applyStatus;

		if (_HaveApplied(temp))
		{
			continue;
		}
		
		MentorShipOneSide* pOneSide = MentorMgr::Instance()->GetMentorShip(iter->second->GetID());
		if (!pOneSide)
		{
			continue;
		}

		int tempapply = MentorRelationMgr::Instance()->GetRelationApplyStatus(m_pMentorOneSide, pOneSide);
		if (tempapply == applyStatus)
		{
			tempRecommendLists.push_back(pOneSide);
		}

		if (tempRecommendLists.size() >= 50)
		{
			break;
		}
	}

	if (tempRecommendLists.size() <= MENTORRECOMMENDLIST_SIZE)
	{
		vec = tempRecommendLists;
	}
	else
	{
		for (int i = 0; i < MENTORRECOMMENDLIST_SIZE; ++i)
		{
			UINT32 rand = XRandom::randInt(0, tempRecommendLists.size());
			if (rand >= tempRecommendLists.size())
			{
				continue;
			}

			vec.push_back(tempRecommendLists[rand]);
			tempRecommendLists.erase(tempRecommendLists.begin() + rand);
		}

	}

	return true;
}

void MentorMsg::_NotifyCanApply()
{
	int pos = m_pMentorOneSide->GetRelationList()->GetSelfPos();
	if (pos != 0)
	{
		return;
	}

	ApplyOneSideConditon oneCondition(m_pMentorOneSide, KKSG::EMentorPosStudent);
	if (oneCondition.IsValid())
	{
		PtcM2C_NotifyMentorApply ntf;
		ntf.m_Data.set_pos(KKSG::EMentorPosStudent);
		CRole* role = CRoleManager::Instance()->GetByRoleID(m_pMentorOneSide->GetOwnerID());
		if (role)
		{
			role->Send(ntf);
		}

		return;
	}

	ApplyOneSideConditon oneConditionM(m_pMentorOneSide, KKSG::EMentorPosMaster);
	if (oneConditionM.IsValid())
	{
		bool isLevelCanPrompt = false;
		UINT32 levelSealType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
		auto sealRowData = CLevelSealMgr::Instance()->GetLevelSealTypeData(levelSealType);
		if (sealRowData)
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pMentorOneSide->GetOwnerID());
			if (summary && summary->GetLevel() >= sealRowData->ApplyMasterPromptLevel)
			{
				isLevelCanPrompt = true;
			}
		}
		if (!isLevelCanPrompt)
		{
			return;
		}

		if (m_recommendList.find(KKSG::EMentorPosMaster) == m_recommendList.end())
		{
			return;
		}
		PtcM2C_NotifyMentorApply ntf;
		ntf.m_Data.set_pos(KKSG::EMentorPosMaster);
		CRole* role = CRoleManager::Instance()->GetByRoleID(m_pMentorOneSide->GetOwnerID());
		if (role)
		{
			role->Send(ntf);
		}

		return;
	}

}

void MentorMsg::_NotifyMsg()
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_pMentorOneSide->GetOwnerID());
	if (!role)
		return;

	bool hasMsg = false;
	bool hasInhertOrReport = false;
	bool change = false;
	PtcM2C_NotifyMentorApply ntf;
	for (auto iter = m_msgBeAppliedList.begin(); iter != m_msgBeAppliedList.end(); ++iter)
	{
		std::vector<std::set<OneMentorMsg>::iterator> vec;
		for (auto j = iter->second.begin(); j != iter->second.end(); ++j)
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(j->m_roleID);
			if (!summary)
			{
				continue;
			}
			if (iter->first == KKSG::MentorMsgApplyBreak)
			{
				if (j->m_paramcount > 1)
				{
					continue;
				}
				auto data = ntf.m_Data.add_appliedbreakinfos();
				data->set_roleid(j->m_roleID);
				data->set_rolename(summary->GetName());
				data->set_pos((KKSG::EMentorRelationPosition)m_pMentorOneSide->GetRelationList()->GetRolePos(j->m_roleID));
				data->set_breaktime(j->m_time + MENTORAUTOBREAKTIME);
				vec.push_back(j);
				hasMsg = true;
			}
			else if (iter->first == KKSG::MentorMsgApplyInherit || iter->first == KKSG::MentorMsgApplyReportTask)
			{
				hasMsg = true;
				hasInhertOrReport = true;
				break;
			}
			else
			{
				hasMsg = true;
				break;
			}
		}
		for (auto vecBegin = vec.begin(); vecBegin != vec.end(); ++vecBegin)
		{
			OneMentorMsg temp = **vecBegin;
			++temp.m_paramcount;
			iter->second.erase(*vecBegin);
			iter->second.insert(temp);
			change = true;
		}
	}

	if (hasMsg)
	{
		ntf.m_Data.set_hasmsg(true);
		if (hasInhertOrReport)
		{
			ntf.m_Data.set_hasinheritorreporttask(true);
		}

		role->Send(ntf);

		if (hasInhertOrReport)
		{
			PtcM2C_HintNotifyMS hintntf;
			hintntf.m_Data.add_systemid(SYS_MENTOR);
			role->Send(hintntf);
		}
	}
	if (change)
	{
		MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());
	}
}

void MentorMsg::_NotifyStudentHint()
{
	if (m_pMentorOneSide->GetRelationList()->GetSelfPos() != KKSG::EMentorPosStudent)
	{
		return;
	}
	bool notify = false;
	notify = m_pMentorOneSide->GetMentorTask()->CanReportTask();
	if(!notify)
	{
		notify = m_pMentorOneSide->GetRelationList()->CanInherit();
	}

	if (!notify)
	{
		return;
	}

	CRole* role = CRoleManager::Instance()->GetByRoleID(m_pMentorOneSide->GetOwnerID());
	if (role)
	{
		PtcM2C_HintNotifyMS hintntf;
		hintntf.m_Data.add_systemid(SYS_MENTOR);
		role->Send(hintntf);
	}
}

void MentorMsg::_LoadBreakApplyMsg(OneMentorMsg& msg)
{
	if (msg.m_type != KKSG::MentorMsgApplyBreak)
	{
		return;
	}

	BreakApplyPair pair;
	pair.m_srcID = msg.m_roleID;
	pair.m_destID = m_pMentorOneSide->GetOwnerID();
	//pair.m_time = msg.m_time;
	MentorMgr::Instance()->AddAutoBreak(pair, msg.m_time);
}

void MentorMsg::_SetRecommendApply(UINT64 destRoleID, int type)
{
	auto find = m_recommendList.find(type);
	if (find == m_recommendList.end())
	{
		return;
	}

	auto findRec = find->second.m_recommendList.find(destRoleID);
	if (findRec == find->second.m_recommendList.end())
	{
		return;
	}

	findRec->second.m_isApply = true;
}

void MentorMsg::DebugClear(UINT64 roleID, int type)
{
	auto find = m_msgBeAppliedList.find(type);
	if (find == m_msgBeAppliedList.end())
	{
		return;
	}
	if (roleID == 0)
	{
		m_msgBeAppliedList.erase(type);
	}
	else
	{
		for (auto iter = find->second.begin(); iter != find->second.end();)
		{
			if (iter->m_roleID == roleID)
			{
				find->second.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}
}

bool MentorMsg::_HaveApplied(OneMentorMsg& msg)
{
	auto findType = m_msgApplyList.find(msg.m_type);
	if (findType != m_msgApplyList.end())
	{
		if (findType->second.find(msg) != findType->second.end())
		{
			return true;
		}
	}

	return false;
}

