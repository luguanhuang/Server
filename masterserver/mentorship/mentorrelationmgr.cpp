#include "pch.h"
#include <time.h>
#include "mentorrelationmgr.h"
#include "mentormgr.h"
#include "mentoroneside.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "mentorrelationlist.h"
#include "mentormsg.h"
#include "mentortask.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "levelseal/levelsealMgr.h"
#include "friend/friendop.h"
#include "event/eventmgr.h"
#include "util/gametime.h"

INSTANCE_SINGLETON(MentorRelationMgr)

ApplyOneSideConditon::ApplyOneSideConditon(MentorShipOneSide* oneSide, int pos)
{
	m_pOneSide = oneSide;
	m_pos = pos;
}

bool ApplyOneSideConditon::IsValid()
{
//#ifdef TEST
//	return true;
//#endif
	if (IsLevelValid() && IsPosValid() && IsTimeValid())
	{
		return true;
	}

	return false;
}

bool ApplyOneSideConditon::IsLevelValid()
{
	if (!m_pOneSide)
	{
		return false;
	}

	auto summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pOneSide->GetOwnerID());
	if (!summary)
	{
		return false;
	}
	
	if (!m_pOneSide->IsOpen())
	{
		return false;
	}

	UINT32 levelSealType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	auto sealRowData = CLevelSealMgr::Instance()->GetLevelSealTypeData(levelSealType);
	if (!sealRowData)
	{
		return false;
	}

	int level = summary->GetLevel();
	int studentLevel = 0;
	int masterLevel = 0;
	if (m_pos == KKSG::EMentorPosMaster)
	{
		studentLevel = level;
		if (studentLevel < sealRowData->ApplyMasterMaxLevel.seq[0] || studentLevel > sealRowData->ApplyMasterMaxLevel.seq[1])
		{
			return false;
		}
	}
	else if (m_pos == KKSG::EMentorPosStudent)
	{
		masterLevel = level;
		if (sealRowData->ApplyStudentLevel > masterLevel)
		{
			return false;
		}
	}

	return true;

}

bool ApplyOneSideConditon::IsPosValid()
{
	if (!m_pOneSide)
	{
		return false;
	}

	if (m_pos <=0 || m_pos >= KKSG::EMentorPosMax)
		return false;

	if (!m_pOneSide->GetIsNeedMentor())
	{
		return false;
	}

	MentorShipOneSide* pStudent = NULL;
	MentorShipOneSide* pMaster = NULL;
	if (m_pos == KKSG::EMentorPosMaster)
	{
		pStudent = m_pOneSide;
		if (pStudent->GetRelationList()->GetSelfPos() == KKSG::EMentorPosMaster)
		{
			return false;
		}
		if (pStudent->GetRelationList()->GetMentorRelationCurNum(KKSG::EMentorPosMaster) >= MENTORRELATION_CURMAXNUM)
		{
			return false;
		}
		if (pStudent->HasCompleteMentor())
		{
			return false;
		}
	}
	else
	{
		pMaster = m_pOneSide;
		if (pMaster->GetRelationList()->GetSelfPos() == KKSG::EMentorPosStudent)
		{
			return false;
		}
		if (pMaster->GetRelationList()->GetMentorRelationCurNum(KKSG::EMentorPosStudent) >= MENTORRELATION_CURMAXNUM)
		{
			return false;
		}
	}

	return true;

}

bool ApplyOneSideConditon::IsTimeValid()
{
	if (!m_pOneSide)
	{
		return false;
	}
	MentorShipOneSide* pStudent = NULL;
	MentorShipOneSide* pMaster = NULL;
	if (m_pos == KKSG::EMentorPosMaster)
	{
		pStudent = m_pOneSide;
		if (pStudent->GetRelationList()->GetPunishPeriod())
		{
			return false;
		}
	}
	else
	{
		pMaster = m_pOneSide;
		if (pMaster->GetRelationList()->GetPunishPeriod())
		{
			return false;
		}
		if (pMaster->GetRelationList()->GetIntervalPeriod())
		{
			return false;
		}
	}
	return true;
}

