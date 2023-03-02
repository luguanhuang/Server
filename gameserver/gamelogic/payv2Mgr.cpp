#include "pch.h"
#include "payv2Mgr.h"
#include "payv2Record.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include <time.h>
#include "pb/enum.pb.h"
#include "gamelogic/bagtransition.h"
#include "pay/ptcg2c_payallinfontf.h"
#include "define/systemiddef.h"
#include "gamelogic/loginrecord.h"
#include "payconfig.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "util/gametime.h"
#include "common/ptcg2c_itemchangedntf.h"
#include "unit/systeminfo.h"
#include "define/systemiddef.h"
#include "utility/tlogger.h"
#include "table/OpenSystemMgr.h"
#include "spactivity/spactivitydatasmall.h"
#include "define/spactivityiddef.h"


INSTANCE_SINGLETON(PayV2Mgr)

PayV2Mgr::PayV2Mgr()
{
}

PayV2Mgr::~PayV2Mgr()
{

}

bool PayV2Mgr::Init()
{
	if (!m_oVipTable.LoadFile("table/Vip.txt"))
	{
		LogWarn("Load table %s failed!", "table/Vip.txt");
		return false;
	}
	m_ExpireSoonTime = 3 * (INT32)XCommon::GetOneDayTime();
	m_oConsumeLister[MidasConsumeType_IBShop] = &IBShopConsume::Listener;
	m_oConsumeLister[MidasConsumeType_ExchangeDragon] = &ExchangeDragonConsume::Listener;
	m_oConsumeLister[MidasConsumeType_CustomBattle] = &CustomBattleConsume::Listener;
	m_oConsumeLister[MidasConsumeType_BackFlow] = &BackFlowConsume::Listener;
	m_oConsumeLister[MidasConsumeType_LotteryDraw] = &LotteryDrawConsume::Listener;
	m_oConsumeLister[MidasConsumeType_SummonSpirit] = &GuildPartySummonSpiritConsume::Listener;
	m_oConsumeLister[MidasConsumeType_BackFlowShop] = &BackFlowShopConsume::Listener;
	return true;
}

void PayV2Mgr::Uninit()
{
}

bool PayV2Mgr::CheckFile()
{
	VIPTable tmpTable;
	if (!tmpTable.LoadFile("table/Vip.txt"))
	{
		LogWarn("Load table %s failed!", "table/Vip.txt");
		return false;
	}

	return true;
}

bool PayV2Mgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oVipTable.LoadFile("table/Vip.txt");

	return true;
}

void PayV2Mgr::ClearFile()
{
	m_oVipTable.Clear();
}

void PayV2Mgr::GetPayInfo(Role* pRole, KKSG::PayAllInfo* pPayInfo)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	INT32 nDays = 0;
	UINT32 uRemainCount = 0;
	bool bIsGet = false;
	bool bIsNew = true;

	//周月卡信息
	if (pRecord->payWeekCard.m_iEndTime)//周卡
	{
		KKSG::PayCard* pWeekCard = pPayInfo->add_card();
		pWeekCard->set_type(KKSG::WEEK_CARD);
		if ((pRecord->payWeekCard.m_iEndTime < INT32(GameTime::GetTime())) || XCommon::IsDaySame((time_t)pRecord->payWeekCard.m_iEndTime, GameTime::GetTime()))
		{
			uRemainCount = 0;
			bIsGet = false;
		}
		else
		{
			nDays = GetRemainedDays(pRecord->payWeekCard.m_iEndTime);
			bIsGet = XCommon::IsDaySame(GameTime::GetTime(), pRecord->payWeekCard.m_iLastGetAwardTime)?true:false;
			uRemainCount = bIsGet?nDays-1:nDays;
		}
		pWeekCard->set_remainedcount(uRemainCount);
		pWeekCard->set_isget(bIsGet);
	}

	if (pRecord->payMonthCard.m_iEndTime)//月卡
	{
		KKSG::PayCard* pMonthCard = pPayInfo->add_card();
		pMonthCard->set_type(KKSG::MONTH_CARD);
		if ((pRecord->payMonthCard.m_iEndTime < INT32(GameTime::GetTime())) || XCommon::IsDaySame((time_t)pRecord->payMonthCard.m_iEndTime, GameTime::GetTime()))
		{
			uRemainCount = 0;
			bIsGet = false;
		}
		else
		{
			nDays = GetRemainedDays(pRecord->payMonthCard.m_iEndTime);
			bIsGet = XCommon::IsDaySame(GameTime::GetTime(), pRecord->payMonthCard.m_iLastGetAwardTime)?true:false;
			uRemainCount = bIsGet?nDays-1:nDays;
		}
		pMonthCard->set_remainedcount(uRemainCount);
		pMonthCard->set_isget(bIsGet);
	}
	pPayInfo->set_paycardremaintime((UINT32)(XCommon::GetDayBeginTime(GameTime::GetTime()) + XCommon::GetOneDayTime() - GameTime::GetTime()));

	//每日艾琳礼包
	GetPayAileenInfo(pRole);
	//配置表中有的，但是保存中的数据没有，也需要添加进来;
	CPayConfig::Instance()->CheckAileenConfig(pRecord, pPayInfo);

	//充值信息
	KKSG::PayBaseInfo* pBaseInfo = NULL;
	for (auto i = pRecord->payInfo.begin(); i != pRecord->payInfo.end(); i ++)
	{
		pBaseInfo = pPayInfo->add_pay();
		pBaseInfo->set_paramid(i->paramid());
		pBaseInfo->set_ispay(i->ispay());
	}

	//vip信息
	pPayInfo->set_totalpay(pRecord->m_TotalPay + pRecord->m_VipPoint);
	UINT32 nLevel = GetVipLevel(pRecord->m_TotalPay/100 + pRecord->m_VipPoint/100);
	if (pRecord->m_VipLevel != nLevel)
	{
		//测试时发现vip配置被改时会有问题，所以加上判断;
		pRecord->m_VipLevel = nLevel;
		pRecord->m_isModified.Set();
		pRole->OnChangeVip();
	}
	pPayInfo->set_viplevel(pRecord->m_VipLevel);

	if (pRecord->payCardButtonStatus == KKSG::PAY_BUTTON_CLICK)
	{
		pPayInfo->set_paycardfirstclick(true);
	}
	else
	{
		pPayInfo->set_paycardfirstclick(false);
	}

	if (pRecord->payAileenButtonStatus == KKSG::PAY_BUTTON_CLICK)
	{
		pPayInfo->set_payaileenfirstclick(true);
	}
	else
	{
		pPayInfo->set_payaileenfirstclick(false);
	}

	//首充奖励相关信息;
	if (pRecord->m_uLastFirstPayAwardTime)
	{
		pPayInfo->set_payfirstaward(true);
	}
	else
	{
		pPayInfo->set_payfirstaward(false);
	}

	if (pRecord->m_uPayFirstAwardButtonStatus == KKSG::PAY_BUTTON_CLICK)
	{
		pPayInfo->set_payfirstawardclick(true);
	}
	else
	{
		pPayInfo->set_paycardfirstclick(false);
	}

	//成长基金相关信息;
	if (pRecord->payGrowthFund.m_iEndTime > (INT32)GameTime::GetTime())
	{
		pPayInfo->set_buygrowthfund(true);
	}
	else
	{
		pPayInfo->set_buygrowthfund(false);
	}

	if (pRecord->m_uGrowthFundButtonStatus == KKSG::PAY_BUTTON_CLICK)
	{
		pPayInfo->set_growthfundclick(true);
	}
	else
	{
		pPayInfo->set_growthfundclick(false);
	}

	for (auto i = pRecord->fundLevelInfo.begin(); i != pRecord->fundLevelInfo.end(); i ++)
	{
		pPayInfo->add_growthfundlevelinfo(i->id());
	}

	for (auto i = pRecord->fundLoginInfo.begin(); i != pRecord->fundLoginInfo.end(); i ++)
	{
		pPayInfo->add_growthfundlogininfo(i->id());//成长基金中领取过登录相关的信息;
	}

	for (auto i = pRecord->vipLevelGiftInfo.begin(); i != pRecord->vipLevelGiftInfo.end(); i ++)
	{
		pPayInfo->add_viplevelgift(i->id());//已经领取过vip等级礼包的vip等级;
	}
	pPayInfo->set_totallogindays(pRole->Get<MyLoginRecord>()->GetRealLoginDayCount());

	CPayConfig::Instance()->CheckMemberConfig(pRecord, pPayInfo);
	pPayInfo->set_isiosopen(pRole->Get<CRoleSystem>()->IsSystemOpened(XSys_Pay));
	pPayInfo->set_rewardcooltime(pRecord->RewardCoolTime());
}

