#include "pch.h"
#include "qqviprecord.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "bagtransition.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "utility/loghelper.h"
#include "foreach.h"
#include "task/ptcg2c_takerandomtask.h"
#include "util/XCommon.h"
#include "utility/tlogger.h"
#include "bag.h"
#include "payv2Record.h"
#include "task/ptcg2c_updatetaskstatus.h"
#include "foreach.h"
#include "designationMgr.h"
#include "util/gametime.h"
#include "findback/findbackrecord.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "table/globalconfig.h"
#include "unit/systeminfo.h"

QQVipRecord::QQVipRecord(Role *pRole)
:m_role(pRole)
,m_isVip(false)
,m_isSVip(false)
,m_isYearVip(false)
,m_vipStart(0)
,m_vipEnd(0)
,m_SVipStart(0)
,m_SVipEnd(0)
,m_yearVipStart(0)
,m_yearVipEnd(0)
,m_VipNewBieRewarded(false)
,m_SVipNewBieRewarded(false)
,m_isXinYueVip(false)
{

}

QQVipRecord::~QQVipRecord()
{

}
	
bool QQVipRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	FromKKSG(poRoleAllInfo->qqvip());
	return true;
}

void QQVipRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_dirty.TestAndReset())
	{
		return;
	}
	ToKKSG(*poRoleAllInfo->mutable_qqvip());

	roChanged.insert(poRoleAllInfo->mutable_qqvip());
}

void QQVipRecord::FromKKSG(const KKSG::QQVipInfo& qqVip)
{
	m_isVip = qqVip.is_vip();
	m_isSVip = qqVip.is_svip();
	m_isYearVip = qqVip.is_year_vip();
	m_vipStart = qqVip.qq_vip_start();
	m_vipEnd = qqVip.qq_vip_end();
	m_SVipStart = qqVip.qq_svip_start();
	m_SVipEnd = qqVip.qq_svip_end();
	m_yearVipStart = qqVip.qq_year_vip_start();
	m_yearVipEnd = qqVip.qq_year_vip_end();
	m_VipNewBieRewarded = qqVip.vip_newbie_rewarded();
	m_SVipNewBieRewarded = qqVip.svip_newbie_rewarded();
	m_isXinYueVip = qqVip.is_xinyue_vip();
}

void QQVipRecord::ToKKSG(KKSG::QQVipInfo& qqVip)
{
	qqVip.set_is_vip(m_isVip);
	qqVip.set_is_svip(m_isSVip);
	qqVip.set_is_year_vip(m_isYearVip);
	qqVip.set_qq_vip_start(m_vipStart);
	qqVip.set_qq_vip_end(m_vipEnd);
	qqVip.set_qq_svip_start(m_SVipStart);
	qqVip.set_qq_svip_end(m_SVipEnd);
	qqVip.set_qq_year_vip_start(m_yearVipStart);
	qqVip.set_qq_year_vip_end(m_yearVipEnd);
	qqVip.set_vip_newbie_rewarded(m_VipNewBieRewarded);
	qqVip.set_svip_newbie_rewarded(m_SVipNewBieRewarded);
	qqVip.set_is_xinyue_vip(m_isXinYueVip);
}

bool QQVipRecord::IsVip()
{ 
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_QQVIP))
	{
		return false;
	}

	if (!m_isVip)
	{
		return false;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now >= m_vipEnd)
	{
		return false;
	}
	return true;
}

bool QQVipRecord::IsSVip()
{
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_QQVIP))
	{
		return false;
	}

	if (!m_isSVip)
	{
		return false;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now >= m_SVipEnd)
	{
		return false;
	}
	return true;
}

bool QQVipRecord::IsYearVip()
{
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_QQVIP))
	{
		return false;
	}

	if (!m_isYearVip)
	{
		return false;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now >= m_yearVipEnd)
	{
		return false;
	}
	return true;
}

bool QQVipRecord::IsXinYueVip()
{
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_QQVIP))
	{
		return false;
	}
	return m_isXinYueVip;
}

void QQVipRecord::SetVipTime(UINT32 start, UINT32 end)
{
	m_vipStart = start;
	m_vipEnd = end;
	m_dirty.Set();
}

void QQVipRecord::SetSVipTime(UINT32 start, UINT32 end)
{
	m_SVipStart = start;
	m_SVipEnd = end;
	m_dirty.Set();
}

void QQVipRecord::SetYearVipTime(UINT32 start, UINT32 end)
{
	m_yearVipStart = start;
	m_yearVipEnd = end;
	m_dirty.Set();
}

void QQVipRecord::CheckNewBieGift()
{
	return;

	// 会员礼包
	if (IsVip() && !m_VipNewBieRewarded)
	{
		m_VipNewBieRewarded = true;
		m_dirty.Set();

		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_QQVipGift);
		if (conf)
		{
			MailSender sender;
			sender.SetContent(Mail_System, conf->m_title,  conf->m_content);
			std::vector<ItemDesc> items;
			for (size_t i = 0; i < GetGlobalConfig().QQVipGift.size(); ++i)
			{
				Sequence<UINT32, 2>& itemConf = GetGlobalConfig().QQVipGift[i];
				ItemDesc item(itemConf[0], itemConf[1]);
				items.push_back(item);
			}
			sender.SetReason(ItemFlow_QQVip);
			sender.SetItems(items);
			sender.Send(m_role->GetID());
		}
	}

	// 超会礼包
	if (IsSVip() && !m_SVipNewBieRewarded)
	{
		m_SVipNewBieRewarded = true;
		m_dirty.Set();

		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_QQSVipGift);
		if (conf)
		{
			MailSender sender;
			sender.SetContent(Mail_System, conf->m_title,  conf->m_content);
			std::vector<ItemDesc> items;
			for (size_t i = 0; i < GetGlobalConfig().QQSVipGift.size(); ++i)
			{
				Sequence<UINT32, 2>& itemConf = GetGlobalConfig().QQSVipGift[i];
				ItemDesc item(itemConf[0], itemConf[1]);
				items.push_back(item);
			}
			sender.SetReason(ItemFlow_QQVip);
			sender.SetItems(items);
			sender.Send(m_role->GetID());
		}
	}
}

void QQVipRecord::FillClientInfo(KKSG::QQVipInfoClient& info)
{
	info.set_is_vip(m_isVip);
	info.set_is_svip(m_isSVip);
	info.set_qq_vip_end(m_vipEnd);
	info.set_qq_svip_end(m_SVipEnd);

	bool bigger = false;
	UINT32 now = TimeUtil::GetTime();
	if (m_vipEnd < now)
	{
		bigger = false;
	}
	// 腾讯示例代码
	else if (m_vipEnd - now > (86400 * 32))
	{
		bigger = true;
	}
	info.set_is_bigger_one_month(bigger);
}
