#include "pch.h"
#include "mentorship/mentoroneside.h"
#include "mentorrelationlist.h"
#include "mentortask.h"
#include "mentormsg.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"
#include "levelseal/levelsealMgr.h"
#include "table/globalconfig.h"
#include "mentormgr.h"
#include "mentorrelationmgr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "util/gametime.h"


MentorShipOneSide::MentorShipOneSide(UINT64 roleID)
{
	m_roleID = roleID;

	m_updateTime = 0;
	m_mentorAudioID = 0;
	m_isNeedStudent = true;
	m_isOnlineNotify = true;

	m_mentorRelation = new MentorRelationList(this);
	m_mentorTask = new MentorTask(this);
	m_mentorMsg = new MentorMsg(this);

	//not save
	m_hasCompleteMentor = false;
	m_menteeCompleteNum = 0;
}

MentorShipOneSide::~MentorShipOneSide()
{

}

void MentorShipOneSide::Load(const KKSG::MentorAllData2Db& data)
{
	m_updateTime = data.updatetime();
	m_mentorAudioID = data.mentoraudioid();
	m_isNeedStudent = data.isneedstudent();
	m_isOnlineNotify = data.isonlinenotify();
	if (data.has_mentorwords())
	{
		m_mentorWords = data.mentorwords();
	}
	m_hasCompleteMentor = data.hascompletementor();
	m_menteeCompleteNum = data.menteecompletenum();

	m_mentorRelation->Load(data);
	m_mentorTask->Load(data);
	m_mentorMsg->Load(data);
}

void MentorShipOneSide::Save(KKSG::MentorAllData2Db& data)
{
	data.set_roleid(m_roleID);
	data.set_updatetime(m_updateTime);
	data.set_mentoraudioid(m_mentorAudioID);
	data.set_isneedstudent(m_isNeedStudent);
	data.set_isonlinenotify(m_isOnlineNotify);
	data.set_mentorwords(m_mentorWords);
	data.set_hascompletementor(m_hasCompleteMentor);
	data.set_menteecompletenum(m_menteeCompleteNum);
	
	m_mentorRelation->Save(data);
	m_mentorTask->Save(data);
	m_mentorMsg->Save(data);
}

/*void MentorShipOneSide::SetChanged()
{
	m_switch.Set();
}*/

bool MentorShipOneSide::IsOpen()
{
	auto findSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleID);
	if (!findSummary)
	{
		return false;
	}

	auto data = OpenSystemMgr::Instance()->GetSystemRowData(SYS_MENTOR);
	if (!data)
	{
		return false;
	}

	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_MENTOR))
	{
		return false;
	}

	//需要修改,system的配置项,按照等级来
	if (findSummary->GetLevel() >= data->PlayerLevel && CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel >= GetGlobalConfig().MentorOpenSealLevel\
		&& data->IsOpen)
	{
		return true;
	}

	return false;
}

void MentorShipOneSide::DoOpen()
{
	m_mentorTask->DoOpen();

	LogInfo("%llu open mentorship", GetOwnerID());
}

void MentorShipOneSide::OnRoleLogin()
{
	if (!IsOpen())
	{
		return;
	}

	m_mentorTask->Init();

	GetMentorMsg()->OnLogin();
}

bool MentorShipOneSide::HasCompleteMentor()
{
	return m_hasCompleteMentor;
}

UINT32 MentorShipOneSide::ClientGetMentorInfo(KKSG::GetMyMentorInfoRes& roRes)
{
	UINT32 error = KKSG::ERR_SUCCESS;
	roRes.set_audioid(m_mentorAudioID);
	roRes.set_mentorwords(m_mentorWords);
	roRes.set_isneedstudent(m_isNeedStudent);
	int pos = GetRelationList()->GetSelfPos();
	if (pos == KKSG::EMentorPosMaster)
	{
		error = GetRelationList()->Convert2ClientRelation(roRes);
	}
	else if (pos == KKSG::EMentorPosStudent)
	{
		error = GetRelationList()->Convert2ClientRelation(roRes);
		GetMentorTask()->Convert2SelfClient(*roRes.mutable_mentorselfinfo());
	}
	else
		return KKSG::ERR_SUCCESS;

	return error;
}

