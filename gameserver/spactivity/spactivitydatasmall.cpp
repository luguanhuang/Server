#include "pch.h"
#include "spactivitydatasmall.h"
#include "spactivitydatabase.h"
#include "unit/role.h"
#include "unit/systeminfo.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/bag.h"
#include "util/gametime.h"
#include "spactivitysmall.h"
#include "spactivitymgr.h"
#include "spactivity/ptcg2c_themeactivitychangentf.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/midas.h"

SpActOpenServerData::SpActOpenServerData(Role* pRole, UINT32 actid):SpActivityDataBase(pRole, actid)
{
	assert(actid == SpActivity_OpenServer);

	m_getBigPrize = false;
}

SpActOpenServerData::~SpActOpenServerData()
{

}

void SpActOpenServerData::Load(const KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Load(spActOneData);

	m_getBigPrize = spActOneData.getbigprize();
}

void SpActOpenServerData::Save(KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Save(spActOneData);

	spActOneData.set_getbigprize(m_getBigPrize);
}

KKSG::ErrorCode SpActOpenServerData::GetBigPrize()
{
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		LogWarn("can't find SpActivityTimeConf, actid:%u", m_actid);
		return KKSG::ERR_UNKNOWN;
	}

	///> system open
	if (!m_pRole->Get<CRoleSystem>()->IsSystemOpened(conf->systemid))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	if (0 == conf->pointid) //no big prize
	{
		return KKSG::ERR_SPACTIVITY_NOPRIZE;
	}
	if (m_getBigPrize)
	{
		return KKSG::ERR_SPACTIVITY_PRIZE_GET;
	}

	BagTakeItemTransition takeTransition(m_pRole);
	takeTransition.SetReason(ItemFlow_SpActivity, ItemFlow_SpActivity_BigPrize);

	UINT32 pointNum = (UINT32)m_pRole->Get<Bag>()->CountVirtualItem(conf->pointid);
	if (pointNum < conf->needpoint)
	{
		if (GetActStage() != 2)
		{
			return KKSG::ERR_SPACTIVITY_NOTPRIZETIME;
		}
		int needDragon = (int)(1.0 * (conf->needpoint - pointNum) * conf->rate + 0.5);	
		if (!takeTransition.TakeItem(DRAGON_COIN, needDragon))
		{
			takeTransition.RollBack();
			return KKSG::ERR_SPACTIVITY_NOTENOUGH_MONEY;
		}
		takeTransition.TakeItem(conf->pointid, pointNum);
	}
	else
	{
		takeTransition.TakeItem(conf->pointid, conf->needpoint);
	}
	takeTransition.NotifyClient();

	m_getBigPrize = true; //already get

	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_SpActivity, ItemFlow_SpActivity_BigPrize);
	for (auto i = conf->bigprize.begin(); i != conf->bigprize.end(); ++i)
	{
		transition.GiveItem((*i)[0], (*i)[1]);
	}
	transition.NotifyClient();

	LogInfo("role [%llu] get big prize, actid:%u", m_pRole->GetID(), m_actid);
	
	SetModify();

	return KKSG::ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActBackFlowData::SpActBackFlowData(Role* pRole, UINT32 actid):SpActivityDataBase(pRole, actid)
{
	assert(actid == SpActivity_BackFlow);

	m_worldLevel = 0;
	m_lostDay = 1;
	m_point = 0;
	m_shopUpdateTime = 0;
	m_freshCount = 0;
	m_isFinishBackFlowScene = false;
	m_lastSmallDragonFinishTime = 0;
	m_lastNestFinishTime = 0;
	m_nestFinishCount = 0;
}

SpActBackFlowData::~SpActBackFlowData()
{

}

