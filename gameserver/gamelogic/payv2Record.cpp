#include "pch.h"
#include <time.h>
#include "payv2Record.h"
#include "payv2Mgr.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "util/XCommon.h"
#include "payconfig.h"
#include "checkinmgr.h"
#include "pay/ptcg2c_paymemberprivilegentf.h"
#include "ibshopmgr.h"
#include "util/gametime.h"
#include "define/tlogenum.h"

#include "buff/XBuff.h"
#include "entity/XRole.h"
#include "component/XBuffComponent.h"


void stPaytssInfo::ToKKSG(KKSG::PaytssInfo* pTssinfo) const
{
	pTssinfo->Clear();
	pTssinfo->set_begintime(m_iBeginTime);
	pTssinfo->set_endtime(m_iEndTime);
	pTssinfo->set_lastgetawardtime(m_iLastGetAwardTime);
}

void stPaytssInfo::ToThis(const KKSG::PaytssInfo& pTssinfo)
{
	m_iBeginTime = pTssinfo.begintime();
	m_iEndTime = pTssinfo.endtime();
	m_iLastGetAwardTime = pTssinfo.lastgetawardtime();
}

CPayV2Record::CPayV2Record(Role *pRole) : m_pRole(pRole)
{
	m_uLastUpdateDay = 0;
	m_TotalPay = 0;
	m_VipPoint = 0;
	m_GmTotalAmt = 0;
	m_VipLevel = 0;
	payCardButtonStatus = 0;
	payAileenButtonStatus = 0;
	m_uLastFirstPayAwardTime = 0;
	m_uPayFirstAwardButtonStatus = 0;
	m_uGrowthFundButtonStatus = 0;
	payInfo.clear();
	aileenInfo.clear();
	fundLevelInfo.clear();
	fundLoginInfo.clear();
	vipLevelGiftInfo.clear();
	memberInfo.clear();
	Init();
	m_iUsedChatCount = 0;
	m_iUsedAbyssCount = 0;
	m_iUseReviveCount = 0;
	m_iUsedBossRushCount = 0;
	m_iUsedBuyGreenAgateCount = 0;
	m_iUsedSuperRiskCount = 0;
	m_vPriShop.clear();
	m_isEverPay = false;
	m_rewardTime = 0;
	m_growthfundnotifytime = 0;
	m_vSetId.clear();
	m_vActId.clear();

	m_RebateConsumeNum = 0;
	m_RebateConsumeTime = 0;
	m_RebateRate = 0;
}

