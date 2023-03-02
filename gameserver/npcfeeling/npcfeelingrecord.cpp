#include "pch.h"
#include "npcfeelingrecord.h"
#include "util/gametime.h"
#include "unit/role.h"
#include "npcfeelingmgr.h"
#include "mail/mailconfig.h"
#include "mail/mailsender.h"
#include "define/tlogenum.h"
#include "config/itemdropconfig.h"
#include "gamelogic/payv2Record.h"

CNpcFeelingRecord::CNpcFeelingRecord(Role* pRole)
	:m_pRole(pRole)
{

}

CNpcFeelingRecord::~CNpcFeelingRecord()
{

}

bool CNpcFeelingRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if(poRoleAllInfo && poRoleAllInfo->has_npcflrec())
	{
		m_data = poRoleAllInfo->npcflrec();
	}

	Update();

	return true;
}

void CNpcFeelingRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if(m_modified.TestAndReset())
	{
		auto& npcfl = *poRoleAllInfo->mutable_npcflrec();
		roChanged.insert(&npcfl);
		npcfl.Clear();

		npcfl = m_data;
	}
}

void CNpcFeelingRecord::Update()
{
	UINT32 now = UINT32(GameTime::GetTime());
	if(0 == m_data.lastupdaytime())
	{
		m_data.set_lastupdaytime(now);
		m_modified.Set();
	}
	if(!GameTime::IsInSameDay(m_data.lastupdaytime(), now, true))
	{
		//diffdaycount 时间回调会异常 线上不会出现所以不考虑
		UINT32 diffDayCount = GameTime::GetDiffDayCount(m_data.lastupdaytime(), now);
		m_data.set_lastupdaytime(now);
		m_modified.Set();
		DayZero(diffDayCount);
	}
}

void CNpcFeelingRecord::DayZero(UINT32 diffDayCount)
{
	SSInfo << " npcfeeling record dayzero diffday = " << diffDayCount << " role = " << m_pRole->GetName() << END;
	m_modified.Set();

	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		//函数调用过程中会清空原有的数据
		ResetNpcLikeItem(*m_data.mutable_npclist(i));	
		m_data.mutable_npclist(i)->clear_exchange();
	}

	UINT32 left = GetGiveLeftCount();
	if(diffDayCount > 1)
	{
		left += (diffDayCount - 1) * GetDayGiveCount();
	}
	if(left > GetGlobalConfig().NpcFeelingGiveLeftAddLimit)
	{
		left = GetGlobalConfig().NpcFeelingGiveLeftAddLimit;
	}
	m_data.set_giveleftaddcount(left);

	//次数清空
	ResetBaseCount();
}

void CNpcFeelingRecord::InfoPrint()
{
	SSInfo << " roleid = " << m_pRole->GetID() << " rolename = " << m_pRole->GetName() << END;
	bool isChanged = m_modified.TestAndReset();
	if(isChanged)
	{
		m_modified.Set();
	}
	SSInfo << " lastuptime = " << m_data.lastupdaytime() << " ischanged " << isChanged << END;
	SSInfo << " give count = " << m_data.givecount() << " buycount = " << m_data.buycount() << " leftadd = " << m_data.giveleftaddcount() << END;
	SSInfo << " exchange count = " << m_data.exchangecount() << " favortrriger count = " << m_data.triggerfavorcount() << END;
	SSInfo << " npc list size = " << m_data.npclist().size() << END;
	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		InfoPrint(m_data.npclist(i));
	}
	SSInfo << " unite list size = " << m_data.unitelist().size() << END;
	for(int i = 0; i < m_data.unitelist_size(); ++i)
	{
		InfoPrint(m_data.unitelist(i));
	}

	SSInfo << " return drop count = " << m_data.returncount() << " list size = " << m_data.returndrop_size() << END;
	for(int i = 0; i < m_data.returndrop_size(); ++i)
	{
		const auto& tmp = m_data.returndrop(i);
		SSInfo << " dropid = " << tmp.dropid() << " drop time = " << tmp.gtime() << " mail conf = " << tmp.mailconfid() << END;
	}
}