void SpActBackFlowData::Load(const KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Load(spActOneData);

	const KKSG::BackFlowData& data = spActOneData.backflow();
	m_worldLevel = data.worldlevel();
	m_point = data.point();
	for (int i = 0; i < data.alreadyget_size(); ++i)
	{
		if (data.alreadyget(i) < GetGlobalConfig().BackFlowTreasure.size())
		{
			m_alreadyGet.push_back(data.alreadyget(i));
		}
	}
	if (data.paygiftcount_size() == data.paygifttype_size())
	{
		for (int i = 0; i < data.paygiftcount_size(); ++ i)
		{
			m_PayGift[data.paygifttype(i)] = data.paygiftcount(i);
		}
	}

	m_lostDay = data.lostday();
	const KKSG::BackFlowShopData& shopData = data.shop();
	m_shopUpdateTime = shopData.lastupdatetime();
	for (int i = 0; i < shopData.goods_size(); ++i)
	{
		m_shopGoods.insert(std::make_pair(shopData.goods(i).goodid(), SpActBackFlowShopGood(shopData.goods(i).isbuy(), shopData.goods(i).locktime())));
	}
	m_freshCount = shopData.freshcount();

	m_isFinishBackFlowScene = data.isfinishbackflowscene();

	m_lastSmallDragonFinishTime = data.lastsmalldragonfinishtime();
	m_lastNestFinishTime = data.lastnestfinishtime();
	m_nestFinishCount = data.nestfinishcount();
}

void SpActBackFlowData::Save(KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Save(spActOneData);

	KKSG::BackFlowData* data = spActOneData.mutable_backflow();
	data->set_worldlevel(m_worldLevel);
	data->set_point(m_point);
	for (auto i = m_alreadyGet.begin(); i != m_alreadyGet.end(); ++i)
	{
		data->add_alreadyget(*i);
	}

	for (auto i = m_PayGift.begin(); i != m_PayGift.end(); ++ i)
	{
		data->add_paygifttype(i->first);
		data->add_paygiftcount(i->second);
	}

	data->set_lostday(m_lostDay);
	FillShopData(*data->mutable_shop());
	data->set_isfinishbackflowscene(m_isFinishBackFlowScene);

	data->set_lastsmalldragonfinishtime(m_lastSmallDragonFinishTime);
	data->set_lastnestfinishtime(m_lastNestFinishTime);
	data->set_nestfinishcount(m_nestFinishCount);
}

void SpActBackFlowData::UpdatePerMinute()
{
	GetShopLeftTime();
}

void SpActBackFlowData::OnSpActivityOpen()
{
	SpActivityBase::OpenSpActivitySystem(m_pRole, SYS_BACKFLOW_LEVELUP);
	SpActivityBase::OpenSpActivitySystem(m_pRole, SYS_BACKFLOW_MALL);
}

void SpActBackFlowData::OnSpActivityClose()
{
	SpActivityBase::CloseSpActivitySystem(m_pRole, SYS_BACKFLOW_LEVELUP);
	SpActivityBase::CloseSpActivitySystem(m_pRole, SYS_BACKFLOW_MALL);
}