void PayV2Mgr::GetPayAileenInfo(Role* pRole)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return;
	}
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	UINT32 nDays = XCommon::GetWeekNum(GameTime::GetTime());
	//检查保存中的数据;
	for (auto i = pRecord->aileenInfo.begin(); i != pRecord->aileenInfo.end(); )
	{
		PayAileenTable::RowData* rowdata = CPayConfig::Instance()->GetTodayPayAileenTable(i->second.paramid());
		INT32 itemid = CPayConfig::Instance()->GetAileenItemID(i->second.paramid());
		//不是当日礼包，则删除数据;
		if (NULL == rowdata || !itemid)
		{
			pRecord->aileenInfo.erase(i++);
			pRecord->m_isModified.Set();
		}
		else //是今天的礼包;
		{
			if (!XCommon::IsDaySame(GameTime::GetTime(), i->second.lastbuytime()))//今天没买过
			{
				if (itemid != i->second.itemid())
				{
					i->second.set_itemid(itemid);
				}
				i->second.set_lastbuytime(0);
				pRecord->m_isModified.Set();
			}
			i ++;
		}
	}
	return;
}


INT32 PayV2Mgr::Pay(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData)
{
	INT32 ret = KKSG::ERR_FAILED;
	switch (CPayConfig::Instance()->GetParamType(ParamId))
	{
	case KKSG::PAY_PARAM_AILEEN:
		{
			ret = BuyAileen(pRole, ParamId, oData);
		}
		break;
	case KKSG::PAY_PARAM_CARD:
		{
			ret = BuyCard(pRole, ParamId, oData);
		}
		break;
	case KKSG::PAY_PARAM_GROWTH_FUND:
		{
			ret = BuyGrowthFund(pRole, ParamId, oData);
		}
		break;
	case KKSG::PAY_PARAM_MEMBER:
		{
			ret = BuyMember(pRole, ParamId, oData);
		}
		break;
	default:
		break;
	}
	return ret;
}
INT32 PayV2Mgr::BuyPay(Role* pRole, INT32 iBalance, INT32 iSaveAmt)
{
	NotifyDiamond(pRole, iBalance);	
	if (iSaveAmt <= 0)
	{
		return KKSG::ERR_SUCCESS;
	}
	CalcVipLevel(pRole, iSaveAmt*10, KKSG::PAY_PARAM_LIST);
	LogInfo("buypay roleid=%llu,balance=%d,saveamt=%d", pRole->GetID(), iBalance, iSaveAmt);
	return KKSG::ERR_SUCCESS;
}