void CNpcFeelingRecord::InfoPrint(const KKSG::NpcFeelingOneNpc& data)
{
	SSInfo << " npcid = " << data.npcid() << " level = " << data.level() << " exp = " << data.exp() << END;
	for(int i = 0; i < data.likeitem_size(); ++i)
	{
		const auto& litem = data.likeitem(i);
		SSInfo << " itemid = " << litem.itemid() << " itemcount = " << litem.itemcount() << " exp = " << litem.addexp() << " type = " << litem.type() << END;
	}
	for(int i = 0; i < data.exchange_size(); ++i)
	{
		const auto& item = data.exchange(i);
		SSInfo << " itemid = " << item.itemid() << " itemcount = " << item.itemcount() << " isbind = " << item.isbind() << END;
	}
}

void CNpcFeelingRecord::InfoPrint(const KKSG::NpcFeelingUnite& data)
{
	SSInfo << " uniteid = " << data.id() << " level = " << data.level() << END;
}

void CNpcFeelingRecord::AddNpc(UINT32 npcID)
{
	m_modified.Set();

	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		if(m_data.npclist(i).npcid() == npcID)
		{
			SSError << " add npcid repeat role = " << m_pRole->GetID() << " npcid = " << npcID << END;
			return ;
		}
	}


	if(m_data.npclist_size() >= int(MaxNpcCount()))
	{
		SSError << " npc count to many role = " << m_pRole->GetID() << END;
		return;
	}

	//new npc
	KKSG::NpcFeelingOneNpc oneNpc;
	oneNpc.set_npcid(npcID);
	oneNpc.set_level(1);
	oneNpc.set_exp(0);
	oneNpc.set_isnew(true);
	ResetNpcLikeItem(oneNpc);

	*m_data.add_npclist() = oneNpc;
}

void CNpcFeelingRecord::FillNpcIDs(std::set<UINT32>& npcIDs)
{
	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		npcIDs.insert(m_data.npclist(i).npcid());
	}
}

bool CNpcFeelingRecord::IsSystemOpened()
{
	return m_pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_NPCFEELING);
}

const KKSG::NpcFeelingOneNpc* CNpcFeelingRecord::GetOneNpc(UINT32 npcID)
{
	return GetOneNpcNoConst(npcID);
}

void CNpcFeelingRecord::SetNpcExpLevel(UINT32 npcID, UINT32 exp, UINT32 level)
{
	auto pData = GetOneNpcNoConst(npcID);
	if(NULL == pData)
	{
		SSError << " get one npc failed npcid = " << npcID << " role = " << m_pRole->GetID() << END;
		return;
	}

	m_modified.Set();

	pData->set_exp(exp);
	pData->set_level(level);
}

UINT32 CNpcFeelingRecord::GetGiveLeftCount()
{
	UINT32 all = GetGiveMaxCount();
	if(all > m_data.givecount())
	{
		return all - m_data.givecount();
	}
	return 0;
}

UINT32 CNpcFeelingRecord::GetDayGiveCount()
{
	return GetGlobalConfig().NpcFeelingGiveCount + m_pRole->Get<CPayV2Record>()->GetNpcFeelingCount();
}

UINT32 CNpcFeelingRecord::GetGiveMaxCount()
{
	return m_data.buycount() + m_data.giveleftaddcount() + GetDayGiveCount();
}

void CNpcFeelingRecord::ApplyEffect(CombatAttribute *combatAttr)
{
	if(NULL == combatAttr)
	{
		return;
	}

	if(!IsSystemOpened())
	{
		return;
	}

	//单个NPC加成
	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		UINT32 npcID = m_data.npclist(i).npcid();	
		UINT32 level = m_data.npclist(i).level();
		auto pConf = CNpcFeelingMgr::Instance()->GetNpcAttrConf(npcID, level);
		if(NULL == pConf)
		{
			SSError << " find npc attr conf failed npcid = " << npcID << " level = " << level << " role = " << m_pRole->GetID() << END;
			continue;
		}
		for(size_t j = 0; j < pConf->Attr.size(); ++j)
		{
			const auto& attr = pConf->Attr[j];
			combatAttr->AddAttr(CombatAttrDef(attr[0]), attr[1]);
		}
	}

	//联合Npc加成
	for(int i = 0; i < m_data.unitelist_size(); ++i)
	{
		UINT32 uniteID = m_data.unitelist(i).id();
		UINT32 level = m_data.unitelist(i).level();
		auto pConf = CNpcFeelingMgr::Instance()->GetUniteAttrConf(uniteID, level);
		if(NULL == pConf)
		{
			SSError << " find npc unite attr conf failed uniteid = " << uniteID << " level = " << level << " role = " << m_pRole->GetID() << END;
			continue;
		}
		for(size_t j = 0; j < pConf->Attr.size(); ++j)
		{
			const auto& attr = pConf->Attr[j];
			combatAttr->AddAttr(CombatAttrDef(attr[0]), attr[1]);
		}
	}
}

