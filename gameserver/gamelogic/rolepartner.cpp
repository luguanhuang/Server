#include "pch.h"
#include "rolepartner.h"
#include "util/gametime.h"
#include "tablemgr/partnertablemgr.h"
#include "table/globalconfig.h"
#include "unit/role.h"
#include "define/systemiddef.h"
#include "gamelogic/bag.h"
#include "table/globalconfig.h"
#include "gamelogic/designationMgr.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "define/tlogenum.h"

RolePartner::RolePartner(Role *pRole)
:m_role(pRole)
,m_partnerID(0)
,m_lastLeavePartnerTime(0)
,m_takedChest(0)
,m_openShopTime(0)
,m_applyLeaveTime(0)
,m_lastUpdateTime(0)
,m_hasChest(false)
,m_compensateTime(0)
{

}

RolePartner::~RolePartner()
{

}

bool RolePartner::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::RolePartnerData& data = poRoleAllInfo->partner();
	m_partnerID = data.partnerid();
	m_lastLeavePartnerTime = data.last_leave_partner_time();
	m_takedChest = data.taked_chest();
	m_openShopTime = data.open_shop_time();
	m_applyLeaveTime = data.apply_leave_time();
	m_hasChest = data.chest_redpoint();
	m_lastUpdateTime = data.last_update_time();
	m_compensateTime = data.compenstatetime();
	OnDayPass();
	return true;
}

void RolePartner::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_dirty.TestAndReset())
	{
		return;
	}
	KKSG::RolePartnerData& data = *poRoleAllInfo->mutable_partner();
	data.set_partnerid(m_partnerID);
	data.set_last_leave_partner_time(m_lastLeavePartnerTime);
	data.set_taked_chest(m_takedChest);
	data.set_open_shop_time(m_openShopTime);
	data.set_apply_leave_time(m_applyLeaveTime);
	data.set_chest_redpoint(m_hasChest);
	data.set_last_update_time(m_lastUpdateTime);
	data.set_compenstatetime(m_compensateTime);

	roChanged.insert(&data);
}

void RolePartner::OnDayPass()
{
	UINT32 now = GameTime::GetTime();
	if (XCommon::IsDaySame(m_lastUpdateTime, now))
	{
		return;
	}

	m_lastUpdateTime = now;
	m_takedChest = 0;
	m_hasChest = false;

	m_dirty.Set();
}

void RolePartner::SetPartnerID(UINT64 partnerID)
{
	if (m_partnerID == partnerID)
	{
		return;
	}
	m_partnerID = partnerID;
	if (m_partnerID)
	{
		DesignationMgr::Instance()->OnGay(m_role);
	}
	m_dirty.Set();
}

void RolePartner::SetLastLeavePartnerTime(UINT32 lastTime)
{
	if (m_lastLeavePartnerTime == lastTime)
	{
		return;
	}
	m_lastLeavePartnerTime = lastTime;
	m_dirty.Set();
}

bool RolePartner::IsMakePartnerCDOk()
{
	if (m_lastLeavePartnerTime == 0)
	{
		return true;
	}
	return !GameTime::IsInSameDay(m_lastLeavePartnerTime, GameTime::GetTime(), true);
}

bool RolePartner::IsChestTaked(UINT32 index)
{
	if (index >= 32)
	{
		LogWarn("chest index[%u] invalid", index);
		return false;
	}
	return (m_takedChest & (1 << index)) > 0 ? true : false;
}

void RolePartner::SetChestTaked(UINT32 index)
{
	if (index >= 32)
	{
		LogWarn("chest index[%u] invalid", index);
		return;
	}
	m_takedChest |= (1 << index);
	m_dirty.Set();
}

bool RolePartner::HasCheskCanTake(UINT32 level, UINT32 liveness)
{
	if (m_partnerID == 0)
	{
		return false;
	}
	std::vector<UINT32> all;
	PartnerTableMgr::Instance()->GetCanTakeChestIndex(level, liveness, all);
	for (auto it = all.begin(); it != all.end(); ++it)
	{
		if (!IsChestTaked(*it))
		{
			return true;
		}
	}
	return false;
}

void RolePartner::SetApplyLeaveTime(UINT32 t)
{
	if (m_applyLeaveTime == t)
	{
		return;
	}
	m_applyLeaveTime = t;
	m_dirty.Set();
}

void RolePartner::UpdateLeaveState()
{
	// 没有战友，而且之前申请过离开战友，说明下线过程中保护时间到期
	if (m_partnerID == 0 && m_applyLeaveTime != 0)
	{
		UINT32 leaveTime = m_applyLeaveTime + GetGlobalConfig().PartnerLeaveTime;
		SetLastLeavePartnerTime(leaveTime);
	}
}

void RolePartner::CheckChest(UINT32 partLevel, UINT32 liveness)
{
	bool old = m_hasChest;
	m_hasChest = HasCheskCanTake(partLevel, liveness);

	if (old == false && m_hasChest == true)
	{
		// 推送红点
		m_role->HintNotify(SYS_PARTNER_LIVENESS, false);
		m_dirty.Set();
	}
	if (old == true && m_hasChest == false)
	{
		// 推送红点消失
		m_role->HintNotify(SYS_PARTNER_LIVENESS, true);
		m_dirty.Set();
	}
}

KKSG::ErrorCode RolePartner::CheckCanMakePartner()
{
	// 系统开放
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_PARTNER))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	// 龙币不足
	Bag* bag = m_role->Get<Bag>();
	if (bag->CountVirtualItem(DRAGON_COIN) < GetGlobalConfig().PartnerNeedDragonCoin)
	{
		return KKSG::ERR_PARTNER_NOT_ENOUGH_DRAGON;
	}
	// 退出战友CD判断
	if (!IsMakePartnerCDOk())
	{
		return KKSG::ERR_PARTNER_CD_NOT_OK;
	}
	return KKSG::ERR_SUCCESS;
}

void RolePartner::OnLogin()
{
	if (m_partnerID != 0 && m_compensateTime == 0)
	{
		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_PartnerCompensate);
		if (conf)
		{
			MailSender sender;
			sender.AddConfigItem(conf);
			sender.SetContent(Mail_System, conf->m_title, conf->m_content);
			sender.SetReason(ItemFlow_PartnerCompenstate);
			sender.Send(m_role->GetID());

			m_compensateTime = GameTime::GetTime();
			m_dirty.Set();
		}
	}
}