INT32 PayV2Mgr::BuyCard(Role* pRole, std::string szParamID, KKSG::PaytssInfo oData)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	PayCardTable::RowData* rowdata = CPayConfig::Instance()->GetPayCardTable().GetByParamID(szParamID);
	if (NULL == rowdata)
	{
		LogError("pay card roleid(%llu) no paramid(%s),invalid config", pRole->GetID(), szParamID.c_str());
		return KKSG::ERR_FAILED;
	}

	if (rowdata->Type != KKSG::WEEK_CARD && rowdata->Type != KKSG::MONTH_CARD)
	{
		LogError("pay card roleid(%llu) paramid(%s),invalid type", pRole->GetID(), szParamID.c_str());
		return KKSG::ERR_FAILED;
	}

	if (rowdata->Type == KKSG::WEEK_CARD)
	{
		if (pRecord->payWeekCard.m_iBeginTime == oData.begintime() && pRecord->payWeekCard.m_iEndTime == oData.endtime())
		{
			LogInfo("pay weekcard already set roleid=%llu,paramid=%s,begin=%d,end=%d", pRole->GetID(), szParamID.c_str(),  oData.begintime(), oData.endtime());
			return KKSG::ERR_UNKNOWN;
		}
		if (rowdata->Diamond && !pRole->AddDiamond(rowdata->Diamond, ItemFlow_Pay, ItemFlow_BuyPayCard))
		{
			SSWarn << "add diamond send to ms error" << END;
			return KKSG::ERR_FAILED;
		}
		pRecord->payWeekCard.ToThis(oData);
		pRecord->payWeekCard.m_iLastGetAwardTime = (INT32)GameTime::GetTime();
	}
	else
	{
		if (pRecord->payMonthCard.m_iBeginTime == oData.begintime() && pRecord->payMonthCard.m_iEndTime == oData.endtime())
		{
			LogInfo("pay monthcard already set roleid=%llu,paramid=%s,begin=%d,end=%d", pRole->GetID(), szParamID.c_str(),  oData.begintime(), oData.endtime());
			return KKSG::ERR_SUCCESS;
		}
		if (rowdata->Diamond && !pRole->AddDiamond(rowdata->Diamond, ItemFlow_Pay, ItemFlow_BuyPayCard))
		{
			SSWarn << "add diamond send to ms error" << END;
			return KKSG::ERR_FAILED;
		}
		pRecord->payMonthCard.ToThis(oData);
		pRecord->payMonthCard.m_iLastGetAwardTime = (INT32)GameTime::GetTime();
	}
	pRole->HintNotify(Welfare_DiamondCard, false);
	pRecord->m_isModified.Set();
	CalcVipLevel(pRole, rowdata->Price, KKSG::PAY_PARAM_CARD);
	PayCardHintNotify(pRole);

	//开始发奖
	DoTxLog(pRole, rowdata->Price, rowdata->ParamID);
	return KKSG::ERR_SUCCESS;
}
INT32 PayV2Mgr::BuyAileen(Role* pRole, std::string szParamID, KKSG::PaytssInfo oData)
{
	INT32 now = INT32(GameTime::GetTime());
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	PayAileenTable::RowData* rowdata = CPayConfig::Instance()->GetTodayPayAileenTable(szParamID);
	if (NULL == rowdata)
	{
		LogError("roleid(%llu) pay aileen,paramid(%s),today no gift", pRole->GetID(), szParamID.c_str());
		return KKSG::ERR_FAILED;
	}

	//判断下是否是当日礼包，vip等级限制;
	if (!CPayConfig::Instance()->GetAileenItemID(rowdata->ParamID))
	{
		LogError("pay aileen roleid(%llu) paramid(%s),today no itemid", pRole->GetID(), szParamID.c_str());
		return KKSG::ERR_FAILED;
	}
	//判断是否满足会员限制
	if (rowdata->MemberLimit)
	{
		auto it = std::find_if(pRecord->memberInfo.begin(), pRecord->memberInfo.end(), EqualPayMemberID(rowdata->MemberLimit));
		if (it == pRecord->memberInfo.end() || it->expiretime() < now)
		{
			LogError("pay aileen roleid(%llu) paramid(%s),don't meet the memberlimit(%d)",pRole->GetID(), szParamID.c_str(), rowdata->MemberLimit);
			return KKSG::ERR_FAILED;
		}
	}
	if (pRecord->GetVipLevel() < rowdata->VipLimit )
	{
		LogError("pay aileen roleid(%llu) paramid(%s),player vip (%d) not enough config vip(%d)", pRole->GetID(), szParamID.c_str(), pRecord->GetVipLevel(), rowdata->VipLimit);
		return KKSG::ERR_FAILED;
	}
	INT32 itemid = CPayConfig::Instance()->GetAileenItemID(szParamID);
	if (!itemid)
	{
		LogError("pay aileen roleid(%llu) paramid(%s),invalid config,no item", pRole->GetID(), szParamID.c_str());
		return KKSG::ERR_FAILED;
	}

	//判断当日礼包是否购买过
	auto i = pRecord->aileenInfo.find(szParamID);
	if (i != pRecord->aileenInfo.end())
	{
		if (i->second.detail().begintime() == oData.begintime() && i->second.detail().endtime() == oData.endtime())
		{
			LogInfo("pay aileen already set roleid=%llu,paramid=%s,begin=%d,end=%d", pRole->GetID(), szParamID.c_str(),  oData.begintime(), oData.endtime());
			return KKSG::ERR_UNKNOWN;
		}
		if (XCommon::IsDaySame(time_t(now), i->second.lastdelivertime()))//今天已经购买过艾琳礼包
		{
			LogError("already buy aileen roleid(%llu) paramid(%s),buytime(%d) today no itemid", pRole->GetID(), szParamID.c_str(), i->second.lastbuytime());
			return KKSG::ERR_ALREADY_BUYAILEEN;
		}

		if (rowdata->Diamond && !pRole->AddDiamond(rowdata->Diamond, ItemFlow_Pay, ItemFlow_BuyAileen))
		{
			SSError << "add diamond send to ms fail" << END;
			return KKSG::ERR_FAILED;
		}
		if (!XCommon::IsDaySame(time_t(now), i->second.lastbuytime()))//今天是否已经购买的状态没有设置;
		{
			i->second.set_lastbuytime(now);
		}
		i->second.set_itemid(itemid);
		i->second.set_lastdelivertime(now);
		i->second.mutable_detail()->CopyFrom(oData);
	}
	else
	{
		if (rowdata->Diamond && !pRole->AddDiamond(rowdata->Diamond, ItemFlow_Pay, ItemFlow_BuyAileen))
		{
			SSError << "add diamond send to ms fail" << END;
			return KKSG::ERR_FAILED;
		}
		else
		{
			KKSG::PayAileenRecord roAileen;
			roAileen.set_paramid(rowdata->ParamID);
			roAileen.set_itemid(itemid);
			roAileen.set_lastbuytime(now);
			roAileen.set_lastdelivertime(now);
			roAileen.mutable_detail()->CopyFrom(oData);
			pRecord->aileenInfo.insert(std::make_pair(rowdata->ParamID, roAileen));
		}
	}
	pRecord->m_isModified.Set();
		
	CalcVipLevel(pRole, rowdata->Price, KKSG::PAY_PARAM_AILEEN);

	bool isdouble = false;
	SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
	if (backflow && backflow->GetPayGiftCount(rowdata->ParamID) < GetGlobalConfig().BackFlowPayAlieenCount)
	{
		isdouble = true;
		backflow->UpdatePayGiftCount(szParamID);
	}

	UINT32 itemcount = 1;
	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_Reward, ItemFlow_BuyAileen, false);
	stTransition.GiveItem(itemid, isdouble?2*itemcount:itemcount);
	stTransition.NotifyClient();

	pRole->OnBuyAileen();
	DoTxLog(pRole, rowdata->Price, rowdata->ParamID);
	return KKSG::ERR_SUCCESS;
}
INT32 PayV2Mgr::BuyGrowthFund(Role* pRole, std::string szParamID, KKSG::PaytssInfo oData)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	RechargeTable::RowData* rowdata = CPayConfig::Instance()->GetPayGrowthFundTable().GetByParamID(szParamID);
	if (NULL == rowdata)
	{
		LogError("pay growthfund roleid(%llu) no paramid(%s),invalid config", pRole->GetID(), szParamID.c_str());
		return KKSG::ERR_FAILED;
	}
	if (pRecord->payGrowthFund.m_iBeginTime == oData.begintime() && pRecord->payGrowthFund.m_iEndTime == oData.endtime())
	{
		LogInfo("pay growthfund already set roleid=%llu,paramid=%s,begin=%d,end=%d", pRole->GetID(), szParamID.c_str(),  oData.begintime(), oData.endtime());
		return KKSG::ERR_UNKNOWN;
	}

	if (pRecord->payGrowthFund.m_iBeginTime && pRecord->payGrowthFund.m_iEndTime)
	{
		LogError("already bought growthfund,roleid=%llu, begintime=%d,endtime=%d", pRole->GetID(), pRecord->payGrowthFund.m_iBeginTime, pRecord->payGrowthFund.m_iEndTime);
		return KKSG::ERR_FAILED;
	}
	if (rowdata->Diamond && !pRole->AddDiamond(rowdata->Diamond, ItemFlow_Pay, ItemFlow_BuyGrowthFund))
	{
		SSError << "add diamond send to ms fail" << END;
		return KKSG::ERR_FAILED;

	}
	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(CPayConfig::Instance()->GetGrowthFundSystemID()))
	{
		CloseGrowthFund(pRole, true);//防止系统已经关闭，但是充值延迟到账的问题;
	}
	oData.set_lastgetawardtime(0);
	pRecord->payGrowthFund.ToThis(oData);
	pRecord->m_isModified.Set();
	CalcVipLevel(pRole, rowdata->Price, KKSG::PAY_PARAM_GROWTH_FUND);
	GrowthFundHintNotify(pRole);
	pRole->OnBuyGrouwthFund();
	DoTxLog(pRole, rowdata->Price, rowdata->ParamID);
	return KKSG::ERR_SUCCESS;
}
INT32 PayV2Mgr::BuyMember(Role* pRole, std::string szParamID, KKSG::PaytssInfo oData)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	PayMemberTable::RowData* rowdata = CPayConfig::Instance()->GetPayMemberTable().GetByParamID(szParamID);
	if (NULL == rowdata)
	{
		LogError("pay member roleid(%llu) no paramid(%s),invalid config", pRole->GetID(), szParamID.c_str());
		return KKSG::ERR_FAILED;
	}
	auto it = std::find_if(pRecord->memberInfo.begin(), pRecord->memberInfo.end(), EqualPayMemberID(rowdata->ID));
	if (it == pRecord->memberInfo.end())
	{
		KKSG::PayMemberRecord roMemInfo;
		roMemInfo.set_id(rowdata->ID);
		roMemInfo.set_isclick(false);
		roMemInfo.set_begintime(oData.begintime());
		roMemInfo.set_expiretime(oData.endtime());
		roMemInfo.set_isnotifyexpire(false);
		roMemInfo.set_isnotifyexpiresoon(false);
		roMemInfo.set_buttonstatus(KKSG::PAY_BUTTON_NONE);
		roMemInfo.set_lastdragonflowertime(INT32(GameTime::GetTime()));
		pRecord->memberInfo.push_back(roMemInfo);
		SendMemberDailyAward(pRole, rowdata, INT32(GameTime::GetTime()));
	}
	else
	{
		if (oData.begintime() == it->begintime() && oData.endtime() == it->expiretime())
		{
			LogInfo("pay member already set roleid=%llu,paramid=%s,begin=%d,end=%d", pRole->GetID(), szParamID.c_str(),  oData.begintime(), oData.endtime());
			return KKSG::ERR_FAILED;
		}

		if (it->expiretime() <= (INT32)GameTime::GetTime())
		{
			if (SendMemberDailyAward(pRole, rowdata, INT32(GameTime::GetTime())))
			{
				it->set_lastdragonflowertime(INT32(GameTime::GetTime()));
			}
		}
		it->set_begintime(oData.begintime());
		it->set_expiretime(oData.endtime());
		it->set_isnotifyexpire(false);
		it->set_isnotifyexpiresoon(false);
	}
	pRole->OnBuyPayMember(rowdata->ID);
	ChangePayMember(pRole, rowdata->ID);
	CalcVipLevel(pRole, rowdata->Price, KKSG::PAY_PARAM_MEMBER);
	pRecord->m_isModified.Set();
	DoTxLog(pRole, rowdata->Price, rowdata->ParamID);
	return KKSG::ERR_SUCCESS;
}
void PayV2Mgr::SetButtonStatus(Role* pRole, INT32 nSystemID)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	if (nSystemID == Welfare_DiamondCard)
	{
		pRecord->payCardButtonStatus = KKSG::PAY_BUTTON_OPEN;//开启系统，未点击过
	}
	else if (nSystemID == Welfare_GiftBag)
	{
		pRecord->payAileenButtonStatus = KKSG::PAY_BUTTON_OPEN;
	}
	else if (nSystemID == CPayConfig::Instance()->GetGrowthFundSystemID() && GetGrowthFundLeftTime())
	{
		pRecord->m_uGrowthFundButtonStatus = KKSG::PAY_BUTTON_OPEN;
	}
	else if (nSystemID == CPayConfig::Instance()->GetPayFirstSystemID())
	{
		pRecord->m_uPayFirstAwardButtonStatus = KKSG::PAY_BUTTON_OPEN;
	}
	else
	{
		//会员特权
		PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberBySystem(nSystemID);
		if (pRow)
		{
			auto it = std::find_if(pRecord->memberInfo.begin(), pRecord->memberInfo.end(), EqualPayMemberID(pRow->ID));
			if (it != pRecord->memberInfo.end())
			{
				it->set_buttonstatus(KKSG::PAY_BUTTON_OPEN);
			}
			else
			{
				KKSG::PayMemberRecord roMemInfo;
				roMemInfo.set_id(pRow->ID);
				roMemInfo.set_isclick(false);
				roMemInfo.set_expiretime(0);
				roMemInfo.set_isnotifyexpire(false);
				roMemInfo.set_buttonstatus(KKSG::PAY_BUTTON_OPEN);
				pRecord->memberInfo.push_back(roMemInfo);
			}
		}
	}
	pRecord->m_isModified.Set();
	pRole->HintNotify(nSystemID, false);
}


