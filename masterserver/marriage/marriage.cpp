#include "pch.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"
#include "table/globalconfig.h"
#include "marriagemgr.h"
#include "marriage.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "util/gametime.h"
#include "marriagemsg.h"
#include "marriage/ptcm2c_notifymarriagedivorceapply.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "foreach.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "tablemgr/marriagetablemgr.h"
#include "weddingmgr.h"
#include "mail/maildata.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "spactivity/spactivitymgr.h"
#include "define/spactivityiddef.h"


MarriageRelation::MarriageRelation()
{
	Reset();
}

void MarriageRelation::Reset()
{
	m_takeWeddingCar = false;
	m_holdedWedding = false;
	m_applyDivorceInfo.Reset();
	m_marriageTime = 0;
	m_divorceTime = 0;
	m_roleID = 0;
	m_weddingType = KKSG::WeddingType_Normal;
	m_pos = KKSG::MarriagePos_Null;
	m_status = KKSG::MarriageStatus_Null;
	m_liveness = 0;
	m_livenessRecord.clear();
	m_chestTaked = 0;
}

void MarriageRelation::ConvertFromDb(const KKSG::OneMarriageRelation2DB& relation)
{
	m_marriageTime = relation.marriagetime();
	m_divorceTime = relation.divorcetime();
	m_roleID = relation.roleid();
	m_weddingType = relation.type();
	m_pos = relation.pos();
	m_holdedWedding = relation.weddingholded();
	m_takeWeddingCar = relation.takeweddingcar();
	m_status = relation.status();
	m_applyDivorceInfo.m_isDivorceApplier = relation.isdivorceapplier();
	m_applyDivorceInfo.m_needNotifyDivorce = relation.notifydivorce();
	m_applyDivorceInfo.m_needNotifyDivorceCancel = relation.notifydivorcecancel();
	m_liveness = relation.liveness();
	m_chestTaked = relation.chesttaked();
	m_livenessRecord.clear();	
	for (int i = 0; i < relation.livenessrecord_size(); ++i)
	{
		PartnerLivenessRecord record;
		record.FromKKSG(relation.livenessrecord(i));
		m_livenessRecord.push_back(record);
	}
}

void MarriageRelation::Convert2Db(KKSG::OneMarriageRelation2DB* relation)
{
	relation->set_marriagetime(m_marriageTime);
	relation->set_divorcetime(m_divorceTime);
	relation->set_roleid(m_roleID);
	relation->set_type(m_weddingType);
	relation->set_pos(m_pos);
	relation->set_status(m_status);
	relation->set_weddingholded(m_holdedWedding);
	relation->set_takeweddingcar(m_takeWeddingCar);
	relation->set_isdivorceapplier(m_applyDivorceInfo.m_isDivorceApplier);
	relation->set_notifydivorce(m_applyDivorceInfo.m_needNotifyDivorce);
	relation->set_notifydivorcecancel(m_applyDivorceInfo.m_needNotifyDivorceCancel);
	relation->set_liveness(m_liveness);
	relation->set_chesttaked(m_chestTaked);
	for (auto it = m_livenessRecord.begin(); it != m_livenessRecord.end(); ++it)
	{
		it->ToKKSG(*relation->add_livenessrecord());
	}
}

UINT32 MarriageRelation::GetDivorceLeftTime()
{
	if (!m_divorceTime)
	{
		return -1;
	}

	int now = GameTime::GetTime();
	if (now >= m_divorceTime)
	{
		return 0;
	}

	return m_divorceTime - now;
}

Marriage::Marriage(UINT64 roleID)
{
	m_roleID = roleID;
	m_updateTime = 0;
	m_selfPos = KKSG::MarriagePos_Null;

	m_pMarriageMsg = new MarriageMsg(this);

	m_getActivityEndTime = 0;
}

Marriage::~Marriage()
{
	delete m_pMarriageMsg;
}