UINT32 CNpcFeelingRecord::MaxNpcCount()
{
	return 1000;//以防万一
}

const KKSG::NpcFeelingUnite* CNpcFeelingRecord::GetNpcUnite(UINT32 uinteID)
{
	for(int i = 0; i < m_data.unitelist_size(); ++i)
	{
		if(m_data.unitelist(i).id() == uinteID)
		{
			return &m_data.unitelist(i);
		}
	}
	return NULL;
}

void CNpcFeelingRecord::SetNpcUniteLevel(UINT32 uniteID, UINT32 level)
{
	m_modified.Set(); 

	const auto pTmp = GetNpcUnite(uniteID);
	if(pTmp)
	{
		auto pData = const_cast<KKSG::NpcFeelingUnite*>(pTmp);
		if(pData)
		{
			pData->set_level(level);
		}
	}
	else
	{
		auto& npcUnite = *m_data.add_unitelist();
		npcUnite.set_id(uniteID);
		npcUnite.set_level(level);
	}
}

UINT32 CNpcFeelingRecord::GetOneNpcTotalExp(const KKSG::NpcFeelingOneNpc* pOneNpc)
{
	if(NULL == pOneNpc)
	{
		return 0;
	}
	const auto& oneNpc = *pOneNpc;
	auto pConf = CNpcFeelingMgr::Instance()->GetNpcAttrConf(oneNpc.npcid(), oneNpc.level());
	if(pConf)
	{
		return (pConf->needExp + oneNpc.exp());
	}
	SSError << " find npc attr conf failed npcid = " << oneNpc.npcid() << " level = " << oneNpc.level() << " role = " << m_pRole->GetID() << END;
	return oneNpc.exp();
}

UINT32 CNpcFeelingRecord::GetAllNpcTotalExp()
{
	UINT32 allExp = 0;
	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		allExp += GetOneNpcTotalExp(&m_data.npclist(i));
	}
	return allExp;
}

UINT32 CNpcFeelingRecord::GetExchagneLeftCount()
{
	if(GetGlobalConfig().NpcFeelingExchangeCount > m_data.exchangecount())
	{
		return GetGlobalConfig().NpcFeelingExchangeCount - m_data.exchangecount();
	}
	return 0;
}

UINT32 CNpcFeelingRecord::GetReturnLeftCount()
{
	if(GetGlobalConfig().NpcFeelingReturnCount > m_data.returncount())
	{
		return GetGlobalConfig().NpcFeelingReturnCount - m_data.returncount();
	}
	return 0;
}

void CNpcFeelingRecord::AddGiveCount()
{
	m_modified.Set();	
	m_data.set_givecount(m_data.givecount() + 1);
}

void CNpcFeelingRecord::AddExchangeCount()
{
	m_modified.Set();	
	m_data.set_exchangecount(m_data.exchangecount() + 1);
}

void CNpcFeelingRecord::AddReturnCount()
{
	m_modified.Set();	
	m_data.set_returncount(m_data.returncount() + 1);	
}

void CNpcFeelingRecord::AddExchange(UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role)
{
	m_modified.Set();

	auto pData = GetOneNpcNoConst(npcID);
	if(NULL == pData)
	{
		return;
	}
	*pData->add_exchange() = role2npc;
	*pData->add_exchange() = npc2role;
}