void PayV2Mgr::CheckPaySystemID(Role* pRole, INT32 nSystemID)
{
	if (CPayConfig::Instance()->IsPaySystemID(nSystemID))
	{
		SetButtonStatus(pRole, nSystemID);
	}
}

void PayV2Mgr::CheckMemberDragonFlowerAward(Role* pRole)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	for (UINT32 i = 0; i < pRecord->memberInfo.size(); i ++)
	{
		PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberByID(pRecord->memberInfo[i].id());
		if (NULL == pRow)
		{
			continue;
		}
		if (!pRow->DragonCoin && (!pRow->Flower[0] || !pRow->Flower[1]) && (!pRow->PetFeedAdd[0] || !pRow->PetFeedAdd[1]) && (!pRow->SpriteFeedAdd[0] || !pRow->SpriteFeedAdd[1]))
		{
			continue;
		}
		//未开通;
		if (!pRecord->memberInfo[i].expiretime())
		{
			continue;
		}
		//当天已经发过奖励
		if (XCommon::IsDaySame(GameTime::GetTime(), pRecord->memberInfo[i].lastdragonflowertime()) || !pRecord->memberInfo[i].lastdragonflowertime() || pRecord->memberInfo[i].lastdragonflowertime() > INT32(GameTime::GetTime()))
		{
			continue;
		}
		//已经过期且已发放完奖励
		if (pRecord->memberInfo[i].lastdragonflowertime() >= pRecord->memberInfo[i].expiretime())
		{
			continue;
		}
		//开始发奖
		time_t tTime = pRecord->memberInfo[i].expiretime() > GameTime::GetTime() ? (GameTime::GetTime()) : (time_t)pRecord->memberInfo[i].expiretime();
		INT32 Days = (INT32)XCommon::GetDiffDayCount(tTime, pRecord->memberInfo[i].lastdragonflowertime());
		for (INT32 j = 1; j <= Days; ++ j)
		{
			SendMemberDailyAward(pRole, pRow, INT32(pRecord->memberInfo[i].lastdragonflowertime() + j * XCommon::GetOneDayTime()));
		}
		pRecord->memberInfo[i].set_lastdragonflowertime(INT32(GameTime::GetTime()));
		pRecord->m_isModified.Set();
	}
}
void PayV2Mgr::SendMail(Role* pRole, INT32 iID, INT32 iTime)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(iID);
	if (conf)
	{
		std::string mailtile;
		std::string mailcontent;
		mailtile = conf->m_title;
		mailcontent = MailConfig::Instance()->ReplaceString("x", TimeUtil::GetDateTimeStr(iTime), conf->m_content);
		MailSender sender;
		sender.SetContent(Mail_System, mailtile, mailcontent);
		sender.Send(pRole->GetID());
	}
}
void PayV2Mgr::CheckMemberExpire(Role* pRole)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	INT32 nowtime = INT32(GameTime::GetTime());
	for (UINT32 i = 0; i < pRecord->memberInfo.size(); ++ i)
	{
		if (!pRecord->memberInfo[i].expiretime())
		{
			continue;
		}
		PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberByID(pRecord->memberInfo[i].id());

		if (!pRecord->memberInfo[i].isnotifyexpire() && pRecord->memberInfo[i].expiretime() < nowtime)//过期了
		{
			if (pRow)
			{
				SendMail(pRole, pRow->ExpireMailID, pRecord->memberInfo[i].expiretime());
			}
			pRecord->memberInfo[i].set_isnotifyexpire(true);
			pRecord->CheckMemberPrivilege();
			pRecord->SavePayMemberMask();
			PayV2Mgr::Instance()->NotifyPayAllInfoToClient(pRole, KKSG::PAY_PARAM_MEMBER);
			pRecord->m_isModified.Set();
			LogInfo("role=%llu pay memeber id(%d) expired.", pRole->GetID(), pRecord->memberInfo[i].id());
		}
		else if (!pRecord->memberInfo[i].isnotifyexpiresoon() && nowtime < pRecord->memberInfo[i].expiretime() && (nowtime + m_ExpireSoonTime) >= pRecord->memberInfo[i].expiretime())//即将过期的;
		{
			if(pRow)
			{
				SendMail(pRole, pRow->ExpireSoonMailID, pRecord->memberInfo[i].expiretime());
			}
			pRecord->memberInfo[i].set_isnotifyexpiresoon(true);
			pRecord->m_isModified.Set();
		}
	}

	//检查是否需要发送基金即将关闭的通知,只针对未购买过的;
	if (!pRecord->IsBuyGrowFund())
	{
		if (!XCommon::IsDaySame(pRecord->m_growthfundnotifytime, nowtime) && GetGrowthFundCloseTime() > nowtime && (nowtime + GetGlobalConfig().GrowthFundExpireMailDays * XCommon::GetOneDayTime()) >= GetGrowthFundCloseTime())
		{
			SendMail(pRole, MailConf_GrowthFundCloseSoon, GetGrowthFundCloseTime());
			pRecord->m_growthfundnotifytime = nowtime;
			pRecord->m_isModified.Set();
		}
		if (pRole->Get<CRoleSystem>()->IsSystemOpened(CPayConfig::Instance()->GetGrowthFundSystemID()) && nowtime >= GetGrowthFundCloseTime())
		{
			CloseGrowthFund(pRole);
		}
	}


}
void PayV2Mgr::ChangePayMember(Role* pRole, INT32 iID)
{
	pRole->Get<CPayV2Record>()->CheckMemberPrivilege();
	pRole->Get<CPayV2Record>()->SavePayMemberMask();
	pRole->OnChangePayMember(iID);
}
bool PayV2Mgr::SetPayMemberExpireTime(Role* pRole, INT32 iID, INT32 iTime)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	INT32 iExpire = INT32(GameTime::GetTime()) + iTime;
	auto it = std::find_if(pRecord->memberInfo.begin(), pRecord->memberInfo.end(), EqualPayMemberID(iID));
	if (it != pRecord->memberInfo.end())
	{
		if (it->expiretime() > INT32(GameTime::GetTime()))
		{
			it->set_expiretime(iExpire);
			pRecord->m_isModified.Set();
			ChangePayMember(pRole, iID);
			PayV2Mgr::Instance()->NotifyPayAllInfoToClient(pRole, KKSG::PAY_PARAM_NONE);
			return true;
		}
	}
	return false;
}