void SpActBackFlowData::IncreaseProgress(const SpActivityTaskConf* conf, UINT32 count)
{
	SpActivityDataBase::IncreaseProgress(conf, count);

	if (count == 0)
	{
		return;
	}
	if (conf == NULL || conf->actid != m_actid)
	{
		return;
	}
	auto iter = m_mapTask.find(conf->taskid);
	if (iter == m_mapTask.end())
	{
		return;
	}

	const BackflowActivityConf* backConf = SpActivityConfig::Instance()->GetBackflowActivityConf(conf->taskid);
	if (backConf == NULL)
	{
		return;
	}
	if (backConf->Type == 1)
	{
		UINT32 nowTime = GameTime::GetTime();
		if (conf->basetask == SpActTask_CompleteSmallDragonNest)
		{
			if (XCommon::IsDaySame(nowTime, m_lastSmallDragonFinishTime))
			{
				return;
			}
			m_lastSmallDragonFinishTime = nowTime;
		}
		else if (conf->basetask == SpActTask_CompleteSceneType)
		{
			if (!XCommon::IsDaySame(nowTime, m_lastNestFinishTime))
			{
				m_lastNestFinishTime = nowTime;
				m_nestFinishCount = 0;
			}
			if (m_nestFinishCount >= GetGlobalConfig().BackFlowNestFinishCountLimit)
			{
				return;
			}
			++m_nestFinishCount;
		}
		m_point += backConf->Point;
		LogInfo("role [%llu] increaseProgress taskid [%u], add backflow point [%u], backflow point [%u]", m_pRole->GetID(), conf->taskid, backConf->Point, m_point);
		
		SetModify();

		const std::vector<Sequence<UINT32, 3>>& vecTreasure = GetGlobalConfig().BackFlowTreasure;
		for (UINT32 i = 0; i < vecTreasure.size(); ++i)
		{
			if (m_point >= vecTreasure[i][0] && std::find(m_alreadyGet.begin(), m_alreadyGet.end(), i) == m_alreadyGet.end())
			{
				m_pRole->HintNotify(SYS_BACKFLOW_TARGET, false);
				break;
			}
		}
	}
}

void SpActBackFlowData::SetInitData(UINT32 worldLevel, UINT32 lostDay)
{
	m_worldLevel = worldLevel;
	m_lostDay = lostDay;

	m_shopUpdateTime = GameTime::GetTime();
	m_freshCount = GetGlobalConfig().BackFlowShopFreshCount;
	m_isFinishBackFlowScene = false;
	m_lastSmallDragonFinishTime = 0;
	m_lastNestFinishTime = 0;
	m_nestFinishCount = 0;

	DoFreshShop();
	
	SetModify();
}

KKSG::ErrorCode SpActBackFlowData::GetTreasure(UINT32 pos)
{
	const std::vector<Sequence<UINT32, 3>>& vecTreasure = GetGlobalConfig().BackFlowTreasure;
	if (pos >= vecTreasure.size())
	{
		return KKSG::ERR_FAILED;
	}
	if (std::find(m_alreadyGet.begin(), m_alreadyGet.end(), pos) != m_alreadyGet.end())
	{
		return KKSG::ERR_BACKFLOW_ALREADYGET;
	}
	if (vecTreasure[pos][0] > m_point)
	{
		return KKSG::ERR_BACKFLOW_LESSPOINT;
	}
	BagGiveItemTransition give(m_pRole);
	give.SetReason(ItemFlow_SpActivity, ItemFlow_BackFlow_GetTreasure);
	give.GiveItem(vecTreasure[pos][1], vecTreasure[pos][2]);
	give.NotifyClient();

	m_alreadyGet.push_back(pos);

	SetModify();

	return KKSG::ERR_SUCCESS;
}

void SpActBackFlowData::FillTreasureData(KKSG::BackFlowActivityOperationRes& res)
{
	res.set_errorcode(KKSG::ERR_SUCCESS);
	res.set_point(m_point);
	for (auto i = m_alreadyGet.begin(); i != m_alreadyGet.end(); ++i)
	{
		res.add_alreadyget(*i);
	}
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	if (!XCommon::IsDaySame(nowTime, m_lastSmallDragonFinishTime))
	{
		res.set_leftsmalldragoncount(1);
	}
	else
	{
		res.set_leftsmalldragoncount(0);
	}
	UINT32 nestFinishCountLimit = GetGlobalConfig().BackFlowNestFinishCountLimit;
	if (!XCommon::IsDaySame(nowTime, m_lastNestFinishTime))
	{
		res.set_leftnestcount(nestFinishCountLimit);
	}
	else
	{
		res.set_leftnestcount(nestFinishCountLimit < m_nestFinishCount ? 0 : nestFinishCountLimit - m_nestFinishCount);
	}
}

UINT32 SpActBackFlowData::GetPayGiftCount(const std::string& paramid)
{
	auto it = m_PayGift.find(paramid);
	if (it != m_PayGift.end())
	{
		return it->second;
	}
	return 0;
}