void Marriage::Load(const KKSG::MarriageAllData2DB& data)
{
	m_updateTime = data.updatetime();
	m_selfPos = data.selfpos();
	m_getActivityEndTime = data.getactivityrewardtime();
	m_curRelation.ConvertFromDb(data.currelation());
	for (int i = 0; i < data.hisrelation_size(); ++i)
	{
		MarriageRelation oneRelation;
		oneRelation.ConvertFromDb(data.hisrelation(i));
		m_hisRelation.push_back(oneRelation);
	}

	if (IsInMarriagePos() && m_curRelation.m_status == KKSG::MarriageStatus_DivorceApply)
	{
		if (m_curRelation.m_applyDivorceInfo.m_isDivorceApplier)
		{
			BreakApplyPair info;
			info.m_srcID = m_curRelation.m_roleID;
			info.m_destID = GetOwnerID();
			//info.m_time = m_curRelation.m_divorceTime;
			MarriageMgr::Instance()->AddAutoBreak(info, m_curRelation.m_divorceTime);
		}
	}

	if (!GameTime::IsInSameDay(GameTime::GetTime(), m_updateTime, true))
	{
		OnDayPass();
	}
}

void Marriage::Save(KKSG::MarriageAllData2DB* data)
{
	data->set_roleid(m_roleID);
	data->set_updatetime(m_updateTime);
	data->set_selfpos(m_selfPos);
	data->set_getactivityrewardtime(m_getActivityEndTime);
	m_curRelation.Convert2Db(data->mutable_currelation());

	foreach(i in m_hisRelation)
	{
		i->Convert2Db(data->add_hisrelation());
	}
}

void Marriage::SetChanged()
{
	MarriageMgr::Instance()->AddChanged(GetOwnerID());
}

bool Marriage::IsOpen()
{
	auto findSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleID);
	if (!findSummary)
	{
		return false;
	}

	auto data = OpenSystemMgr::Instance()->GetSystemRowData(SYS_MARRIAGE);
	if (!data)
	{
		return false;
	}

	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_MARRIAGE))
	{
		return false;
	}

	//需要修改,system的配置项,按照等级来
	if (findSummary->GetLevel() >= data->PlayerLevel && data->IsOpen)
	{
		return true;
	}

	return false;
}

void Marriage::OnRoleLogin(CRole* role)
{
	if (!IsOpen())
	{
		return;
	}

	if (!IsInMarriagePos())
	{
		return;
	}

	_NotifyDivorceOrCancel(role);
	NotifyLivenessHint();
}

void Marriage::OnDayPass()
{
	m_updateTime = GameTime::GetTime();
	SetChanged();
	if (GameTime::GetWeekDay() == 1)
	{
		OnWeekPass();
	}
}

void Marriage::OnWeekPass()
{
	if (IsInMarriagePos())
	{
		m_curRelation.m_liveness = 0;
		m_curRelation.m_chestTaked = 0;
		m_curRelation.m_livenessRecord.clear();
		SetChanged();
	}
}

bool Marriage::AddRelation(Marriage* destMarriage, UINT32 type, KKSG::MarriagePos pos)
{
	if (type != KKSG::WeddingType_Normal && type != KKSG::WeddingType_Luxury)
	{
		return false;
	}
	if (IsInMarriagePos())
	{
		return false;
	}

	m_selfPos = pos;

	m_curRelation.m_roleID = destMarriage->GetOwnerID();
	m_curRelation.m_weddingType = (KKSG::WeddingType)type;
	m_curRelation.m_status = KKSG::MarriageStatus_Marriaged;
	m_curRelation.m_marriageTime = GameTime::GetTime();
	if (pos == KKSG::MarriagePos_Husband)
	{
		m_curRelation.m_pos = KKSG::MarriagePos_Wife;
	}
	else
	{
		m_curRelation.m_pos = KKSG::MarriagePos_Husband;
	}

	_OnAddRelation();

	LogInfo("%llu marry %llu, type:%u, pos:%u", GetOwnerID(), destMarriage->GetOwnerID(), type, pos);

	return true;
}

UINT32 Marriage::FillMarrigeRelation2Client(KKSG::GetMarriageRelationRes& roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	if (!IsInMarriagePos())
	{
		roRes.set_marriagestatus(KKSG::MarriageStatus_Null);
		return KKSG::ERR_SUCCESS;
	}

	CRoleSummary* selfSumm = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleID);
	CRoleSummary* otherSumm = CRoleSummaryMgr::Instance()->GetRoleSummary(m_curRelation.m_roleID);
	if (!selfSumm || !otherSumm)
	{
		return KKSG::ERR_SUCCESS;
	}

	roRes.set_type(m_curRelation.m_weddingType);
	selfSumm->FillRoleOutLookBrief(*roRes.add_infos());
	otherSumm->FillRoleOutLookBrief(*roRes.add_infos());
	roRes.set_leftdivorcetime(m_curRelation.GetDivorceLeftTime());
	if (m_curRelation.m_status == KKSG::MarriageStatus_DivorceApply)
	{
		if (m_curRelation.m_applyDivorceInfo.m_isDivorceApplier)
		{
			roRes.set_applydivorceid(m_curRelation.m_roleID);
		}
		else
		{
			roRes.set_applydivorceid(GetOwnerID());
		}
	}

	roRes.set_marriagestatus(_GetMarriageStatus2Client());
	roRes.set_coupleofflinetime(GetCoupleOfflineTime());

	return KKSG::ERR_SUCCESS;
}