bool PayV2Mgr::SetPayMemberDragonFlowerTime(Role* pRole, INT32 iID, INT32 iTime)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	auto it = std::find_if(pRecord->memberInfo.begin(), pRecord->memberInfo.end(), EqualPayMemberID(iID));
	if (it != pRecord->memberInfo.end())
	{
		if (it->expiretime() > INT32(GameTime::GetTime()))
		{
			it->set_lastdragonflowertime(iTime);
			pRecord->m_isModified.Set();
			return true;
		}
	}
	return false;
}
void PayV2Mgr::SendDailyAwardMail(Role* pRole, INT32 iID, INT32 iTime, std::vector<ItemDesc>& items)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(iID);
	std::string mailtitle;
	std::string mailcontent;
	if (conf)
	{
		mailtitle = conf->m_title;
		mailcontent = MailConfig::Instance()->ReplaceString("s", TimeUtil::GetDateTimeStr(iTime).substr(0,10), conf->m_content);
	}
	MailSender sender;
	sender.SetReason(ItemFlow_PayMember);
	sender.SetContent(Mail_System, mailtitle,  mailcontent);
	sender.SetItems(items);
	sender.Send(pRole->GetID());
}
bool PayV2Mgr::SendMemberDailyAward(Role* pRole, PayMemberTable::RowData* pRow, INT32 iTime)
{	
	if (NULL == pRole || NULL == pRow)
	{
		return false;
	}
	bool isGet = false;
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	std::vector<ItemDesc> items;
	if (pRow->DragonCoin)
	{
		ItemDesc pdesc(DRAGON_COIN, pRow->DragonCoin);
		items.push_back(pdesc);
	}
	if (pRow->Flower[0] && pRow->Flower[1])
	{
		ItemDesc pdesc(pRow->Flower[0], pRow->Flower[1]);
		items.push_back(pdesc);
	}
	std::string mailtitle;
	std::string mailcontent;
	const MailConf* conf = NULL;
	if (items.size())
	{
		conf = MailConfig::Instance()->GetMailConf(MailConf_PayMemDragonFlower);
		if (conf)
		{
			mailtitle = conf->m_title;
			mailcontent = MailConfig::Instance()->ReplaceString("s", TimeUtil::GetDateTimeStr(iTime).substr(0,10), conf->m_content);
			mailcontent = MailConfig::Instance()->ReplaceString("x", ToString(pRow->DragonCoin), mailcontent);
			mailcontent = MailConfig::Instance()->ReplaceString("y", ToString(pRow->Flower[1]), mailcontent);
		}
		MailSender sender;
		sender.SetReason(ItemFlow_PayMember);
		sender.SetContent(Mail_System, mailtitle,  mailcontent);
		sender.SetItems(items);
		sender.Send(pRole->GetID());
		isGet =  true;
	}
	items.clear();
	if (pRow->PetFeedAdd[0] && pRow->PetFeedAdd[1])
	{
		ItemDesc pdesc(pRow->PetFeedAdd[0], pRow->PetFeedAdd[1]);
		items.push_back(pdesc);
		SendDailyAwardMail(pRole, MailConf_PayMemberPetAward, iTime, items);
		isGet =  true;
	}

	items.clear();
	if (pRow->SpriteFeedAdd[0] && pRow->SpriteFeedAdd[1])
	{
		ItemDesc pdesc(pRow->SpriteFeedAdd[0], pRow->SpriteFeedAdd[1]);
		items.push_back(pdesc);
		SendDailyAwardMail(pRole, MailConf_PayMemberSpriteAward, iTime, items);
		isGet =  true;
	}
	return isGet;
}
VIPTable::RowData * PayV2Mgr::GetVipConf(int vipLevel)
{
	for (UINT32 i = 0; i < m_oVipTable.Table.size(); ++i)
	{
		if (m_oVipTable.Table[i]->VIP == vipLevel)
		{
			return m_oVipTable.Table[i];
		}
	}

	return NULL;
}