CPayV2Record::~CPayV2Record()
{

}
void CPayV2Record::Init()
{
	m_iChatcount = 0;
	m_iFatigueLimit = 0;
	m_iReviveCount = 0;
	m_iBossRushCount = 0;
	m_iBuyGreenAgateLimit = 0;
	m_iSuperRiskCount = 0;
	m_iAbyssCount = 0;
	m_iNpcFeelingCount = 0;
}
bool CPayV2Record::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_payv2())
	{
		const KKSG::PayV2Record &pay = poRoleAllInfo->payv2();
		m_TotalPay = pay.totalpay();
		m_VipPoint = pay.vippoint();
		m_VipLevel = pay.viplevel();
		payWeekCard.ToThis(pay.weekcard());
		payMonthCard.ToThis(pay.monthcard());
		payGrowthFund.ToThis(pay.growthfund());
		payCardButtonStatus = pay.paycardbuttonstatus();
		payAileenButtonStatus = pay.payaileenbuttonstatus();
		m_uLastFirstPayAwardTime = pay.lastfirstpayawardtime();
		m_uPayFirstAwardButtonStatus = pay.payfirstawardbuttonstatus();
		m_uGrowthFundButtonStatus = pay.growthfundbuttonstatus();
		m_uLastUpdateDay = pay.lastupdateday();
		m_isEverPay = pay.iseverpay();
		m_iUsedChatCount = pay.privilege().usedchatcount();
		m_iUseReviveCount = pay.privilege().usedrevivecount();
		m_iUsedAbyssCount = pay.privilege().usedabysscount();
		m_iUsedBossRushCount = pay.privilege().usedbossrushcount();
		m_iUsedBuyGreenAgateCount = pay.privilege().usedbuygreenagatecount();
		m_iUsedSuperRiskCount = pay.privilege().usedsuperriskcount();
		m_rewardTime = pay.rewardtime();
		m_growthfundnotifytime = pay.growthfundnotifytime();
		for (INT32 i = 0; i < pay.privilege().usedprivilegeshop_size(); i ++)
		{
			m_vPriShop.push_back(pay.privilege().usedprivilegeshop(i));
		}

		for (INT32 i = 0; i < pay.pay_size(); i ++)
		{
			payInfo.push_back(pay.pay(i));
		}
		for (INT32 i = 0; i < pay.aileen_size(); i ++)
		{
			aileenInfo.insert(std::make_pair(pay.aileen(i).paramid(), pay.aileen(i)));
		}

		for(INT32 i = 0; i < pay.growthfundlevelinfo_size(); i ++)
		{
			fundLevelInfo.push_back(pay.growthfundlevelinfo(i));
		}

		for (INT32 i = 0; i < pay.growthfundlogininfo_size(); i ++)
		{
			fundLoginInfo.push_back(pay.growthfundlogininfo(i));
		}

		for (INT32 i = 0; i < pay.viplevelgiftinfo_size(); i ++)
		{
			vipLevelGiftInfo.push_back(pay.viplevelgiftinfo(i));
		}

		for (INT32 i = 0; i < pay.paymemberinfo_size(); i ++)
		{
			memberInfo.push_back(pay.paymemberinfo(i));
		}

		for (INT32 i = 0; i < pay.consumelist_size(); i ++)
		{
			billListInfo.insert(std::make_pair(pay.consumelist(i).ts(), pay.consumelist(i).billno()));
		}

		for (INT32 i = 0; i < pay.consume().setid_size(); i ++)
		{
			m_vSetId.push_back(pay.consume().setid(i));
		}

		for (INT32 i = 0; i < pay.consume().activateid_size(); i ++)
		{
			m_vActId.push_back(pay.consume().activateid(i));
		}

		m_RebateConsumeNum = pay.rebate().consumenum();
		m_RebateConsumeTime = pay.rebate().lastconsumetime();
		m_RebateRate = pay.rebate().rate();
	}
	CheckMemberPrivilege();
	return true;
}