void CNpcFeelingRecord::DelExchange(UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role)
{
	m_modified.Set();
	auto pData = GetOneNpcNoConst(npcID);
	if(NULL == pData)
	{
		return;
	}

	int index = GetExchangeIndex(*pData, role2npc, npc2role);
	int N = pData->exchange_size();
	if(index >= 0 && index + 1 < N && N >= 2)
	{
		if(index + 1 != N - 1)	//不是最后两个
		{
			pData->mutable_exchange()->SwapElements(index, N - 2);
			pData->mutable_exchange()->SwapElements(index + 1, N - 1);
		}
		pData->mutable_exchange()->RemoveLast();
		pData->mutable_exchange()->RemoveLast();
	}
}

bool CNpcFeelingRecord::HaveExchange(UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role)
{
	auto pData = GetOneNpcNoConst(npcID);
	if(NULL == pData)
	{
		return false;
	}
	return (GetExchangeIndex(*pData, role2npc, npc2role) != -1);
}

int CNpcFeelingRecord::GetExchangeIndex(const KKSG::NpcFeelingOneNpc& oneNpc, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role)
{
	auto pMgr = CNpcFeelingMgr::Instance();
	for(int i = 0; i + 1 < oneNpc.exchange_size(); ++i)
	{
		if(pMgr->IsSameItem(oneNpc.exchange(i), role2npc)
			&& pMgr->IsSameItem(oneNpc.exchange(i+1), npc2role))
		{
			return i;
		}
	}
	return -1;//-1表示没有
}

void CNpcFeelingRecord::AddReturnDrop(UINT32 drapID, UINT32 gtime, UINT32 mailConfID)
{
	m_modified.Set();

	auto& tmp = *m_data.add_returndrop();
	tmp.set_dropid(drapID);
	tmp.set_gtime(gtime);
	tmp.set_mailconfid(mailConfID);

	//排序
	for(int i = m_data.returndrop_size() - 1; i - 1 >= 0; --i)
	{
		if(m_data.returndrop(i-1).gtime() < m_data.returndrop(i).gtime())
		{
			m_data.mutable_returndrop()->SwapElements(i - 1, i);	
		}
	}

	if(m_data.returndrop_size() > 100 && m_data.returndrop_size() > int(GetGlobalConfig().NpcFeelingReturnCount))
	{
		SSError << " return drop size to many role = " << m_pRole->GetID() << END;
		m_data.mutable_returndrop()->RemoveLast();
	}
}

void CNpcFeelingRecord::CheckReturnDrop(UINT32 now)
{
	if(m_data.returndrop_size() == 0)
	{
		return;
	}
	//每次只检查最后一个
	KKSG::NpcFlReturn tmp = m_data.returndrop(m_data.returndrop_size() - 1);
	if(now < tmp.gtime())
	{
		return ;//时间未到
	}

	m_data.mutable_returndrop()->RemoveLast();
	m_modified.Set();

	//发邮件
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(tmp.mailconfid());
	if(NULL == pMailConf)
	{
		SSError << " mail conf = null confid = " << tmp.mailconfid() << " role = " << m_pRole->GetID() << END;
		return ;
	}
	auto pItem =  ItemDropConfig::Instance()->GetRandomDrop(tmp.dropid(), m_pRole->GetLevel());		
	if(NULL == pItem)
	{
		SSError << " find drop item failed dropid = " << tmp.dropid() << " level = " << m_pRole->GetLevel() << " role= " << m_pRole->GetID() << END;
		return;
	}
	MailSender mailsender;
	mailsender.SetContent(Mail_System, pMailConf->m_title, pMailConf->m_content);

	mailsender.AddItem(*pItem);
	mailsender.SetReason(ItemFlow_NpcFeeling, ItemFlow_NpcFeeling_NpcReturn);

	mailsender.Send(m_pRole->GetID());
}

void CNpcFeelingRecord::AddBuyCount()
{
	m_data.set_buycount(m_data.buycount() + 1);
}

UINT32 CNpcFeelingRecord::GetBuyLeftCount()
{
	if(GetGlobalConfig().NpcFeelingBuyCount > m_data.buycount())
	{
		return GetGlobalConfig().NpcFeelingBuyCount - m_data.buycount();
	}
	return 0;
}

