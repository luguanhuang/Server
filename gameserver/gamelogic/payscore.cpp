#include "pch.h"
#include "payv2Record.h"
#include "globalconfig.h"
#include "tablemgr/prerogativemgr.h"
#include "util/gametime.h"
#include "LogInit.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "pay/ptcg2c_payscorentf.h"
#include "unit/roleoutlook.h"
#include "rolesummary/rolesummarybase.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "mail/mailsender.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/payconsumemgr.h"

void CPayV2Record::AddScore(INT32 amt)
{
	if (amt <= 0)
	{
		return;
	}
	AddRebate(amt);
}

void CPayV2Record::NotifyPreClient()
{
	RoleOutLook outlook(m_pRole);
	outlook.Fill(SUMMARY_PAYSCOREPRE, m_pRole);
	outlook.BroadCast();
}

void CPayV2Record::GetPayScorePre(KKSG::OutLookConsumePre* pScore)
{
	for(UINT32 i = 0; i < m_vSetId.size(); i ++)
	{
		pScore->add_setid(m_vSetId[i]);
	}
}

void CPayV2Record::GetPayScorePre(KKSG::PayConsume* pScore)
{
	for (auto it = m_vSetId.begin(); it != m_vSetId.end(); it ++)
	{
		pScore->add_setid(*it);
	}

	for (auto it = m_vActId.begin(); it != m_vActId.end(); it ++)
	{
		pScore->add_activateid(*it);
	}
}

void CPayV2Record::ActivatePre(UINT32 id)
{
	m_vActId.push_back(id);
	NotifyPreClient();
	Change();
}

bool CPayV2Record::IsActivatePre(UINT32 id)
{
	auto it = std::find_if(m_vActId.begin(), m_vActId.end(), EqualPreID(id));
	return it==m_vActId.end()?false:true;
}
void CPayV2Record::ClearSetPre()
{
	m_vSetId.clear();
	m_pRole->OnChangePre();
	Change();
}
bool CPayV2Record::SetPreShow(const KKSG::SetPreShowArg& roArg)
{
	std::map<UINT32, UINT32> vShowId;
	for (INT32 i = 0; i < roArg.showid_size(); i ++)
	{
		auto ite = CPreprogativeConfig::Instance()->GetPrerogativeConf(roArg.showid(i));
		if (ite == NULL)
		{
			LogError("roleid=%llu, not find id=%u in config", m_pRole->GetID(), roArg.showid(i));
			return false;
		}
		vShowId[roArg.showid(i)] = ite->Type;
		if (ite->Normal == 2)//初始化就有的，不需要激活也可以设置;
		{
			continue;
		}
		if (!IsActivatePre(roArg.showid(i)))
		{
			LogWarn("roleid=%llu,not activate id=%u", m_pRole->GetID(), roArg.showid(i));
			return false;
		}
	}
	for (auto i = vShowId.begin(); i != vShowId.end(); i ++)
	{
		bool isnew = true;
		for (UINT32 j = 0; j < m_vSetId.size(); j ++)
		{
			auto ite = CPreprogativeConfig::Instance()->GetPrerogativeConf(m_vSetId[j]);
			if (ite && ite->Type == i->second)
			{
				m_vSetId[j] = i->first;
				isnew = false;
				break;
			}
		}
		if (isnew)
		{
			m_vSetId.push_back(i->first);
		}
	}
	m_pRole->OnChangePre();
	Change();
	return true;
}

void CPayV2Record::CheckRebate()
{
	time_t nowtime = GameTime::GetTime();
	if (!XCommon::IsDaySame(nowtime, m_RebateConsumeTime))
	{
		if (m_RebateConsumeNum && m_RebateRate)
		{
			const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_Rebate);
			std::string mailtitle;
			std::string mailcontent;
			if (conf)
			{
				mailtitle = conf->m_title;
				mailcontent = MailConfig::Instance()->ReplaceString("%s", TimeUtil::GetDateTimeStr(m_RebateConsumeTime-SECONDS_OFFSET).substr(0,10), conf->m_content);
			}
			std::vector<ItemDesc> items;
			items.push_back(ItemDesc(DRAGON_COIN, UINT32(m_RebateConsumeNum*m_RebateRate/100)));
			MailSender sender;
			sender.SetReason(ItemFlow_ConsumeRebate);
			sender.SetContent(Mail_System, mailtitle,  mailcontent);
			sender.SetItems(items);
			sender.Send(m_pRole->GetID());
		}

		m_RebateConsumeTime = nowtime;
		m_RebateConsumeNum = 0;
		m_RebateRate = 0;
		Change();
	}
}

void CPayV2Record::AddRebate(INT32 amt)
{
	CheckRebate();
	m_RebateConsumeNum += amt;
	m_RebateConsumeTime = UINT32(GameTime::GetTime());
	m_RebateRate = CPayConsumeMgr::Instance()->GetRate(m_pRole->GetID());
	Change();
}