void CPayV2Record::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_isModified.TestAndReset())
	{
		KKSG::PayV2Record* pPay = poRoleAllInfo->mutable_payv2();
		pPay->Clear();
		pPay->set_totalpay(m_TotalPay);
		pPay->set_vippoint(m_VipPoint);
		pPay->set_viplevel(m_VipLevel);
		payWeekCard.ToKKSG(pPay->mutable_weekcard());
		payMonthCard.ToKKSG(pPay->mutable_monthcard());
		payGrowthFund.ToKKSG(pPay->mutable_growthfund());
		pPay->set_paycardbuttonstatus(payCardButtonStatus);
		pPay->set_payaileenbuttonstatus(payAileenButtonStatus);
		pPay->set_lastfirstpayawardtime(m_uLastFirstPayAwardTime);
		pPay->set_payfirstawardbuttonstatus(m_uPayFirstAwardButtonStatus);
		pPay->set_growthfundbuttonstatus(m_uGrowthFundButtonStatus);
		pPay->set_lastupdateday(m_uLastUpdateDay);
		pPay->set_iseverpay(m_isEverPay);
		pPay->set_rewardtime(m_rewardTime);
		pPay->set_growthfundnotifytime(m_growthfundnotifytime);
		
		KKSG::PayMemberPrivilege* pPrivilege =  pPay->mutable_privilege();
		pPrivilege->set_usedchatcount(m_iUsedChatCount);
		pPrivilege->set_usedrevivecount(m_iUseReviveCount);
		pPrivilege->set_usedabysscount(m_iUsedAbyssCount);
		pPrivilege->set_usedbossrushcount(m_iUsedBossRushCount);
		pPrivilege->set_usedbuygreenagatecount(m_iUsedBuyGreenAgateCount);
		pPrivilege->set_usedsuperriskcount(m_iUsedSuperRiskCount);

		for (UINT32 i = 0; i < m_vPriShop.size(); i ++)
		{
			KKSG::PayPrivilegeShop* pPriShop = pPrivilege->add_usedprivilegeshop();
			pPriShop->CopyFrom(m_vPriShop[i]);
		}

		for (UINT32 i = 0; i < payInfo.size(); i ++)
		{
			KKSG::PayBaseInfo* pBaseInfo = pPay->add_pay();
			pBaseInfo->CopyFrom(payInfo[i]);
		}

		for (auto i = aileenInfo.begin(); i != aileenInfo.end(); i ++)
		{
			KKSG::PayAileenRecord* pAileenRecord = pPay->add_aileen();
			pAileenRecord->CopyFrom(i->second);
		}

		for (UINT32 i = 0; i < fundLevelInfo.size(); i ++)
		{
			KKSG::PayAwardRecord* pAwardRecord = pPay->add_growthfundlevelinfo();
			pAwardRecord->CopyFrom(fundLevelInfo[i]);
		}

		for (UINT32 i = 0; i < fundLoginInfo.size(); i ++)
		{
			KKSG::PayAwardRecord* pAwardRecord = pPay->add_growthfundlogininfo();
			pAwardRecord->CopyFrom(pAwardRecord[i]);
		}

		for (UINT32 i = 0; i < vipLevelGiftInfo.size(); i ++)
		{
			KKSG::PayAwardRecord* pAwardRecord = pPay->add_viplevelgiftinfo();
			pAwardRecord->CopyFrom(vipLevelGiftInfo[i]);
		}

		for (UINT32 i = 0; i < memberInfo.size(); i ++)
		{
			KKSG::PayMemberRecord* pMemRecord = pPay->add_paymemberinfo();
			pMemRecord->CopyFrom(memberInfo[i]);
		}

		for (auto i = billListInfo.begin(); i != billListInfo.end(); i ++)
		{
			KKSG::PayconsumeBrief* pConsumeRecord = pPay->add_consumelist();
			pConsumeRecord->set_ts(i->first);
			pConsumeRecord->set_billno(i->second);
		}
		KKSG::PayConsume* pConsume = pPay->mutable_consume();
		GetPayScorePre(pConsume);

		KKSG::PayConsumeRebate* pRebate = pPay->mutable_rebate();
		pRebate->set_consumenum(m_RebateConsumeNum);
		pRebate->set_lastconsumetime(m_RebateConsumeTime);
		pRebate->set_rate(m_RebateRate);

		roChanged.insert(pPay);
	}
}

void CPayV2Record::Update()
{
	//主要是针对0点在线玩家通知奖励已刷新;
	UINT32 uUniqueDay = XCommon::GetTodayUnique();
	if (m_uLastUpdateDay != uUniqueDay)
	{
		m_uLastUpdateDay = uUniqueDay;
		PayV2Mgr::Instance()->PayCheck(m_pRole);

		m_iUsedAbyssCount= 0;
		m_iUsedBossRushCount = 0;
		m_iUseReviveCount = 0;
		m_iUsedSuperRiskCount = 0;
		m_iUsedNpcFeelingCount = 0;
		CheckRebate();
		ChangeAndNotify();
	}
}

void CPayV2Record::CheckMemberPrivilege()
{
	Init();
	for (UINT32 i = 0; i < memberInfo.size(); i ++)
	{
		if (memberInfo[i].expiretime() < (INT32)GameTime::GetTime())
		{
			continue;
		}
		PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberByID(memberInfo[i].id());
		if (NULL == pRow)
		{
			continue;
		}
		m_iChatcount += pRow->ChatCount;
		m_iFatigueLimit += pRow->FatigueLimit;
		m_iReviveCount += pRow->ReviveCount;
		m_iBossRushCount += pRow->BossRushCount;
		m_iBuyGreenAgateLimit += pRow->BuyGreenAgateLimit;
		m_iSuperRiskCount += pRow->SuperRiskCount;
		m_iAbyssCount += pRow->AbyssCount;
	}
	IBShopMgr::Instance()->GetPayMemberPrivilegeShop(m_pRole, m_iBuyGreenAgateLimit, m_vPriShop);
	m_isModified.Set();
}
bool CPayV2Record::IsCheckinDoubleDays(INT32 iDays)
{
	for (UINT32 i = 0; i < memberInfo.size(); i ++)
	{
		if (memberInfo[i].expiretime() < (INT32)GameTime::GetTime())
		{
			continue;
		}
		PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberByID(memberInfo[i].id());
		if (NULL == pRow)
		{
			continue;
		}
		if (CCheckinMgr::Instance()->IsOddMonth())
		{
			auto it = std::find(pRow->CheckinDoubleDays.begin(), pRow->CheckinDoubleDays.end(), iDays);
			if (it != pRow->CheckinDoubleDays.end())
			{
				return true;
			}
		}
		else
		{
			auto it = std::find(pRow->CheckinDoubleEvenDays.begin(), pRow->CheckinDoubleEvenDays.end(), iDays);
			if (it != pRow->CheckinDoubleEvenDays.end())
			{
				return true;
			}
		}
	}
	return false;
}