ApplyConditon::ApplyConditon(MentorShipOneSide* src, MentorShipOneSide* dest, int pos)
{
	m_pSrc = src;
	m_pDest = dest;
	m_pos = pos;
}

bool ApplyConditon::IsValid()
{
//#ifdef TEST
//	return true;
//#endif
	ApplyOneSideConditon oneSide(m_pSrc, m_pos);
	if (!oneSide.IsValid())
	{
		return false;
	}
	ApplyOneSideConditon otherSide(m_pDest, MentorRelationMgr::Instance()->GetOppositePos(m_pos));
	if (!otherSide.IsValid())
	{
		return false;
	}
	if (/*IsLevelValid() && */IsPosValid() && IsTimeValid())
	{
		return true;
	}

	return false;
}

bool ApplyConditon::IsLevelValid()
{
	if (!m_pSrc || !m_pDest)
	{
		return false;
	}

	auto srcSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pSrc->GetOwnerID());
	auto destSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pDest->GetOwnerID());
	if (!srcSummary || !destSummary)
	{
		return false;
	}
	
	int srcLevel = srcSummary->GetLevel();
	int destLevel = destSummary->GetLevel();
	int studentLevel = 0;
	int masterLevel = 0;
	if (m_pos == KKSG::EMentorPosMaster)
	{
		studentLevel = srcLevel;
		masterLevel = destLevel;
	}
	else if (m_pos == KKSG::EMentorPosStudent)
	{
		studentLevel = destLevel;
		masterLevel = srcLevel;
	}
	if (masterLevel- studentLevel < 10)
	{
		return false;
	}

	return true;
}

bool ApplyConditon::IsPosValid()
{
	if (!m_pSrc || !m_pDest)
	{
		return false;
	}

	if (m_pos <=0 || m_pos >= KKSG::EMentorPosMax)
		return false;

	if (m_pSrc->GetRelationList()->GetRolePos(m_pDest->GetOwnerID()) != KKSG::EMentorPosMax \
		&& m_pSrc->GetRelationList()->GetCurStatus(m_pDest->GetOwnerID()) != KKSG::MentorRelationBreak)
	{
		return false;
	}
	
	if (m_pDest->GetRelationList()->GetRolePos(m_pSrc->GetOwnerID()) != KKSG::EMentorPosMax \
		&& m_pDest->GetRelationList()->GetCurStatus(m_pSrc->GetOwnerID()) != KKSG::MentorRelationBreak)
	{
		return false;
	}
	MentorShipOneSide* pStudent = NULL;
	MentorShipOneSide* pMaster = NULL;
	if (m_pos == KKSG::EMentorPosMaster)
	{
		pStudent = m_pSrc;
		pMaster = m_pDest;
	}
	else
	{
		pStudent = m_pDest;
		pMaster = m_pSrc;
	}

	if (pMaster->GetRelationList()->GetRolePos(pStudent->GetOwnerID()) == KKSG::EMentorPosMaster)
	{
		return false;
	}
	if (pStudent->GetRelationList()->GetRolePos(pMaster->GetOwnerID()) == KKSG::EMentorPosStudent)
	{
		return false;
	}

	return true;
}

bool ApplyConditon::IsTimeValid()
{
	return true;
}

MentorRelationMgr::MentorRelationMgr()
{

}

MentorRelationMgr::~MentorRelationMgr()
{

}

bool MentorRelationMgr::Init()
{
	return true;
}

void MentorRelationMgr::Uninit()
{

}