UINT32 CNpcFeelingRecord::GetBuyCount()
{
	return m_data.buycount();
}

void CNpcFeelingRecord::FillClientData(KKSG::NpcFlRes& roRes)
{
	*roRes.mutable_npclist() = m_data.npclist();
	FillUniteList(roRes);
	FillClientCount(roRes);
}

void CNpcFeelingRecord::FillOneNpc(KKSG::NpcFlRes& roRes, UINT32 npcID)
{
	auto pData = GetOneNpc(npcID);
	if(pData)
	{
		*roRes.add_changenpclist() = *pData;
	}
}

void CNpcFeelingRecord::FillOneUnite(KKSG::NpcFlRes& roRes, UINT32 uniteID)
{
	auto pData = GetNpcUnite(uniteID);
	if(pData)
	{
		*roRes.add_unitelist() = *pData;
	}
}

void CNpcFeelingRecord::FillUniteList(KKSG::NpcFlRes& roRes)
{
	*roRes.mutable_unitelist() = m_data.unitelist();
}

void CNpcFeelingRecord::FillClientCount(KKSG::NpcFlRes& roRes)
{
	roRes.set_giveleftcount(GetGiveLeftCount());
	roRes.set_buyleftcount(GetBuyLeftCount());
	roRes.set_buycost(GetBuyCostCount());
}

UINT32 CNpcFeelingRecord::GetBuyCostCount()
{
	const auto& buyCost = GetGlobalConfig().NpcFeelingBuyCost;
	if(buyCost.empty())
	{
		return 0;
	}
	UINT32 index = GetBuyCount();

	UINT32 itemCount = 0;	
	if(index < buyCost.size())
	{
		itemCount = buyCost[index];
	}
	else
	{
		itemCount = buyCost.back();
	}
	return itemCount;
}

KKSG::NpcFeelingOneNpc* CNpcFeelingRecord::GetOneNpcNoConst(UINT32 npcID)
{
	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		if(m_data.npclist(i).npcid() == npcID)
		{
			return m_data.mutable_npclist(i);
		}
	}
	return NULL;
}

UINT32 CNpcFeelingRecord::GetTriggerFavorLeftCount()
{
	if(GetGlobalConfig().NpcFeelingTriggerFavorCount > m_data.triggerfavorcount())
	{
		return GetGlobalConfig().NpcFeelingTriggerFavorCount - m_data.triggerfavorcount();
	}
	return 0;
}

void CNpcFeelingRecord::AddTriggerFavorLeftCount()
{
	m_modified.Set();
	m_data.set_triggerfavorcount( m_data.triggerfavorcount() + 1 );
}

void CNpcFeelingRecord::ResetNpcLikeItem(KKSG::NpcFeelingOneNpc& oneNpc)
{
	m_modified.Set();

	oneNpc.clear_likeitem();//清除原有
	MakeNpcNormalLikeItem(oneNpc);
}

void CNpcFeelingRecord::MakeNpcNormalLikeItem(KKSG::NpcFeelingOneNpc& oneNpc)
{
	m_modified.Set();

	auto pMgr = CNpcFeelingMgr::Instance();

	double addWeight = GetAddExpWeight();
	auto pConf = pMgr->GetConf(oneNpc.npcid(), oneNpc.level());
	if(NULL == pConf)
	{
		return;
	}
	for(size_t i = 0; i < pConf->likeItem.size(); ++i)
	{
		pMgr->Sequence2NpcItem(pConf->likeItem[i], *oneNpc.add_likeitem(), KKSG::NPCFL_ITEM_NORMAL, addWeight);
	}
	if(!pConf->randomItem.empty())
	{
		UINT32 index = XRandom::randInt(0, pConf->randomItem.size());
		if(index < pConf->randomItem.size())
		{
			pMgr->Sequence2NpcItem(pConf->randomItem[index], *oneNpc.add_likeitem(), KKSG::NPCFL_ITEM_RANDOM, addWeight);
		}
	}
}