void CPayV2Record::UpateReviveCount()
{
	m_iUseReviveCount ++;
	ChangeAndNotify();
}

bool CPayV2Record::HasReviveCount()
{
	return m_iUseReviveCount < m_iReviveCount;
}

INT32 CPayV2Record::GetRemainAbyssCount()
{
	return m_iAbyssCount > m_iUsedAbyssCount ? m_iAbyssCount - m_iUsedAbyssCount : 0;
}

INT32 CPayV2Record::GetUsedAbyssCount()
{
	return m_iUsedAbyssCount;
}

void CPayV2Record::UpdateAbyssCount()
{
	m_iUsedAbyssCount ++;
	ChangeAndNotify();
}

void CPayV2Record::ClearAbyssCount()
{
	m_iUsedAbyssCount = 0;
	ChangeAndNotify();
}

INT32 CPayV2Record::GetRemainBossRushCount()
{
	return m_iBossRushCount > m_iUsedBossRushCount ? m_iBossRushCount - m_iUsedBossRushCount : 0;
}

INT32 CPayV2Record::GetUsedBossRushCount()
{
	return m_iUsedBossRushCount;
}

void CPayV2Record::UpdateBossRushCount()
{
	m_iUsedBossRushCount ++;
	ChangeAndNotify();
}

void CPayV2Record::ClearBossRushCount()
{
	m_iUsedBossRushCount = 0;
	ChangeAndNotify();
}

INT32 CPayV2Record::GetRemainSuperRiskCount()
{
	return m_iSuperRiskCount > m_iUsedSuperRiskCount ? m_iSuperRiskCount - m_iUsedSuperRiskCount : 0;
}

bool CPayV2Record::ReduceSuperRiskCount(INT32 iCount)
{
	if (GetRemainSuperRiskCount() >= iCount)
	{
		m_iUsedSuperRiskCount += iCount;
		ChangeAndNotify();
		return true;
	}
	return false;
}

INT32 CPayV2Record::GetRemainChatCount()
{
	return m_iChatcount > m_iUsedChatCount ? m_iChatcount - m_iUsedChatCount : 0;
}

void CPayV2Record::UpdateChatCount()
{
	m_iUsedChatCount ++;
	ChangeAndNotify();
}
void CPayV2Record::ClearUpdateChatCount()
{
	m_iUsedChatCount = 0;
	ChangeAndNotify();
}
void CPayV2Record::NotifyMemberPrivilege()
{
	PtcG2C_PayMemberPrivilegeNtf ntf;
	ntf.m_Data.set_usedabysscount(m_iUsedAbyssCount);
	ntf.m_Data.set_usedbossrushcount(m_iUsedBossRushCount);
	ntf.m_Data.set_usedbuygreenagatecount(m_iUsedBuyGreenAgateCount);
	ntf.m_Data.set_usedchatcount(m_iUsedChatCount);
	ntf.m_Data.set_usedrevivecount(m_iUseReviveCount);
	ntf.m_Data.set_usedsuperriskcount(m_iUsedSuperRiskCount);
	for (UINT32 i = 0; i < m_vPriShop.size(); i ++)
	{
		KKSG::PayPrivilegeShop* pShop = ntf.m_Data.add_usedprivilegeshop();
		pShop->CopyFrom(m_vPriShop[i]);
	}
	m_pRole->Send(ntf);
}

INT32 CPayV2Record::GetRemainShopCount(INT32 iGoodsID)
{
	auto it = std::find_if(m_vPriShop.begin(), m_vPriShop.end(), EqualPayMemberID(iGoodsID));
	if (it != m_vPriShop.end())
	{
		return it->totalcount() > it->usedcount() ? it->totalcount() - it->usedcount() : 0;
	}
	return 0;
}

