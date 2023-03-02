#include "pch.h"
#include <time.h>
#include "marriagerelationmgr.h"
#include "marriagemgr.h"
#include "marriage.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "levelseal/levelsealMgr.h"
#include "friend/friendop.h"
#include "event/eventmgr.h"
#include "util/gametime.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammember.h"
#include "marriage//marriagemsg.h"
#include "marriage/ptcm2c_notifymarriageapply.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "marriage/ptcm2c_notifymarriagedivorceapply.h"
#include "friend/friend.h"
#include "friend/friendmgr.h"
#include "common/rpcm2g_msgiveitem.h"
#include "define/tlogenum.h"
#include "network/gslink.h"
#include "event/eventmgr.h"
#include "loghelper/tlogr.h"

INSTANCE_SINGLETON(MarriageRelationMgr)

MarriageApplyOne::MarriageApplyOne(Marriage* oneSide)
{
	m_pOneSide = oneSide;
}

UINT32 MarriageApplyOne::IsValid()
{
	UINT32 error = IsLevelValid();
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	error = IsPosValid();
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	return error;
}

UINT32 MarriageApplyOne::IsLevelValid()
{
	if (!m_pOneSide)
	{
		return KKSG::ERR_FAILED;
	}

	auto summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pOneSide->GetOwnerID());
	if (!summary)
	{
		return KKSG::ERR_FAILED;
	}
	
	if (!m_pOneSide->IsOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageApplyOne::IsPosValid()
{
	if (!m_pOneSide)
	{
		return KKSG::ERR_FAILED;
	}

	if (m_pOneSide->IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NEED_TWO_NOTMARRY;
	}

	return KKSG::ERR_SUCCESS;
}

MarriageCondition::MarriageCondition(Marriage* src, Marriage* dest)
{
	m_pSrc = src;
	m_pDest = dest;
}

UINT32 MarriageCondition::IsValid()
{
	if(!m_pSrc || !m_pDest)
		return KKSG::ERR_FAILED;

	CRole* roleSrc = CRoleManager::Instance()->GetByRoleID(m_pSrc->GetOwnerID());
	CRole* roleDest = CRoleManager::Instance()->GetByRoleID(m_pDest->GetOwnerID());
	if (!roleSrc || !roleDest)
	{
		return KKSG::ERR_FAILED;
	}

	MarriageApplyOne oneSide(m_pSrc);
	UINT32 error = oneSide.IsValid();
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	MarriageApplyOne otherSide(m_pDest);
	error = otherSide.IsValid();
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	return KKSG::ERR_SUCCESS;
}

MarriageRelationMgr::MarriageRelationMgr()
{

}

MarriageRelationMgr::~MarriageRelationMgr()
{

}

bool MarriageRelationMgr::Init()
{
	return true;
}

void MarriageRelationMgr::Uninit()
{

}


UINT32 MarriageRelationMgr::HandleRelationOp(CRole* role, const KKSG::MarriageRelationOpArg& roArg, DelayMarriageProcessInfo& delayInfo)
{
	UINT32 error = KKSG::ERR_FAILED;
	switch(roArg.optype())
	{
	case KKSG::MarriageOpType_MarryApply:
		{
			error = PreApplyRelation(role, delayInfo.roleID);
		}
		break;
	case KKSG::MarriageOpType_MarryAgree:
		{
			error = PreAgreeApplyRelation(role, roArg.destroleid(), delayInfo.type);
		}
		break;
	case KKSG::MarriageOpType_MarryRefuse:
		{
			error = RefuseApplyRelation(role, roArg.destroleid());
		}
		break;
	case KKSG::MarriageOpType_Divorce:
		{
			error = PreApplyDivorce(role, delayInfo.toGs);
		}
		break;
	case KKSG::MarriageOpType_DivorceCancel:
		{
			error = ApplyDivorceCancel(role);
		}
		break;
	default:
		break;
	}

	return error;
}

UINT32 MarriageRelationMgr::DelayProcess(CRole* role, UINT64 srcRoleID, UINT64 destRoleID, const KKSG::MarriageRelationOpArg& roArg, KKSG::MarriageRelationOpRes& roRes)
{
	if (roArg.optype() == KKSG::MarriageOpType_MarryApply)
	{
		UINT64 teamDestRoleID = 0;
		UINT32 error = MarriageRelationMgr::Instance()->PreApplyRelation(role, teamDestRoleID);
		if (error != KKSG::ERR_SUCCESS)
		{
			return error;
		}

		UINT64 checkRoleID2 = teamDestRoleID;
		if (checkRoleID2 != destRoleID)
		{
			return KKSG::ERR_MARRIAGE_NEED_TWO_NOTMARRY;
		}

		error = MarriageRelationMgr::Instance()->ApplyRelation(role, destRoleID, roArg.type());
		if (error != KKSG::ERR_SUCCESS)
		{
			return error;
		}
	}

	if (roArg.optype() == KKSG::MarriageOpType_MarryAgree)
	{
		KKSG::WeddingType tempType = KKSG::WeddingType_Normal;
		UINT32 error = MarriageRelationMgr::Instance()->PreAgreeApplyRelation(role, srcRoleID, tempType);
		if (error != KKSG::ERR_SUCCESS)
		{
			return error;
		}

		error = MarriageRelationMgr::Instance()->AgreeApplyRelation(role, srcRoleID);
		if (error != KKSG::ERR_SUCCESS)
		{
			return error;
		}

		CRole* srcRole = CRoleManager::Instance()->GetByRoleID(srcRoleID);
		if (srcRole)
		{
			roRes.set_opporoleid(srcRoleID);
			roRes.set_opporolename(srcRole->GetName());
		}
	}

	if (roArg.optype() == KKSG::MarriageOpType_Divorce)
	{
		UINT32 error = ApplyDivorce(role);

		return error;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageRelationMgr::BuildRelation(Marriage* src, Marriage* dest, UINT32 type)
{
	CRoleSummary* srcsummary = CRoleSummaryMgr::Instance()->GetRoleSummary(src->GetOwnerID());
	if (!srcsummary)
	{
		return KKSG::ERR_ROLE_NOTEXIST;
	}

	CRoleSummary* destsummary = CRoleSummaryMgr::Instance()->GetRoleSummary(dest->GetOwnerID());
	if (!destsummary)
	{
		return KKSG::ERR_ROLE_NOTEXIST;
	}

	int srcPro = srcsummary->GetProfession();
	auto table = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(srcPro);
	if (!table)
	{
		return KKSG::ERR_PROF_ERROR;
	}

	KKSG::MarriagePos srcPos = KKSG::MarriagePos_Null;
	KKSG::MarriagePos destPos = KKSG::MarriagePos_Null;
	if (table->Sex == 1)
	{
		srcPos = KKSG::MarriagePos_Husband;
		destPos = KKSG::MarriagePos_Wife;
	}
	else if (table->Sex == 2)
	{
		srcPos = KKSG::MarriagePos_Wife;
		destPos = KKSG::MarriagePos_Husband;
	}
	else
		return KKSG::ERR_PROF_ERROR;
	
	src->AddRelation(dest, type, srcPos);
	dest->AddRelation(src, type, destPos);

	DoTxLog(src->GetOwnerID(), dest->GetOwnerID(), TMarriageType2, type);
	DoTxLog(dest->GetOwnerID(), src->GetOwnerID(), TMarriageType2, type);
	EventMgr::Instance()->AddEvent(src->GetOwnerID(),DESIGNATION_COM_TYPE_MARRY,src->GetMarriagePos(),GlobalEvent::EVENT_OP_ADD,destsummary->GetName());
	EventMgr::Instance()->AddEvent(dest->GetOwnerID(),DESIGNATION_COM_TYPE_MARRY,dest->GetMarriagePos(),GlobalEvent::EVENT_OP_ADD,srcsummary->GetName());
	UINT32 ringID = 0;
	if (type == KKSG::WeddingType_Normal)
	{
		ringID = GetGlobalConfig().MarriageRing;
	}
	else if(type == KKSG::WeddingType_Luxury)
	{
		ringID = GetGlobalConfig().MarriageSplendidRing;
	}
	RpcM2G_MsGiveItem * rpc1 = RpcM2G_MsGiveItem::CreateRpc();
	rpc1->m_oArg.set_roleid(src->GetOwnerID());
	rpc1->m_oArg.set_reason(ItemFlow_Marriage);
	rpc1->m_oArg.set_subreason(ItemFlow_Marriage_Marry);
	KKSG::ItemBrief* brief = rpc1->m_oArg.add_items();
	brief->set_itemid(ringID);
	brief->set_itemcount(1);
	brief->set_isbind(true);
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc1);

	RpcM2G_MsGiveItem * rpc2 = RpcM2G_MsGiveItem::CreateRpc();
	rpc2->m_oArg.CopyFrom(rpc1->m_oArg);
	rpc2->m_oArg.set_roleid(dest->GetOwnerID());
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc2);

	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageRelationMgr::PreApplyRelation(CRole* role, UINT64& destRoleID)
{
	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!marriage)
	{
		return KKSG::ERR_FAILED;
	}
	if (!role->GetTeamID())
	{
		return KKSG::ERR_MARRIAGE_NEED_TEAM;
	}
	Team* pteam = TeamMgr::Instance()->FindTeam(role->GetTeamID());
	if (!pteam || pteam->GetExpeditionType() != TEAM_TYPE_MARRIAGE)
	{
		return KKSG::ERR_MARRIAGE_NEED_TEAM;
	}
	if (pteam->GetMemberCount() != 2 || pteam->GetMemberCountWithoutRobot() != 2)
	{
		return KKSG::ERR_MARRIAGE_NEED_TWO_NOTMARRY;
	}
	auto members = pteam->GetMember();
	for (auto iter = members.begin(); iter != members.end(); ++iter)
	{
		if ((*iter)->m_qwRoleID != role->GetID())
		{
			destRoleID = (*iter)->m_qwRoleID;
			break;
		}
	}
	CRole* destRole = CRoleManager::Instance()->GetByRoleID(destRoleID);
	if (!destRole)
	{
		return KKSG::ERR_MARRIAGE_NEED_TWO_NOTMARRY;
	}
	if (role->GetMapID() != HALL_MAP_ID || destRole->GetMapID() != HALL_MAP_ID)
	{
		return KKSG::ERR_TEAM_NOT_IN_HALL;
	}

	Friend* friendInfo = FriendMgr::Instance()->GetFriendInfo(role->GetID());
	if (!friendInfo)
	{
		return KKSG::ERR_FRIEND_NOTEXIST;
	}
	if (friendInfo->GetDegree(destRoleID) < GetGlobalConfig().MarriageFriendDegreeNeed)
	{
		return KKSG::ERR_FRIEND_DEGREE_NOTENOUGH;
	}

	Marriage* destMarriage = MarriageMgr::Instance()->GetMarriage(destRoleID);
	if (!destMarriage)
	{
		return KKSG::ERR_MARRIAGE_NEED_TWO_NOTMARRY;
	}

	MarriageCondition condition(marriage, destMarriage);

	return condition.IsValid();
}

UINT32 MarriageRelationMgr::ApplyRelation(CRole* role, UINT64 destRoleID, KKSG::WeddingType type)
{
	if (type != KKSG::WeddingType_Normal && type != KKSG::WeddingType_Luxury)
	{
		return KKSG::ERR_FAILED;
	}
	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	Marriage* marriage2 = MarriageMgr::Instance()->GetMarriage(destRoleID);

	marriage->GetMarriageMsg()->AddApplyMsg(destRoleID, type);

	CRole* destRole = CRoleManager::Instance()->GetByRoleID(destRoleID);
	if (destRole)
	{
		PtcM2C_NotifyMarriageApply ntf;
		ntf.m_Data.mutable_applyinfo()->set_applyroleid(role->GetID());
		ntf.m_Data.mutable_applyinfo()->set_applyname(role->GetName());
		ntf.m_Data.mutable_applyinfo()->set_type(type);
		destRole->Send(ntf);
		DoTxLog(role->GetID(), destRoleID, TMarriageType1, type);//·¢Æð½á»élog
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageRelationMgr::PreAgreeApplyRelation(CRole* role, UINT64 srcRoleID, KKSG::WeddingType& type)
{
	Marriage* srcMarriage = MarriageMgr::Instance()->GetMarriage(srcRoleID, false);
	Marriage* selfMarriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!srcMarriage || !selfMarriage)
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 teamSrcID = 0;
	UINT32 error = PreApplyRelation(role, teamSrcID);
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}
	if (teamSrcID != srcRoleID)
	{
		return KKSG::ERR_MARRIAGE_NEED_TWO_NOTMARRY;
	}

	//UINT32 type = KKSG::WeddingType_Normal;
	bool srcMsgExist = srcMarriage->GetMarriageMsg()->CheckApplyInfoExist(srcRoleID, role->GetID(), type);
	if (!srcMsgExist)
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageRelationMgr::AgreeApplyRelation(CRole* role, UINT64 srcRoleID)
{
	Marriage* srcMarriage = MarriageMgr::Instance()->GetMarriage(srcRoleID, false);
	Marriage* selfMarriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!srcMarriage || !selfMarriage)
	{
		return KKSG::ERR_FAILED;
	}

	KKSG::WeddingType type = KKSG::WeddingType_Normal;
	bool srcMsgExist = srcMarriage->GetMarriageMsg()->CheckApplyInfoExist(srcRoleID, role->GetID(), type);
	if (!srcMsgExist)
	{
		return KKSG::ERR_FAILED;
	}

	srcMarriage->GetMarriageMsg()->RemoveApplyInfo(srcRoleID, role->GetID());

	UINT32 error = BuildRelation(srcMarriage, selfMarriage, type);
	if (error == KKSG::ERR_SUCCESS)
	{
		CRole* srcRole = CRoleManager::Instance()->GetByRoleID(srcMarriage->GetOwnerID());
		if (srcRole)
		{
			PtcM2C_NotifyMarriageApply ntf;
			KKSG::MarriageApplyResponse* reponse = ntf.m_Data.mutable_response();
			reponse->set_isagree(true);
			reponse->set_roleid(role->GetID());
			reponse->set_rolename(role->GetName());
			srcRole->Send(ntf);
		}

		MarriageMgr::Instance()->QuickSave(srcMarriage);
		MarriageMgr::Instance()->QuickSave(selfMarriage);
	}

	return error;
}

UINT32 MarriageRelationMgr::RefuseApplyRelation(CRole* role, UINT64 srcRoleID)
{
	Marriage* srcMarriage = MarriageMgr::Instance()->GetMarriage(srcRoleID, false);
	Marriage* selfMarriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!srcMarriage || !selfMarriage)
	{
		return KKSG::ERR_FAILED;
	}
	KKSG::WeddingType type = KKSG::WeddingType_Normal;
	bool srcMsgExist = srcMarriage->GetMarriageMsg()->CheckApplyInfoExist(srcRoleID, role->GetID(), type);
	if (!srcMsgExist)
	{
		return KKSG::ERR_FAILED;
	}

	CRole* srcRole = CRoleManager::Instance()->GetByRoleID(srcRoleID);
	if (srcRole)
	{
		PtcM2C_NotifyMarriageApply ntf;
		KKSG::MarriageApplyResponse* reponse = ntf.m_Data.mutable_response();
		reponse->set_isagree(false);
		reponse->set_roleid(role->GetID());
		reponse->set_rolename(role->GetName());
		srcRole->Send(ntf);
	}

	srcMarriage->GetMarriageMsg()->RemoveApplyInfo(srcMarriage->GetOwnerID(), selfMarriage->GetOwnerID());

	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageRelationMgr::PreApplyDivorce(CRole* role, bool& toGs)
{
	if (!role)
	{
		return KKSG::ERR_FAILED;
	}

	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!marriage)
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 coupleID = marriage->GetCoupleID();
	if (!coupleID)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}
	Marriage* couple = MarriageMgr::Instance()->GetMarriage(coupleID);
	if (!couple)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	toGs = true;

	int offlineTime = marriage->GetCoupleOfflineTime();
	if (offlineTime <= FREE_DIVORCE_TIME)
	{
		toGs = true;
	}
	else
	{
		toGs = false;
	}

	if (!toGs)
	{
		return ApplyDivorce(role);
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageRelationMgr::ApplyDivorce(CRole* role)
{
	if (!role)
	{
		return KKSG::ERR_FAILED;
	}

	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!marriage)
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 coupleID = marriage->GetCoupleID();
	if (!coupleID)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}
	Marriage* couple = MarriageMgr::Instance()->GetMarriage(coupleID);
	if (!couple)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	UINT32 error = marriage->ApplyDivorce(role->GetID());
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	couple->ApplyDivorce(role->GetID());

	CRole* coupleRole = CRoleManager::Instance()->GetByRoleID(coupleID);
	if (coupleRole)
	{
		PtcM2C_NotifyMarriageDivorceApply ntf;
		ntf.m_Data.set_isapplycancel(false);
		ntf.m_Data.set_roleid(role->GetID());
		ntf.m_Data.set_name(role->GetName());
		ntf.m_Data.set_lefttime(couple->GetLeftDivorceTime());
		coupleRole->Send(ntf);
	}
	else
	{
		couple->SetNoticeDivorce();
	}

	BreakApplyPair info;
	info.m_srcID = role->GetID();
	info.m_destID = coupleID;
	//info.m_time = GameTime::GetTime() + MARRIAGEDIVORCETIME;
	MarriageMgr::Instance()->AddAutoBreak(info, GameTime::GetTime() + MARRIAGEDIVORCETIME);

	MarriageMgr::Instance()->QuickSave(marriage);
	MarriageMgr::Instance()->QuickSave(couple);

	DoTxLog(role->GetID(), coupleID, TMarriageType3, marriage->GetMarriageType());
	return KKSG::ERR_SUCCESS;
}