UINT32 MentorShipOneSide::UpdateSelfInfo(const KKSG::UpdateMentorApplyStudentInfoArg& arg)
{
	UINT32 error = KKSG::ERR_SUCCESS;
	if (arg.has_audioid())
	{
		m_mentorAudioID = arg.audioid();
	}
	if (arg.has_isneedstudent())
	{
		m_isNeedStudent = arg.isneedstudent();
	}
	if (arg.has_onlinenotify())
	{
		m_isOnlineNotify = arg.onlinenotify();
		m_updateTime = GameTime::GetTime();
	}
	if (arg.has_applywords())
	{
		if (arg.applywords().length() <= 50)
		{
			m_mentorWords = arg.applywords();
		}
		else
		{
			error = KKSG::ERR_DECLARATION_TOO_LONG;
		}
	}

	MentorMgr::Instance()->AddChangedMentor(m_roleID);

	return error;
}

UINT32 MentorShipOneSide::ClientReqestRelationOp(const KKSG::MentorRelationOpArg& arg)
{
	//需要处理
	if (!arg.has_operation() || !arg.has_destroleid())
	{
		return KKSG::ERR_FAILED;
	}
	MentorShipOneSide* destSide = MentorMgr::Instance()->GetMentorShip(arg.destroleid(), false);
	if (!destSide)
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 error = KKSG::ERR_SUCCESS;
	switch(arg.operation())
	{
	case KKSG::MentorRelationOp_ApplyMaster:
		{
			error = MentorRelationMgr::Instance()->ApplyRelation(this, destSide, KKSG::EMentorPosMaster);
		}
		break;
	case KKSG::MentorRelationOp_ApplyStudent:
		{
			error = MentorRelationMgr::Instance()->ApplyRelation(this, destSide, KKSG::EMentorPosStudent);
		}
		break;
	case KKSG::MentorRelationOp_Inherit:
		{
			error = MentorRelationMgr::Instance()->ApplyInherit(this, destSide);
		}
		break;
	case KKSG::MentorRelationOp_ReportTask:
		{
			if (!arg.has_reporttaskid())
			{
				error = KKSG::ERR_FAILED;
				break;
			}

			bool success = GetMentorTask()->ReportTask(arg.reporttaskid(), arg.destroleid());
			if (!success)
			{
				error = KKSG::ERR_FAILED;
			}
		}
		break;
	case KKSG::MentorRelationOp_ReportAllTask:
		{
			bool success = GetMentorTask()->ReportAllTask(arg.destroleid());
			if (!success)
			{
				error = KKSG::ERR_FAILED;
			}
		}
		break;
	case KKSG::MentorRelationOp_Break:
		{
			error = MentorRelationMgr::Instance()->ApplyBreak(this, destSide);
		}
		break;
	case KKSG::MentorRelationOp_BreakCancel:
		{
			error = MentorRelationMgr::Instance()->ApplyBreakCancel(this, destSide);
		}
		break;
	case KKSG::MentorRelationOp_NormalComplete:
		{
			error = MentorRelationMgr::Instance()->NormalComplete(this, destSide);
		}
		break;
	case KKSG::MentorRelationOp_ForceComplete:
		{
			error = MentorRelationMgr::Instance()->ForceComplete(this, destSide);
		}
		break;
	default:
		break;
	}

	if (error == KKSG::ERR_SUCCESS)
	{
		LogInfo("%llu, apply %llu, op:%u", m_roleID, destSide->GetOwnerID(), arg.operation());
	}

	return error;
}

bool MentorShipOneSide::CanOnlinNotifyApply()
{
	if (m_isOnlineNotify)
	{
		return true;
	}
	if (GameTime::GetTime() - m_updateTime > 7 * 86400)
	{
		return true;
	}

	return false;
}

void MentorShipOneSide::AddMentorCompleteNum(int num)
{
	m_menteeCompleteNum += num;

	EventMgr::Instance()->AddEvent(m_roleID, ACH_EVENT_OFFSET + ACHIEVE_COM_MENTOR, m_menteeCompleteNum);
	EventMgr::Instance()->AddEvent(m_roleID, DESIGNATION_COM_MENTOR, m_menteeCompleteNum);
}

void MentorShipOneSide::SetSelfCompleteMentor(bool complete)
{
	m_hasCompleteMentor = complete;
}