KKSG::MentorApplyStatus  MentorRelationMgr::GetRelationApplyStatus(MentorShipOneSide* src, MentorShipOneSide* dest)
{
	if (!src || !dest)
	{
		return KKSG::MentorApplyStatusMax;
	}

	auto srcSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(src->GetOwnerID());
	auto destSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(dest->GetOwnerID());
	if (!srcSummary || !destSummary)
	{
		return KKSG::MentorApplyStatusMax;
	}
	
	int pos = 0;
	auto relation = src->GetRelationList()->GetRelation(dest->GetOwnerID());
	if (relation)
	{
		int status = relation->GetCurStatus();
		if (status == KKSG::MentorRelationIn || status == KKSG::MentorRelationBreakApply || status == KKSG::MentorRelationComplete)
		{
			return KKSG::MentorApplyHas;
		}
		else if (status == KKSG::MentorRelationBreak)
		{
			pos = relation->m_pos;
		}
	}

	if (!pos)
	{
		if (srcSummary->GetLevel() > destSummary->GetLevel())
		{
			pos = KKSG::EMentorPosStudent;
		}
		else if (srcSummary->GetLevel() < destSummary->GetLevel())
		{
			pos = KKSG::EMentorPosMaster;
		}
	}

	if (!pos)
	{
		return KKSG::MentorApplyStatusMax;
	}

	if (_CalRelationApplyPos(src, dest, pos))
	{	
		if (pos == KKSG::EMentorPosMaster)
		{
			return KKSG::MentorApplyMaster;
		}
		else
			return KKSG::MentorApplyStudent;
	}

	return KKSG::MentorApplyStatusMax;
}

bool MentorRelationMgr::BuildRelation(MentorShipOneSide* src, MentorShipOneSide* dest, int pos)
{
	KKSG::MentorApplyStatus status = GetRelationApplyStatus(src, dest);
	if (!((pos == KKSG::EMentorPosMaster && status == KKSG::MentorApplyMaster) || (pos == KKSG::EMentorPosStudent && status == KKSG::MentorApplyStudent)))
	{
		return false;
	}

	src->GetRelationList()->AddRelation(dest->GetOwnerID(), src->GetOwnerID(), pos);
	dest->GetRelationList()->AddRelation(src->GetOwnerID(), src->GetOwnerID(), GetOppositePos(pos));

	MentorShipOneSide* student = NULL;
	MentorShipOneSide* master = NULL;
	if (src->GetRelationList()->GetSelfPos() == KKSG::EMentorPosMaster)
	{
		student = dest;
		master = src;
	}
	else
	{
		student = src;
		master = dest;
	}

	student->GetMentorTask()->Init();

	FriendOp opStudent(student->GetOwnerID());
	UINT32 degree = opStudent.GetDegree(master->GetOwnerID());
	if (degree)
	{
		EventMgr::Instance()->AddEventOnlyOnMS(student->GetOwnerID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_MentorIntimacy, 0, degree);
	}

	FriendOp op(src->GetOwnerID());
	op.SetOther(dest->GetOwnerID());
	op.AddFriend();

	FriendOp opDest(dest->GetOwnerID());
	opDest.SetOther(src->GetOwnerID());
	opDest.AddFriend();

	return true;
}