void SpActBackFlowData::UpdatePayGiftCount(const std::string& paramid)
{
	auto it = m_PayGift.find(paramid);
	if (it != m_PayGift.end())
	{
		it->second ++;
	}
	else
	{
		m_PayGift[paramid] = 1;
	}
	SetModify();
}

UINT32 SpActBackFlowData::GetShopLeftTime()
{
	UINT32 endTime = m_pRole->Get<SpActivityRecord>()->GetLastBackFlowStartTime() + GetGlobalConfig().BackFlowShopDuration * 24 * 3600;
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	if (endTime > nowTime)
	{
		return endTime - nowTime;
	}
	SpActivityBase::CloseSpActivitySystem(m_pRole, SYS_BACKFLOW_MALL);
	return 0;
}

void SpActBackFlowData::FillShopData(KKSG::BackFlowShopData& shopData)
{
	shopData.set_lastupdatetime(m_shopUpdateTime);
	for (auto i = m_shopGoods.begin(); i != m_shopGoods.end(); ++i)
	{
		KKSG::BackFlowShopGood* good = shopData.add_goods();
		good->set_goodid(i->first);
		good->set_isbuy(i->second.isBuy);
		good->set_locktime(i->second.lockTime);
	}
	UpdateShopFreshCount();
	shopData.set_freshcount(m_freshCount);
}

void SpActBackFlowData::UpdateShopFreshCount()
{
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	if (!XCommon::IsDaySame(nowTime, m_shopUpdateTime))
	{
		m_shopUpdateTime = nowTime;
		m_freshCount = GetGlobalConfig().BackFlowShopFreshCount;

		SetModify();
	}
}

KKSG::ErrorCode SpActBackFlowData::FreshShop()
{
	UpdateShopFreshCount();

	if (m_freshCount <= 0)
	{
		return KKSG::ERR_BACKFLOWSHOP_LESSFRESHCOUNT;
	}

	--m_freshCount;

	DoFreshShop();

	SetModify();

	return KKSG::ERR_SUCCESS;
}

void SpActBackFlowData::DoFreshShop()
{
	SpActivityBackFlow* backflow = static_cast<SpActivityBackFlow*>(SpActivityMgr::Instance()->GetSpActivity(SpActivity_BackFlow));
	if (backflow == NULL)
	{
		return;
	}
	std::vector<UINT32> vecGoods;
	backflow->GetNewShopGoodsList(m_pRole, m_lostDay, vecGoods);
	m_shopGoods.clear();
	for (auto i = vecGoods.begin(); i != vecGoods.end(); ++i)
	{
		m_shopGoods.insert(std::make_pair(*i, SpActBackFlowShopGood()));
	}

	SetModify();
}