UINT32 MarriageRelationMgr::ApplyDivorceCancel(CRole* role)
{
	if (!role)
	{
		return KKSG::ERR_FAILED;
	}

	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!marriage)
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 coupleID = marriage->GetCoupleID();
	if (!coupleID)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}
	Marriage* couple = MarriageMgr::Instance()->GetMarriage(coupleID);
	if (!couple)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	UINT32 error = marriage->ApplyDivorceCancel(role->GetID());
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	couple->ApplyDivorceCancel(role->GetID());

	CRole* coupleRole = CRoleManager::Instance()->GetByRoleID(coupleID);
	if (coupleRole)
	{
		PtcM2C_NotifyMarriageDivorceApply ntf;
		ntf.m_Data.set_isapplycancel(true);
		ntf.m_Data.set_roleid(role->GetID());
		ntf.m_Data.set_name(role->GetName());
		ntf.m_Data.set_lefttime(couple->GetLeftDivorceTime());
		coupleRole->Send(ntf);
	}
	else
	{
		couple->SetNoticeDivorceCancel();
	}

	BreakApplyPair info;
	info.m_srcID = role->GetID();
	info.m_destID = coupleID;
	//info.m_time = GameTime::GetTime();
	MarriageMgr::Instance()->RemoveAutoBreak(info);

	MarriageMgr::Instance()->QuickSave(marriage);
	MarriageMgr::Instance()->QuickSave(couple);

	DoTxLog(role->GetID(), coupleID, TMarriageType4, marriage->GetMarriageType());
	return KKSG::ERR_SUCCESS;
}

void  MarriageRelationMgr::DoTxLog(UINT64 roleid1, UINT64 roleid2, UINT32 op, UINT32 type, UINT64 weddingid)
{
	TMarriageFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleid1;
	oLog.m_RoleID2 = roleid2;
	oLog.m_Op = op;
	oLog.m_WeddingType = type;
	oLog.m_WeddingID = weddingid;
	oLog.Do();
}