void CNpcFeelingRecord::MakeNpcFavorLikeItem(KKSG::NpcFeelingOneNpc& oneNpc)
{
	m_modified.Set();

	auto pMgr = CNpcFeelingMgr::Instance();

	double addWeight = GetAddExpWeight();
	auto pConf = pMgr->GetConf(oneNpc.npcid(), oneNpc.level());
	if(NULL == pConf)
	{
		return;
	}
	if(pConf->favorItem.empty())
	{
		return;
	}
	UINT32 index = XRandom::randInt(0, pConf->favorItem.size());
	if(index >= pConf->favorItem.size())
	{
		return;
	}
	pMgr->Sequence2NpcItem(pConf->favorItem[index], *oneNpc.add_likeitem(), KKSG::NPCFL_ITEM_TRIGGER_FAVOR, addWeight);
}

double CNpcFeelingRecord::GetAddExpWeight()
{
	double myExp = double(GetAllNpcTotalExp());
	double topAvgExp = CNpcFeelingMgr::Instance()->GetTopAvExp();
	double addWeight = 0.0;
	if(myExp < topAvgExp * GetGlobalConfig().NpcFeelingAddExtraExpRange / 100.0)
	{
		addWeight += GetGlobalConfig().NpcFeelingAddExtraExpWeight / 100.0;
	}
	// 回流加成 确保load之前数据已经有了
	if(m_pRole->IsBackFlowRole())
	{
		addWeight += GetGlobalConfig().NpcFeelingBackFlowWeight / 100.0;
	}
	else if(m_pRole->IsBackFlowSmallRole())
	{
		addWeight += GetGlobalConfig().NpcFeelingSmallBackFlowWeight / 100.0;
	}

	return addWeight;
}

void CNpcFeelingRecord::AddNpcFavorItem(UINT32 npcID)
{
	m_modified.Set();	

	auto pData = GetOneNpcNoConst(npcID);
	if(pData)
	{
		MakeNpcFavorLikeItem(*pData);
	}
}

UINT32 CNpcFeelingRecord::GetEnhanceReduce()
{
	UINT32 reduce = 0;
	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		UINT32 npcID = m_data.npclist(i).npcid();
		UINT32 level = m_data.npclist(i).level();
		auto pConf = CNpcFeelingMgr::Instance()->GetNpcSpecial(npcID);
		if(NULL == pConf)
		{
			continue;
		}
		if(pConf->enhanceReduce.empty())
		{
			continue;
		}
		UINT32 redu = 0;
		for(size_t j = 0; j < pConf->enhanceReduce.size(); ++j)
		{
			const auto& tmp = pConf->enhanceReduce[j];
			if(level >= tmp[0])
			{
				redu = tmp[1];
			}
		}
		reduce += redu;
	}
	return reduce;
}

void CNpcFeelingRecord::ClearNpcNewFlag()
{
	for(int i = 0; i < m_data.npclist_size(); ++i)
	{
		m_data.mutable_npclist(i)->set_isnew(false);
	}
}

void CNpcFeelingRecord::Clear()
{
	m_data.Clear();
	m_data.set_lastupdaytime(UINT32(GameTime::GetTime()));
	m_modified.Set();
}

void CNpcFeelingRecord::DelNpcItem(UINT32 npcID, const KKSG::NpcLikeItem& litem)
{
	auto pData = GetOneNpcNoConst(npcID);
	if(NULL == pData)
	{
		return;
	}
	const int N = pData->likeitem_size();
	if(N == 0)
	{
		return;
	}
	int index = -1;
	for(int i = 0; i < N; ++i)
	{
		if(CNpcFeelingMgr::Instance()->IsSameItem(pData->likeitem(i), litem))
		{
			index = i;
			break;
		}
	}
	if(0 <= index && index < N)
	{
		if(index != N - 1)
		{
			pData->mutable_likeitem()->SwapElements(index, N-1);
		}
		pData->mutable_likeitem()->RemoveLast();
	}

	m_modified.Set();
}

void CNpcFeelingRecord::ResetBaseCount()
{
	m_modified.Set();

	m_data.set_exchangecount(0);
	m_data.set_returncount(0);
	m_data.set_givecount(0);
	m_data.set_buycount(0);
	m_data.set_triggerfavorcount(0);
}