KKSG::ErrorCode SpActBackFlowData::BuyGoodInShop(UINT32 goodid, UINT32 delayid, bool& needDelay)
{
	needDelay = false;

	auto iter = m_shopGoods.find(goodid);
	if (iter == m_shopGoods.end())
	{
		return KKSG::ERR_BACKFLOWSHOP_NOTHAVEGOOD;
	}
	if (iter->second.isBuy)
	{
		return KKSG::ERR_BACKFLOWSHOP_ALREADYBUY;
	}

	SpActivityBackFlow* backflow = static_cast<SpActivityBackFlow*>(SpActivityMgr::Instance()->GetSpActivity(SpActivity_BackFlow));
	if (backflow == NULL)
	{
		LogError("In Hall GS, but no SpActivityBackFlow, strange error");
		return KKSG::ERR_UNKNOWN;
	}
	const BackflowShopConf* conf = backflow->GetBackflowShopConf(goodid);
	if (conf == NULL)
	{
		LogWarn("role [%llu], goodid [%u] not in BackFlowShop.txt", m_pRole->GetID(), goodid);
		return KKSG::ERR_UNKNOWN;
	}
	if (conf->CostType == DIAMOND)
	{	
		UINT32 nowTime = TimeUtil::GetTime();

		if (iter->second.lockTime > nowTime)
		{
			return KKSG::ERR_FAILED;
		}

		KKSG::PayConsumeBase info;
		info.set_type((int)MidasConsumeType_BackFlowShop);
		info.set_amt(conf->CostNum);
		info.set_subreason(ItemFlow_BackFlowShop);
		info.set_reason(ItemFlow_BackFlowShop_DragonCoin);
		info.set_index(delayid);
		info.set_count(conf->GoodID);
		std::vector<ItemDesc> vecItem;
		vecItem.push_back(ItemDesc(conf->ItemID, conf->ItemCount));
		if (!m_pRole->ConsumeDiamond(info, vecItem))
		{
			return KKSG::ERR_BACKFLOWSHOP_LACKDIAMOND;
		}

		needDelay = true;

		iter->second.lockTime = nowTime + 5;
	}
	else
	{
		BagTakeItemTransition take(m_pRole);
		take.SetReason(ItemFlow_BackFlowShop, ItemFlow_BackFlowShop_DragonCoin);
		if (!take.TakeItem(conf->CostType, conf->CostNum))
		{
			take.RollBack();
			return KKSG::ERR_BACKFLOWSHOP_LACKDRAGONCOIN;
		}
		take.NotifyClient();

		BagGiveItemTransition give(m_pRole);
		give.SetReason(ItemFlow_BackFlowShop, ItemFlow_BackFlowShop_Give);
		give.GiveItem(conf->ItemID, conf->ItemCount);
		give.NotifyClient();

		iter->second.isBuy = true;
	}

	SetModify();

	return KKSG::ERR_SUCCESS;
}

bool SpActBackFlowData::SetGoodBuySucceed(UINT32 goodid)
{
	auto iter = m_shopGoods.find(goodid);
	if (iter == m_shopGoods.end())
	{
		return false;
	}
	iter->second.isBuy = true;
	SetModify();
	return true;
}