UINT32 Marriage::ApplyDivorce(UINT64 applyID)
{
	if (!IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE; 
	}

	if (m_curRelation.m_status == KKSG::MarriageStatus_DivorceApply)
	{
		return KKSG::ERR_MARRIAGE_ALREADY_APPLYDIVORCE;
	}
	
	m_curRelation.m_status = KKSG::MarriageStatus_DivorceApply;
	m_curRelation.m_divorceTime = GameTime::GetTime() + MARRIAGEDIVORCETIME;
	if (applyID == GetOwnerID())
	{
		m_curRelation.m_applyDivorceInfo.m_isDivorceApplier = false;

		LogInfo("%llu apply divorce %llu", GetOwnerID(), m_curRelation.m_roleID);
	}
	else
	{
		m_curRelation.m_applyDivorceInfo.m_isDivorceApplier = true;

		LogInfo("%llu be apply divorce %llu", GetOwnerID(), m_curRelation.m_roleID);
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 Marriage::ApplyDivorceCancel(UINT64 applyID)
{
	if (!IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE; 
	}
	if (m_curRelation.m_status != KKSG::MarriageStatus_DivorceApply)
	{
		return KKSG::ERR_MARRIAGE_NOT_DIVORCESTATUS;
	}

	if (applyID == GetOwnerID() && m_curRelation.m_applyDivorceInfo.m_isDivorceApplier)
	{
		return KKSG::ERR_MARRIAGE_ONLY_APPLIER_CANCLEDIVORCE;
	}

	m_curRelation.m_status = KKSG::MarriageStatus_Marriaged;
	m_curRelation.m_divorceTime = 0;
	if (applyID == GetOwnerID())
	{
		m_curRelation.m_applyDivorceInfo.Reset();

		LogInfo("%llu cancel divorce %llu", GetOwnerID(), m_curRelation.m_roleID);
	}
	else
	{
		LogInfo("%llu be cancel divorce %llu", GetOwnerID(), m_curRelation.m_roleID);
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 Marriage::GetLeftDivorceTime()
{
	return m_curRelation.GetDivorceLeftTime();
}

UINT32 Marriage::Divorce()
{
	m_selfPos = KKSG::MarriagePos_Null;
	m_curRelation.m_status = KKSG::MarriageStatus_Divorced;
	m_hisRelation.push_back(m_curRelation);

	LogInfo("%llu divorce %llu", GetOwnerID(), m_curRelation.m_roleID);

	m_curRelation.Reset();

	return KKSG::ERR_SUCCESS;
}

void Marriage::SetHoldedWedding(bool holded)
{
	if (!IsInMarriagePos())
	{
		return ;
	}
	m_curRelation.m_holdedWedding = holded;
	MarriageMgr::Instance()->QuickSave(this);

	LogInfo("%llu, %llu wedding hold status %u", GetOwnerID(), m_curRelation.m_roleID, holded);
}

KKSG::WeddingType Marriage::GetWeddingType()
{
	return m_curRelation.m_weddingType;
}

UINT32 Marriage::CheckHoldWedding(KKSG::WeddingType& type)
{
	if (!IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	if (m_curRelation.m_status == KKSG::MarriageStatus_DivorceApply)
	{
		return KKSG::ERR_MARRIAGE_ALREADY_APPLYDIVORCE;
	}
	if (m_curRelation.m_holdedWedding)
	{
		return KKSG::ERR_WEDDING_ALREADY_HOLDED;
	}

	type = m_curRelation.m_weddingType;
	return KKSG::ERR_SUCCESS;
}

UINT32 Marriage::CheckTakeWeddingCar()
{
	if (!IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	if (m_curRelation.m_status == KKSG::MarriageStatus_DivorceApply)
	{
		return KKSG::ERR_MARRIAGE_ALREADY_APPLYDIVORCE;
	}

	if (m_curRelation.m_takeWeddingCar)
	{
		return KKSG::ERR_WEDDING_ALREADY_TAKECAR;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 Marriage::SetTakeWeddingCar(bool taked)
{
	if (!IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	m_curRelation.m_takeWeddingCar = taked;

	MarriageMgr::Instance()->QuickSave(this);

	LogInfo("%llu, %llu take wedding car[%u]", GetOwnerID(), m_curRelation.m_roleID, taked);

	return KKSG::ERR_SUCCESS;
}

void Marriage::SetNoticeDivorce(bool set)
{
	m_curRelation.m_applyDivorceInfo.m_needNotifyDivorce = set;
	SetChanged();
}

void Marriage::SetNoticeDivorceCancel(bool set)
{
	m_curRelation.m_applyDivorceInfo.m_needNotifyDivorceCancel = set;
	SetChanged();
}

UINT64 Marriage::GetCoupleID()
{
	if (!IsInMarriagePos())
	{
		return 0;
	}

	return m_curRelation.m_roleID;
}

bool Marriage::IsInMarriagePos()
{
	if (m_selfPos == KKSG::MarriagePos_Husband || m_selfPos == KKSG::MarriagePos_Wife)
	{
		return true;
	}

	return false;
}

bool Marriage::HasHistory()
{
	return !m_hisRelation.empty();
}

UINT32 Marriage::GetMarriageType()
{
	if (!IsInMarriagePos())
	{
		return 0;
	}

	return m_curRelation.m_weddingType;
}

void Marriage::_NotifyDivorceOrCancel(CRole* role)
{
	if (!m_curRelation.m_applyDivorceInfo.m_isDivorceApplier)
	{
		return;
	}

	if (!role)
	{
		return;
	}

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_curRelation.m_roleID);
	if (!summary)
	{
		return;
	}

	if (m_curRelation.m_status == KKSG::MarriageStatus_DivorceApply && m_curRelation.m_applyDivorceInfo.m_needNotifyDivorce)
	{
		PtcM2C_NotifyMarriageDivorceApply ntf;
		ntf.m_Data.set_isapplycancel(false);
		ntf.m_Data.set_roleid(summary->GetID());
		ntf.m_Data.set_name(summary->GetName());
		ntf.m_Data.set_lefttime(GetLeftDivorceTime());
		role->Send(ntf);

		SetNoticeDivorce(false);
	}

	if (m_curRelation.m_status == KKSG::MarriageStatus_Marriaged && m_curRelation.m_applyDivorceInfo.m_needNotifyDivorceCancel)
	{
		PtcM2C_NotifyMarriageDivorceApply ntf;
		ntf.m_Data.set_isapplycancel(true);
		ntf.m_Data.set_roleid(summary->GetID());
		ntf.m_Data.set_name(summary->GetName());
		ntf.m_Data.set_lefttime(GetLeftDivorceTime());
		role->Send(ntf);

		SetNoticeDivorceCancel(false);
	}
}

KKSG::MarriageStatus Marriage::_GetMarriageStatus2Client()
{
	if (!IsInMarriagePos())
	{
		return KKSG::MarriageStatus_Null;
	}
	if (m_curRelation.m_status == KKSG::MarriageStatus_DivorceApply)
	{
		return KKSG::MarriageStatus_DivorceApply;
	}

	KKSG::MarriageStatus status = KKSG::MarriageStatus_Null;
	if (m_curRelation.m_takeWeddingCar)
	{
		if (WeddingMgr::Instance()->IsRoleWeddingHoldding(m_roleID))
		{
			status = KKSG::MarriageStatus_WeddingHoldingAndCar;
		}
		else if (!m_curRelation.m_holdedWedding)
		{
			status = KKSG::MarriageStatus_WeddingCarNoWedding;
		}
		else
		{
			status = KKSG::MarriageStatus_WeddingHoldedAndCar;
		}
	}
	else
	{
		if (WeddingMgr::Instance()->IsRoleWeddingHoldding(m_roleID))
		{
			status = KKSG::MarriageStatus_WeddingHoldingNoCar;
		}
		else if (!m_curRelation.m_holdedWedding)
		{
			status = KKSG::MarriageStatus_Marriaged;
		}
		else
		{
			status = KKSG::MarriageStatus_WeddingHoldedNoCar;
		}
	}

	return status;
}

void Marriage::_OnAddRelation()
{
	UINT32 now = GameTime::GetTime();
	auto data = SpActivityMgr::Instance()->GetActivityData(SpActivity_Marriage);
	if (data && now >= data->m_startTime && now < data->m_endTime)
	{
		CheckGetMarriageActivityReward(data->m_endTime);
	}
}

bool Marriage::IsChestTaked(UINT32 index)
{
	if (index >= 32)
	{
		LogWarn("chest index[%u] invalid", index);
		return false;
	}
	return (m_curRelation.m_chestTaked & (1 << index)) > 0 ? true : false;
}

void Marriage::SetChestTaked(UINT32 index)
{
	if (index >= 32)
	{
		LogWarn("chest index[%u] invalid", index);
		return;
	}
	m_curRelation.m_chestTaked |= (1 << index);
	SetChanged();
	NotifyLivenessHint();
}

UINT32 Marriage::GetCoupleOfflineTime()
{
	if (!IsInMarriagePos())
	{
		return -1;
	}

	CRole* couple = CRoleManager::Instance()->GetByRoleID(m_curRelation.m_roleID);
	if (couple)
	{
		return 0;
	}
	int now = GameTime::GetTime();
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_curRelation.m_roleID);
	if (summary && now > summary->LogoutTime())
	{
		return now - summary->LogoutTime();
	}

	return 0;
}

void Marriage::AddLivenessRecord(UINT64 roleId, UINT32 actId, UINT32 value)
{
	PartnerLivenessRecord item;
	item.roleId = roleId;
	item.actId = actId;
	item.value = value;
	item.time = GameTime::GetTime();
	
	UINT32 oldValue = m_curRelation.m_liveness;
	m_curRelation.m_liveness += value;
	m_curRelation.m_livenessRecord.push_front(item);
	if (m_curRelation.m_livenessRecord.size() > GetGlobalConfig().PartnerLivenessRecordNum)
	{
		m_curRelation.m_livenessRecord.pop_back();
	}

	if (MarriageTableMgr::Instance()->HasNewChest(oldValue, m_curRelation.m_liveness))
	{
		NotifyLivenessHint();
	}
	SetChanged();
}

void Marriage::FillLivenessRecord(KKSG::GetMarriageLivenessRes& res)
{
	UINT32 now = GameTime::GetTime();
	for (auto it = m_curRelation.m_livenessRecord.begin(); it != m_curRelation.m_livenessRecord.end(); ++it)
	{
		PartnerLivenessRecord& item = *it;
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(item.roleId);
		if (su == NULL)
		{
			continue;
		}
		KKSG::PartnerLivenessItem* record = res.add_record();
		record->set_roleid(item.roleId);
		record->set_name(su->GetName());
		record->set_actid(item.actId);
		record->set_value(item.value);
		UINT32 passTime = now > item.time ? (now - item.time) : 0;
		record->set_time(passTime);
	}
}

void Marriage::NotifyLivenessHint()
{
	bool hasChest = false;
	auto& tables = MarriageTableMgr::Instance()->GetLivenessTable();
	for (auto it = tables.Table.begin(); it != tables.Table.end(); ++it)
	{
		auto pData = *it;
		if (pData->liveness <= m_curRelation.m_liveness && !IsChestTaked(pData->index))
		{
			hasChest = true;
			break;
		}
	}

	PtcM2C_HintNotifyMS ntf;
	ntf.m_Data.add_systemid(SYS_MARRIAGE);
	ntf.m_Data.set_isremove(!hasChest);
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_roleID);
	if (pRole)
	{
		pRole->Send(ntf);
	}
}

void Marriage::CheckGetMarriageActivityReward(UINT32 endTime)
{
	if (!IsInMarriagePos())
	{
		return;
	}
	if (!endTime)
	{
		return;
	}

	if (m_getActivityEndTime == endTime)
	{
		return;
	}

	m_getActivityEndTime = endTime;

	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_MarriageActivity);
	if(pMailConf)
	{
		std::vector<ItemDesc> rewards;
		for (auto iter = pMailConf->m_rewards.begin(); iter != pMailConf->m_rewards.end(); ++iter)
		{
			ItemDesc temp(iter->seq[0], iter->seq[1]);
			rewards.push_back(temp);
		}
		MailData maildata(pMailConf->m_title, pMailConf->m_content, rewards, MAIL_EXPIRE_TIME);
		CMailMgr::Instance()->SendMail(GetOwnerID(), maildata);
	}

	SetChanged();

	LogInfo("%llu, get marriage activity reward, end time %u", GetOwnerID(), endTime);
}