void CPayV2Record::UpdateShopCount(INT32 iGoodsID, INT32 iCount)
{
	auto it = std::find_if(m_vPriShop.begin(), m_vPriShop.end(), EqualPayMemberID(iGoodsID));
	if (it != m_vPriShop.end())
	{
		it->set_usedcount(it->usedcount()+iCount);
		ChangeAndNotify();
	}
}

void CPayV2Record::RefreshShopCount()
{
	for (UINT32 i = 0; i < m_vPriShop.size(); i ++)
	{
		m_vPriShop[i].set_usedcount(0);
	}
	ChangeAndNotify();
}
void CPayV2Record::ChangeAndNotify()
{
	Change();
	NotifyMemberPrivilege();
}

void CPayV2Record::SavePayMemberMask()
{
	UINT32 ID = 0;
	for (UINT32 i = 0; i < memberInfo.size(); i ++)
	{
		if (memberInfo[i].expiretime() > INT32(GameTime::GetTime()))
		{
			ID |= (1 << memberInfo[i].id());
		}
	}
	m_pRole->SetPayMemberID(ID);
}

INT32 CPayV2Record::GetExpireTimeByID(INT32 ID)
{
	auto it = std::find_if(memberInfo.begin(), memberInfo.end(), EqualPayMemberID(ID));
	if (it != memberInfo.end())
	{
		return it->expiretime() < INT32(GameTime::GetTime()) ? 0 : it->expiretime();
	}
	return 0;
}

void CPayV2Record::SetIsEverPay()
{
	m_isEverPay = true;
	m_isModified.Set();
}

bool CPayV2Record::AddConsumeBill(std::string billno)
{
	auto it = std::find_if(billListInfo.begin(), billListInfo.end(), EqualBillNo(billno));
	if (it != billListInfo.end())
	{
		return false;
	}
	if (billListInfo.size() >= MAX_PAY_BILL_COUNT)
	{
		auto it = billListInfo.begin();
		billListInfo.erase(it);
	}
	billListInfo.insert(std::make_pair(UINT32(GameTime::GetTime()), billno));
	m_isModified.Set();
	return true;
}

void CPayV2Record::GiveBuff(KKSG::SceneType type)
{
	if(type != KKSG::SCENE_WORLDBOSS&&type!=KKSG::SCENE_GUILD_BOSS)
		return;
	UINT32 nTime = GetExpireTimeByID(pay_member_2);
	if (nTime == 0)
		return;
	PayMemberTable::RowData* pRowData =  CPayConfig::Instance()->GetPayMemberByID(pay_member_2);
	if (pRowData == NULL)
		return;
	if (m_pRole->GetXObject())
	{
		XAddBuffActionArgs args;
		if (type == KKSG::SCENE_WORLDBOSS)
		{
			if (pRowData->worldBossbuffid.Size()==2)
			{
				args.singleBuff.buffID = pRowData->worldBossbuffid[0];
				args.singleBuff.BuffLevel = pRowData->worldBossbuffid[1];
				args.Caster = m_pRole->GetID();
				m_pRole->GetXObject()->DispatchAction(&args);
			}

		}else if (type == KKSG::SCENE_GUILD_BOSS)
		{
			if (pRowData->guildBossBuffid.Size()==2)
			{
				args.singleBuff.buffID = pRowData->guildBossBuffid[0];
				args.singleBuff.BuffLevel = pRowData->guildBossBuffid[1];
				args.Caster = m_pRole->GetID();
				m_pRole->GetXObject()->DispatchAction(&args);
			}
		}
	}
}

UINT32 CPayV2Record::RewardCoolTime()
{
	UINT32 renewTime = XCommon::GetDayBeginTime(GameTime::GetTime()); // 凌晨5点刷新
	return m_rewardTime > renewTime ? (24 * 60 * 60 - (m_rewardTime - renewTime)) : 0;
}

void CPayV2Record::SetRewardTime()
{
	m_rewardTime = GameTime::GetTime(); 
	m_isModified.Set();
}

bool CPayV2Record::IsBuyGrowFund()
{
	return (payGrowthFund.m_iBeginTime&&payGrowthFund.m_iEndTime)?true:false;
}