void SpActBackFlowData::SetIsFinishBackFlowScene()
{
	m_isFinishBackFlowScene = true;
	SetModify();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActArgentaData::SpActArgentaData(Role* pRole, UINT32 actid):SpActivityDataBase(pRole, actid)
{
	assert(actid == SpActivity_Argenta);

	m_argentaStartTime = 0;
	m_lastUpdateTime = 0;
	m_level = 0;
}

SpActArgentaData::~SpActArgentaData()
{

}

void SpActArgentaData::Load(const KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Load(spActOneData);

	const KKSG::ArgentaData& argentData = spActOneData.argenta();

	m_argentaStartTime = argentData.argentastarttime();
	m_level = argentData.level();
	m_lastUpdateTime = argentData.lastupdatetime();
	for (int i = 0; i < argentData.getdailyrewards_size(); ++i)
	{
		m_getDailyRewards.push_back(argentData.getdailyrewards(i));
	}
}

void SpActArgentaData::Save(KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Save(spActOneData);

	KKSG::ArgentaData* argentData = spActOneData.mutable_argenta();
	argentData->set_argentastarttime(m_argentaStartTime);
	argentData->set_level(m_level);
	argentData->set_lastupdatetime(m_lastUpdateTime);
	for (auto i = m_getDailyRewards.begin(); i != m_getDailyRewards.end(); ++i)
	{
		argentData->add_getdailyrewards(*i);
	}
}

void SpActArgentaData::UpdatePerMinute()
{
	time_t nowTime = GameTime::GetTime();
	if (!XCommon::IsDaySame(nowTime, m_lastUpdateTime))
	{
		m_lastUpdateTime = nowTime;
		m_getDailyRewards.clear();
		SetModify();

		const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(SpActivity_Argenta);
		if (conf != NULL)
		{
			m_pRole->HintNotify(conf->systemid, false);
		}
	}
}

void SpActArgentaData::OnFirstEnterScene()
{
	SpActivityArgenta* pArgenta = static_cast<SpActivityArgenta*>(SpActivityMgr::Instance()->GetSpActivity(SpActivity_Argenta));
	if (pArgenta == NULL)
	{
		return;
	}
	bool isGetAll = true;
	for (auto i = pArgenta->m_oDailyTable.Table.begin(); i != pArgenta->m_oDailyTable.Table.end(); ++i)
	{
		bool isFind = false;
		for (auto j = m_getDailyRewards.begin(); j != m_getDailyRewards.end(); ++j)
		{
			if ((*i)->ID == *j)
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			isGetAll = false;
			break;
		}
	}
	if (!isGetAll)
	{
		const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(SpActivity_Argenta);
		if (conf != NULL)
		{
			m_pRole->HintNotify(conf->systemid, false);
		}
	}
}

void SpActArgentaData::SetInitialData()
{
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	m_argentaStartTime = nowTime;
	m_level = m_pRole->GetLevel();
	m_lastUpdateTime = nowTime;

	SetModify();
}

KKSG::ErrorCode SpActArgentaData::GetDailyReward(UINT32 id)
{
	SpActivityArgenta* pArgenta = static_cast<SpActivityArgenta*>(SpActivityMgr::Instance()->GetSpActivity(SpActivity_Argenta));
	if (pArgenta == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	ArgentaDaily::RowData* pDailyConf = pArgenta->m_oDailyTable.GetByID(id);
	if (pDailyConf == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	for (auto i = m_getDailyRewards.begin(); i != m_getDailyRewards.end(); i++)
	{
		if (*i == id)
		{
			return KKSG::ERR_ARGENTA_DAILY_GET;
		}
	}
	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_SpActivity, ItemFlow_SpActivityArgenta_GetDaily);
	for (auto i = pDailyConf->Reward.begin(); i != pDailyConf->Reward.end(); ++i)
	{
		transition.GiveItem((*i)[0], (*i)[1]);
	}
	transition.NotifyClient();

	m_getDailyRewards.push_back(id);

	SetModify();
	return KKSG::ERR_SUCCESS;
}

void SpActArgentaData::FillInfo(KKSG::ArgentaActivityRes& res)
{
	res.set_errorcode(KKSG::ERR_SUCCESS);
	UINT32 endTime = m_argentaStartTime + GetGlobalConfig().ArgentaLastTime * 3600;
	UINT32 nowTime = GameTime::GetTime();
	if (endTime >= nowTime) res.set_lefttime(endTime - nowTime);
	else res.set_lefttime(0);
	res.set_level(m_level);
	for (auto i = m_getDailyRewards.begin(); i != m_getDailyRewards.end(); ++i)
	{
		res.add_getrewardids(*i);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActAncientData::SpActAncientData(Role* pRole, UINT32 actid):SpActivityDataBase(pRole, actid)
{
	assert(actid == SpActivity_AncientTimes);

	m_nAward = 0;
}

SpActAncientData::~SpActAncientData()
{

}

void SpActAncientData::Load(const KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Load(spActOneData);
	const KKSG::AncientTimes& ancientData = spActOneData.ancient();
	m_nAward = ancientData.award();
}

void SpActAncientData::Save(KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Save(spActOneData);
	KKSG::AncientTimes* ancientData = spActOneData.mutable_ancient();
	ancientData->set_award(m_nAward);
}

void SpActAncientData::OnSpActivityOpen()
{
	SpActivityAncient* pAncient = static_cast<SpActivityAncient*>(SpActivityMgr::Instance()->GetSpActivity(SpActivity_AncientTimes));
	if (pAncient == NULL)
	{
		LogError("Role [%llu] SpActAncientData::OnSpActivityOpen, but SpActivityAncient is NULL", m_pRole->GetID());
		return;
	}
	pAncient->OnActivityOpen(m_pRole);
}

UINT32 SpActAncientData::GetAward()
{
	return m_nAward;
}

void SpActAncientData::SetAward(UINT32 nAward)
{
	m_nAward = nAward;
	SetModify();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActThemeData::SpActThemeData(Role* pRole, UINT32 actid):SpActivityDataBase(pRole, actid)
{
	assert(actid == SpActivity_BioHell || actid == SpActivity_Duck);

	m_isHint = true;
}

SpActThemeData::~SpActThemeData()
{

}

void SpActThemeData::Load(const KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Load(spActOneData);
	m_mapFirstScene.clear();

	if (spActOneData.has_theme())
	{
		const KKSG::ThemeActivityData& themeData = spActOneData.theme();
		for (int i = 0; i < themeData.firstscene_size(); ++i)
		{
			std::vector<UINT32> vSceneList;
			for (int j = 0; j < themeData.firstscene(i).sceneid_size(); ++j)
			{
				vSceneList.push_back(themeData.firstscene(i).sceneid(j));
			}
			m_mapFirstScene[themeData.firstscene(i).scenetype()] = vSceneList;
		}

		m_isHint = themeData.ishint();
	}
}

void SpActThemeData::Save(KKSG::SpActivityOne& spActOneData)
{
	SpActivityDataBase::Save(spActOneData);

	KKSG::ThemeActivityData* themeData = spActOneData.mutable_theme();
	for (auto i = m_mapFirstScene.begin(); i != m_mapFirstScene.end(); ++i)
	{
		KKSG::SpFirstCompleteScene* pInfo = themeData->add_firstscene();
		pInfo->set_scenetype(i->first);
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			pInfo->add_sceneid(*j);
		}
	}
	themeData->set_ishint(m_isHint);
}

bool SpActThemeData::IsFirstCompleteScene(UINT32 scenetype, UINT32 sceneid)
{
	auto it = m_mapFirstScene.find(scenetype);
	if (it == m_mapFirstScene.end())
	{
		return true;
	}

	for (auto i = it->second.begin(); i != it->second.end(); ++i)
	{
		if (*i == sceneid)
		{
			return false;
		}
	}
	return true;
}

void SpActThemeData::SetFirstCompleteScene(UINT32 scenetype, UINT32 sceneid)
{
	if (!IsFirstCompleteScene(scenetype, sceneid))
	{
		return;
	}
	auto it = m_mapFirstScene.find(scenetype);
	if (it != m_mapFirstScene.end())
	{
		it->second.push_back(sceneid);
	}
	else
	{
		std::vector<UINT32> vList;
		vList.push_back(sceneid);
		m_mapFirstScene[scenetype] = vList;
	}

	NotifyClient();
	SetModify();

	NoticeMgr::Instance()->NoticeBioHellFirstComplete(m_pRole->GetID(), sceneid);
}

void SpActThemeData::SetHint(bool isHint)
{
	if (m_isHint == isHint)
	{
		NotifyClient();
		return;
	}

	m_isHint = isHint;

	NotifyClient();

	SetModify();
}

void SpActThemeData::NotifyClient()
{
	if (!m_pRole->GetFlag(RSF_ISWaitLogin))
	{
		PtcG2C_ThemeActivityChangeNtf oPtc;
		oPtc.m_Data.set_ishint(m_isHint);
		oPtc.m_Data.set_actid(m_actid);
		for (auto it = m_mapFirstScene.begin(); it != m_mapFirstScene.end(); it ++)
		{
			KKSG::SpFirstCompleteScene* pData =  oPtc.m_Data.add_scene();
			pData->set_scenetype(it->first);
			for (UINT32 i = 0; i < it->second.size(); i ++)
			{
				pData->add_sceneid(it->second[i]);
			}
		}
		m_pRole->Send(oPtc);
	}
}

void SpActThemeData::OnSpActivityOpen()
{
	NotifyClient();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