UINT32 MentorRelationMgr::ApplyRelation(MentorShipOneSide* src, MentorShipOneSide* dest, int pos)
{
	KKSG::MentorApplyStatus status = GetRelationApplyStatus(src, dest);
	if (!((pos == KKSG::EMentorPosMaster && status == KKSG::MentorApplyMaster) || (pos == KKSG::EMentorPosStudent && status == KKSG::MentorApplyStudent)))
	{
		if (pos == KKSG::EMentorPosMaster)
		{
			return KKSG::ERR_MENTOR_CUR_STATUS_CANNOT_APPLYMASTER;
		}
		else
		{
			return KKSG::ERR_MENTOR_CUR_STATUS_CANNOT_APPLYSTUDENT;
		}
	}

	int type = 0;
	if (pos == KKSG::EMentorPosMaster)
	{
		type = KKSG::MentorMsgApplyMaster;
	}
	else
	{
		type = KKSG::MentorMsgApplyStudent;
	}

	UINT32 error = dest->GetMentorMsg()->AddBeApplyMsg(src->GetOwnerID(), type);

	if(error != KKSG::ERR_SUCCESS)
		return error;

	src->GetMentorMsg()->AddApplyMsg(dest->GetOwnerID(), type);

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorRelationMgr::ApplyBreak(MentorShipOneSide* src, MentorShipOneSide* dest)
{
	if (!src || !dest)
	{
		return KKSG::ERR_FAILED;
	}

	int now = GameTime::GetTime();
	UINT32 error = src->GetRelationList()->ApplyBreak(dest->GetOwnerID(), src->GetOwnerID(), now);
	if (error == KKSG::ERR_SUCCESS)
	{
		dest->GetRelationList()->ApplyBreak(src->GetOwnerID(), src->GetOwnerID(), now);

		dest->GetMentorMsg()->AddBeApplyMsg(src->GetOwnerID(), KKSG::MentorMsgApplyBreak);

		BreakApplyPair pair;
		pair.m_srcID = src->GetOwnerID();
		pair.m_destID = dest->GetOwnerID();
		//pair.m_time = now;
		MentorMgr::Instance()->AddAutoBreak(pair, now);

		MentorMgr::Instance()->QuickSave(src);
		MentorMgr::Instance()->QuickSave(dest);

	}

	return error;
}

UINT32 MentorRelationMgr::ApplyBreakCancel(MentorShipOneSide* src, MentorShipOneSide* dest)
{
	if (!src || !dest)
	{
		return KKSG::ERR_FAILED;
	}

	int now = GameTime::GetTime();
	UINT32 error = src->GetRelationList()->ApplyBreakCancel(dest->GetOwnerID(), src->GetOwnerID());
	if (error == KKSG::ERR_SUCCESS)
	{
		dest->GetRelationList()->ApplyBreakCancel(src->GetOwnerID(), src->GetOwnerID());

		dest->GetMentorMsg()->RemoveBeApplyMsg(src->GetOwnerID(), KKSG::MentorMsgApplyBreak);

		BreakApplyPair pair;
		pair.m_srcID = src->GetOwnerID();
		pair.m_destID = dest->GetOwnerID();
		//pair.m_time = now;
		MentorMgr::Instance()->RemoveAutoBreak(pair);

		MentorMgr::Instance()->QuickSave(src);
		MentorMgr::Instance()->QuickSave(dest);
	}

	return error;
}

UINT32 MentorRelationMgr::NormalComplete(MentorShipOneSide* src, MentorShipOneSide* dest)
{
	if (!src || !dest)
	{
		return KKSG::ERR_FAILED;
	}
	CRole* destRole = CRoleManager::Instance()->GetByRoleID(dest->GetOwnerID());
	if (!destRole)
	{
		return KKSG::ERR_ROLE_NOT_ONLINE;
	}

	if (src->GetRelationList()->GetSelfPos() != KKSG::EMentorPosMaster)
	{
		return KKSG::ERR_FAILED;
	}

	return _Complete(src, dest, false);
}

UINT32 MentorRelationMgr::ForceComplete(MentorShipOneSide* src, MentorShipOneSide* dest)
{
	if (!src || !dest)
	{
		return KKSG::ERR_FAILED;
	}

	CRole* destRole = CRoleManager::Instance()->GetByRoleID(dest->GetOwnerID());
	if (destRole)
	{
		return KKSG::ERR_MENTOR_OTHER_ONLINE;
	}

	UINT32 destOfflineTime = 0;
	int now = GameTime::GetTime();

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(dest->GetOwnerID());
	if (!summary)
	{
		destOfflineTime = -1;
	}
	else
	{
		destOfflineTime = summary->GetLastLogout();
	}
	if (destOfflineTime < RELATIONCOMPELETEMFORCETIME)
	{
		return KKSG::ERR_MENTOR_COMPLETE_OTHER_OFFLINE_NEED_DAYS;
	}


	return _Complete(src, dest, true);
}

UINT32 MentorRelationMgr::AutoBreak(MentorShipOneSide* src, MentorShipOneSide* dest)
{
	if (!src || !dest)
	{
		return KKSG::ERR_FAILED;
	}
	int now = GameTime::GetTime();
	UINT32 error = src->GetRelationList()->Break(dest->GetOwnerID(), now);
	if (error == KKSG::ERR_SUCCESS)
	{
		dest->GetRelationList()->Break(src->GetOwnerID(), now);

		MentorMgr::Instance()->QuickSave(src);
		MentorMgr::Instance()->QuickSave(dest);
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MentorRelationMgr::ApplyInherit(MentorShipOneSide* src, MentorShipOneSide* dest)
{
	if (!src || !dest)
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 error = src->GetRelationList()->ApplyInherit(dest, src->GetOwnerID());
	if (error == KKSG::ERR_SUCCESS)
	{
		dest->GetRelationList()->ApplyInherit(src, src->GetOwnerID());
	}
	else
	{
		return error;
	}

	dest->GetMentorMsg()->AddBeApplyMsg(src->GetOwnerID(), KKSG::MentorMsgApplyInherit);

	//MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());
	MentorMgr::Instance()->QuickSave(src);
	MentorMgr::Instance()->QuickSave(dest);

	return error;
}

int MentorRelationMgr::GetOppositePos(int pos)
{
	if (pos == KKSG::EMentorPosMaster)
	{
		return KKSG::EMentorPosStudent;
	}
	else if (pos == KKSG::EMentorPosStudent)
	{
		return KKSG::EMentorPosMaster;
	}

	return KKSG::EMentorPosMax;
}

bool MentorRelationMgr::_CalRelationApplyPos(MentorShipOneSide* src, MentorShipOneSide* dest, int pos)
{
	ApplyConditon conditon(src, dest, pos);

	return conditon.IsValid();
}


UINT32 MentorRelationMgr::_Complete(MentorShipOneSide* src, MentorShipOneSide* dest, bool isForce)
{
	if (!src || !dest)
	{
		return KKSG::ERR_FAILED;
	}

	if (!src->GetRelationList()->IsInMasterStudent(dest->GetOwnerID()))
	{
		return KKSG::ERR_FAILED;
	}
	if (src->GetRelationList()->GetAlreadyMasterStudentTime(dest->GetOwnerID()) < RELATIONCOMPELETEMTIME)
	{
		return KKSG::ERR_MENTOR_COMPLETE_IN_RELATION_NEED_DAYS;
	}
	MentorShipOneSide* pStudent = NULL;
	if (!isForce && src->GetRelationList()->GetSelfPos() != KKSG::EMentorPosMaster)
	{
		return KKSG::ERR_FAILED;
	}
	if (!isForce)
	{
		pStudent = dest;
	}
	else
	{
		if (src->GetRelationList()->GetSelfPos() == KKSG::EMentorPosMaster)
			pStudent = dest;
		else
			pStudent = src;

	}

	int num = pStudent->GetMentorTask()->GetTaskCompleteNum();
	if (num < GetGlobalConfig().MentorNormalCompleteNum)
	{
		return KKSG::ERR_FAILED;
	}
	int maxnum = GetGlobalConfig().MentorPerfectCompleteNum;
	int type = 1;
	if (num >= maxnum)
	{
		type = 2;
	}
	int now = GameTime::GetTime();
	src->GetRelationList()->Complete(dest->GetOwnerID(), now, type);
	dest->GetRelationList()->Complete(src->GetOwnerID(), now, type);

	src->GetMentorMsg()->RemoveBeApplyMsg(src->GetOwnerID(), KKSG::MentorMsgApplyBreak);

	MentorMgr::Instance()->QuickSave(src);
	MentorMgr::Instance()->QuickSave(dest);

	LogInfo("mentor, student %llu, complete, force:%u, %llu, %llu", pStudent->GetOwnerID(), isForce ? 1 : 0 , src->GetOwnerID(), dest->GetOwnerID());

	return KKSG::ERR_SUCCESS;
}