UINT32 PayV2Mgr::GetVipLevel(UINT32 rmb)
{
	if (rmb < m_oVipTable.Table[0]->RMB)
		return 0;

	for (UINT32 i = 1; i < m_oVipTable.Table.size(); ++i)
	{
		if (rmb < m_oVipTable.Table[i]->RMB)
		{
			return m_oVipTable.Table[i-1]->VIP;
		}
	}

	return m_oVipTable.Table.back()->VIP;
}
void PayV2Mgr::NotifyPayAllInfoToClient(Role* pRole, INT32 nType)
{
	PtcG2C_PayAllInfoNtf oPtc;
	KKSG::PayAllInfo roInfo;
	GetPayInfo(pRole, &roInfo);
	oPtc.m_Data.CopyFrom(roInfo);
	oPtc.m_Data.set_paytype(nType);
	pRole->Send(oPtc);
}

void PayV2Mgr::CalcVipLevel(Role* pRole, UINT32 value, INT32 nType)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	INT32 paynum = 0;
	if (nType == KKSG::PAY_PARAM_LIST)
	{
		if (pRecord->m_VipPoint >= value)
		{
			return;
		}
		paynum = value - pRecord->m_VipPoint;
		pRecord->m_VipPoint = value;
		DoTxLog(pRole, paynum, "");
	}
	else
	{
		paynum = value;
		pRecord->m_TotalPay += value;
	}
	UINT32 uLevel = GetVipLevel(pRecord->m_TotalPay/100 + pRecord->m_VipPoint/100);
	if (pRecord->m_VipLevel != uLevel)
	{	
		pRecord->m_VipLevel = uLevel;
		VipGiftHintNotify(pRole);
		pRole->OnChangeVip();
	}
	pRecord->m_isModified.Set();
	pRole->OnPay(pRecord->m_TotalPay+pRecord->m_VipPoint, paynum);
	if (!pRecord->GetIsEverPay())
	{
		pRole->OnFirstPay();
		pRecord->SetIsEverPay();
	}
	NotifyPayAllInfoToClient(pRole, nType);
	PayFirstAwardHintNotify(pRole);

	LogInfo("roleid=%llu pay success,paynum=%u,type=%d", pRole->GetID(), paynum, nType);
}

void PayV2Mgr::SetPayCard(Role* pRole, INT32 nType, INT32 uTime)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	if (nType == KKSG::WEEK_CARD)
	{
		pRecord->payWeekCard.m_iEndTime = uTime;
		pRecord->payWeekCard.m_iLastGetAwardTime = 0;
	}
	else
	{
		pRecord->payMonthCard.m_iEndTime = uTime;
		pRecord->payMonthCard.m_iLastGetAwardTime = 0;
	}
	pRecord->m_isModified.Set();
}

void PayV2Mgr::VipGiftHintNotify(Role* pRole)
{
	return ;//vip功能暂时屏蔽;
	//还有未领取的vip礼包;
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	UINT32 nCount = 0;
	for (auto i = m_oVipTable.Table.begin(); i != m_oVipTable.Table.end(); i ++)
	{
		if (pRecord->GetVipLevel() >= (*i)->VIP)
		{
			nCount ++;
		}
	}
	if (nCount > pRecord->vipLevelGiftInfo.size())
	{
		pRole->HintNotify(XSys_Pay, false);
		return ;
	}
}
void PayV2Mgr::PayCardHintNotify(Role* pRole)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	/*	//2016-11-18 策划要求去掉;
	if (pRecord->payCardButtonStatus == KKSG::PAY_BUTTON_OPEN)
	{
		pRole->HintNotify(Welfare_DiamondCard, false);
		return;
	}
	*/
	if (pRecord->payWeekCard.m_iEndTime > (INT32)GameTime::GetTime())//周卡
	{
		if (!XCommon::IsDaySame(GameTime::GetTime(), pRecord->payWeekCard.m_iLastGetAwardTime))
		{
			pRole->HintNotify(Welfare_DiamondCard, false);
			return;
		}
	}
	if (pRecord->payMonthCard.m_iEndTime > (INT32)GameTime::GetTime())
	{

		if (!XCommon::IsDaySame(GameTime::GetTime(), pRecord->payMonthCard.m_iLastGetAwardTime))
		{
			pRole->HintNotify(Welfare_DiamondCard, false);
			return;
		}
	}
}
void PayV2Mgr::GrowthFundHintNotify(Role* pRole)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	/* //2016-11-18 策划要求去掉;
	if (pRecord->m_uGrowthFundButtonStatus == KKSG::PAY_BUTTON_OPEN)
	{
		pRole->HintNotify(CPayConfig::Instance()->GetGrowthFundSystemID(), false);
		return ;
	}
	*/
	RechargeTable::RowData* pGrowthFundData = CPayConfig::Instance()->GetGrowthFundConf();
	if (pRecord->payGrowthFund.m_iEndTime > INT32(GameTime::GetTime()) && NULL != pGrowthFundData)
	{
		UINT32 nCount = 0;
		for (UINT32 i = 0; i < pGrowthFundData->RoleLevels.size(); i ++)
		{
			if (pGrowthFundData->RoleLevels[i][0] <= pRole->GetLevel())
			{
				nCount ++;
			}
		}
		if (nCount > pRecord->fundLevelInfo.size())
		{
			pRole->HintNotify(CPayConfig::Instance()->GetGrowthFundSystemID(), false);
			return ;
		}
		nCount = 0;
		for (UINT32 i = 0; i < pGrowthFundData->LoginDays.size(); i ++)
		{
			if (pGrowthFundData->LoginDays[i][0] <= INT32(pRole->Get<MyLoginRecord>()->GetRealLoginDayCount()))
			{
				nCount ++;
			}
		}
		if (nCount > pRecord->fundLoginInfo.size())
		{
			pRole->HintNotify(CPayConfig::Instance()->GetGrowthFundSystemID(), false);
			return ;
		}
	}
}
void PayV2Mgr::PayFirstAwardHintNotify(Role* pRole)
{
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	/* //2016-11-18 策划要求去掉;
	if (pRecord->m_uPayFirstAwardButtonStatus == KKSG::PAY_BUTTON_OPEN)
	{
		pRole->HintNotify(CPayConfig::Instance()->GetPayFirstSystemID(), false);
		return ;
	}
	*/
	PayFirst::RowData* pPayFirstData = CPayConfig::Instance()->GetPayFirstConf();
	if (pPayFirstData != NULL && pPayFirstData->Money <= (INT32)pRecord->m_TotalPay && !pRecord->m_uLastFirstPayAwardTime)
	{
		pRole->HintNotify(CPayConfig::Instance()->GetPayFirstSystemID(), false);
		return ;
	}
}
void PayV2Mgr::AileenHintNotify(Role* pRole)
{
	return ;//2016-11-18 策划要求去掉;
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	if (pRecord->payAileenButtonStatus == KKSG::PAY_BUTTON_OPEN && CPayConfig::Instance()->IsTodayAlieen())
	{
		pRole->HintNotify(Welfare_GiftBag, false);
	}
}
void PayV2Mgr::HintNotify(Role* pRole)
{
	AileenHintNotify(pRole);//艾琳礼包红点;
	PayCardHintNotify(pRole);//周月卡;
	GrowthFundHintNotify(pRole);//成长基金;
	VipGiftHintNotify(pRole);	//vip礼包红点;
	PayFirstAwardHintNotify(pRole);//首充奖励红点;
}

UINT32 PayV2Mgr::GetGoldClickCount(int vip)
{
	VIPTable::RowData* data = GetVipConf(vip);
	if (NULL != data)
	{
		return data->GoldClickTimes;
	}
	return 0;
}

void PayV2Mgr::PayCheck(Role* pRole)
{
	HintNotify(pRole);	//充值相关小红点通知
	NotifyPayAllInfoToClient(pRole, KKSG::PAY_PARAM_NONE);
	CheckMemberDragonFlowerAward(pRole);
	pRole->Get<CPayV2Record>()->NotifyMemberPrivilege();
}

bool PayV2Mgr::IsNeedOpenPaySystemID(Role* pRole, INT32 nSystemID)
{
	//领完奖，关闭系统;
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	if (nSystemID == CPayConfig::Instance()->GetPayFirstSystemID() && pRecord->m_uLastFirstPayAwardTime)
	{
		pRole->Get<CRoleSystem>()->SetSystemOpened(CPayConfig::Instance()->GetPayFirstSystemID(), false);
		return true;
	}
	RechargeTable::RowData* pRowData = CPayConfig::Instance()->GetGrowthFundConf();
	if (pRowData)
	{
		if (nSystemID ==  CPayConfig::Instance()->GetGrowthFundSystemID())
		{
			if (!pRecord->IsBuyGrowFund() && !GetGrowthFundLeftTime())//没有购买且已关闭;
			{
				pRole->Get<CRoleSystem>()->SetSystemOpened(CPayConfig::Instance()->GetGrowthFundSystemID(), false);
				return true;
			}
			if (pRowData->RoleLevels.size() == pRecord->fundLevelInfo.size() && pRowData->LoginDays.size() == pRecord->fundLoginInfo.size())//购买了，领取完奖励了
			{
				pRole->Get<CRoleSystem>()->SetSystemOpened(CPayConfig::Instance()->GetGrowthFundSystemID(), false);
				return true;
			}
		}
	}
	return false;
}

INT32 PayV2Mgr::GetRemainedDays(INT32 iTime)
{
	INT32 iTodayBegin = INT32(XCommon::GetTodayBeginTime());
	INT32 iBegin = INT32(XCommon::GetDayBeginTime(iTime));
	return INT32(abs((iBegin-iTodayBegin)/XCommon::GetOneDayTime()));
}

void PayV2Mgr::NotifyDiamond(Role* pRole, INT32 iCount)
{
	INT32 iDiamond = (INT32)pRole->Get<Bag>()->CountVirtualItem(DIAMOND);
	if (iCount >= 0 && iDiamond != iCount)
	{
		pRole->Get<Bag>()->SetVirtualItem(DIAMOND, iCount, false, NULL);
		pRole->OnChangeVirtualItem(DIAMOND, INT64(iCount-iDiamond));
		PtcG2C_ItemChangedNtf oItemChangePtc;
		oItemChangePtc.m_Data.add_virtualitemid(DIAMOND);
		oItemChangePtc.m_Data.add_virtualitemcount(iCount);
		pRole->Send(oItemChangePtc);
	}
}

IConsumeListener* PayV2Mgr::GetConsumeLister(INT32 type)
{
	auto it = m_oConsumeLister.find(type);
	if (it != m_oConsumeLister.end())
	{
		return it->second;
	}
	return NULL;
}

void PayV2Mgr::DoTxLog(Role* pRole, INT32 iAmount, std::string ParamId)
{
	if (NULL == pRole)
	{
		return;
	}
	TPayFlow oLog(pRole);
	oLog.m_iAmount = iAmount;
	oLog.m_szParamID = ParamId;
	oLog.Do();
}

void PayV2Mgr::SetPayAileenStatus(Role* pRole, std::string paramid)
{
	if (NULL == pRole)
	{
		return;
	}
	PayAileenTable::RowData* rowdata = CPayConfig::Instance()->GetTodayPayAileenTable(paramid);
	if (NULL == rowdata)
	{
		return;
	}
	INT32 itemid = CPayConfig::Instance()->GetAileenItemID(paramid);
	if (!itemid)
	{
		return;
	}
	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	auto i = pRecord->aileenInfo.find(paramid);
	if (i != pRecord->aileenInfo.end())
	{
		if (XCommon::IsDaySame(i->second.lastbuytime(), GameTime::GetTime()))
		{
			return;
		}
		i->second.set_itemid(itemid);
		i->second.set_lastbuytime(INT32(GameTime::GetTime()));
	}
	else
	{
		KKSG::PayAileenRecord roAileen;
		roAileen.set_paramid(rowdata->ParamID);
		roAileen.set_itemid(itemid);
		roAileen.set_lastbuytime(INT32(GameTime::GetTime()));
		pRecord->aileenInfo.insert(std::make_pair(rowdata->ParamID, roAileen));
	}
	NotifyPayAllInfoToClient(pRole, KKSG::PAY_PARAM_NONE);
	pRecord->m_isModified.Set();
}

time_t PayV2Mgr::GetGrowthFundCloseTime()
{
	return (GSConfig::Instance()->GetGameServerOpenTimeStamp() + GetGlobalConfig().GrowthFundBuyDays * XCommon::GetOneDayTime());
}

UINT32 PayV2Mgr::GetGrowthFundLeftTime()
{
	INT32 lefttime =  (INT32)GetGrowthFundCloseTime() - (INT32)GameTime::GetTime();
	return lefttime>0?lefttime:0;
}

void PayV2Mgr::CloseGrowthFund(Role* pRole, bool isopen)
{
	INT32 id = CPayConfig::Instance()->GetGrowthFundSystemID();
	pRole->Get<CRoleSystem>()->SetSystemOpened(id, isopen);
	std::vector<UINT32> openids;
	std::vector<UINT32> closeids;
	if (!isopen)
	{
		closeids.push_back(id);
	}
	else
	{
		openids.push_back(id);
	}
	OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, &openids, &closeids);
}
