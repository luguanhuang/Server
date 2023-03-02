#include "pch.h"
#include "npcfeelingmgr.h"
#include "npcfeelingrecord.h"
#include "unit/role.h"
#include "util/XRandom.h"
#include "gamelogic/bagtransition.h"
#include "unit/rolemanager.h"
#include "gamelogic/levelsealMgr.h"
#include "util/gametime.h"
#include "scene/scenebase.h"
#include "npcfeeling/ptcg2c_npcflntf.h"
#include "scene/scene.h"

const UINT32 NpcFeelingUint32Max = UINT32(-1);

CNpcFeelingMgr::CNpcFeelingMgr()
{
	m_handler = 0;
}

CNpcFeelingMgr::~CNpcFeelingMgr()
{

}

bool CNpcFeelingMgr::Init()
{
	if(!LoadFile())
	{
		return false;
	}

	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);	

	return true;
}

void CNpcFeelingMgr::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}
}

void CNpcFeelingMgr::ClearConfig()
{
	m_npcTable.Clear();	
	m_npcAttr.Clear();
	m_unitAttr.Clear();
	m_npcSpecial.Clear();
	m_npclevel2index.clear();
	m_unitelevel2index.clear();
}

bool CNpcFeelingMgr::CheckConfig()
{
	NpcFeeling tmpNpcTable;
	if(!tmpNpcTable.LoadFile("table/NpcFeeling.txt"))
	{
		SSError << " load NpcFeeling.txt failed " << END;
		return false;
	}
	NpcFeelingAttr tmpNpcAttrTable;
	if(!tmpNpcAttrTable.LoadFile("table/NpcFeelingAttr.txt"))
	{
		SSError << " load NpcFeelingAttr.txt failed " << END;
		return false;
	}
	NpcUniteAttr tmpUnitAttrTable;
	if(!tmpUnitAttrTable.LoadFile("table/NpcUniteAttr.txt"))
	{
		SSError << " load NpcUnitAttr.txt failed " << END;
		return false;
	}
	NpcFeelingSpecial tmpNpcSpecial;
	if(!tmpNpcSpecial.LoadFile("table/NpcFeelingSpecial.txt"))
	{
		SSError << " load NpcFeelingSpecial failed " << END;
		return false;
	}

	for(size_t i = 0; i < tmpNpcTable.Table.size(); ++i)
	{
		auto pConf = tmpNpcTable.Table[i];
		if(pConf->exchangeGet.size() != pConf->exchangeGive.size())
		{
			SSError << " exchange config error index = " << i << END;
			return false;
		}
	}

	return true;
}

bool CNpcFeelingMgr::LoadFile()
{
	if(!CheckConfig())
	{
		return false;
	}

	ClearConfig();

	m_npcTable.LoadFile("table/NpcFeeling.txt");

	m_npcAttr.LoadFile("table/NpcFeelingAttr.txt");
	BuildNpcLevel2Index();

	m_unitAttr.LoadFile("table/NpcUniteAttr.txt");
	BuildUniteLevel2Index();

	m_npcSpecial.LoadFile("table/NpcFeelingSpecial.txt");

	return true;
}

void CNpcFeelingMgr::OnLevelUp(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}

	CheckGiveNewNpc(pRole);
}

void CNpcFeelingMgr::OnSystemOpen(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}

	CheckGiveNewNpc(pRole);
}

void CNpcFeelingMgr::OnLogin(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}

	CheckGiveNewNpc(pRole);
}

void CNpcFeelingMgr::InfoPrint(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	//pRole->Get<CNpcFeelingRecord>()->InfoPrint();
	InfoPrintTable();
	//InfoPrint(m_globalData);
}

void CNpcFeelingMgr::InfoPrint(KKSG::NpcFlGlobal& data)
{
	for(int i = 0; i < data.lastday_roles_size(); ++i)
	{
		const auto& tmp = data.lastday_roles(i);
		SSInfo << " last: role = " << tmp.roleid() << " exp = " << tmp.exp() << END;
	}
	for(int i = 0; i < data.lastweek_npc2role_size(); ++i)
	{
		const auto& tmp = data.lastweek_npc2role(i);
		SSInfo << " last: npc = " << tmp.npcid() << " roleid = " << tmp.role().roleid() << " rolename = " << tmp.rolename() << " exp = " << tmp.role().exp() << END;
	}
}

void CNpcFeelingMgr::CheckGiveNewNpc(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	if(!pRecord->IsSystemOpened())
	{
		return;
	}
	pRecord->Update();
	std::set<UINT32> npcIDs;
	pRecord->FillNpcIDs(npcIDs);
	std::set<UINT32> npcGive;
	for(size_t i = 0; i < m_npcTable.Table.size(); ++i)
	{
		auto pConf = m_npcTable.Table[i];
		if(pConf->openLevel > UINT32(pRole->GetLevel()))
		{
			continue;
		}
		if(i > 0 && pConf->npcId == m_npcTable.Table[i-1]->npcId)
		{
			continue;
		}
		if(npcIDs.find(pConf->npcId) != npcIDs.end())//已有
		{
			continue;
		}

		npcGive.insert(pConf->npcId);
	}

	for(auto it = npcGive.begin(); it != npcGive.end(); ++it)
	{
		pRecord->AddNpc(*it);
	}
	//在主城有新的NPC开启
	if(!npcGive.empty() && pRole->GetCurrScene() && pRole->GetCurrScene()->GetSceneType() == KKSG::SCENE_HALL)
	{
		PtcG2C_NpcFlNtf ptc;
		FillBaseData(pRole, ptc.m_Data);
		pRole->Send(ptc);
	}
}

NpcFeeling::RowData* CNpcFeelingMgr::GetConf(UINT32 npcID, UINT32 level /*= 0*/)
{
	for(size_t i = 0; i < m_npcTable.Table.size(); ++i)
	{
		auto pConf = m_npcTable.Table[i];
		if(pConf->npcId == npcID)
		{
			if(0 == level || 
				(pConf->feelingLevel[0] <= level && level <= pConf->feelingLevel[1]))
			{
				return pConf;
			}
		}
	}
	SSError << " get conf failed npcid = " << npcID << " level = " << level << END;
	return NULL;
}

void CNpcFeelingMgr::Sequence2NpcItem(const Sequence<UINT32,3>& seq, KKSG::NpcLikeItem& litem, KKSG::NpcFlItemType type, double addWeight/* = 0.0*/)
{
	litem.set_itemid(seq[0]);
	litem.set_itemcount(seq[1]);
	UINT32 exp = seq[2] + UINT32(seq[2] * addWeight);
	litem.set_addexp(exp);	
	litem.set_type(type);
}

KKSG::ErrorCode CNpcFeelingMgr::HandleGiveNpcItem(Role* pRole, const KKSG::NpcFlArgM2G& m2gArg, KKSG::NpcFlResM2G& m2gRes)
{ 
	UINT32 npcID = m2gArg.c2marg().npcid();
	const KKSG::NpcLikeItem& litem = m2gArg.c2marg().likeitem();

	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();	
	const auto pData = pRecord->GetOneNpc(npcID);
	if(NULL == pData)
	{
		return KKSG::ERR_FAILED;
	}

	if(0 == pRecord->GetGiveLeftCount())
	{
		return KKSG::ERR_NPCFL_GIVE_GIFT_COUNT_LESS;
	}

	bool npcLike = false;
	for(int i = 0; i < pData->likeitem_size(); ++i)
	{
		const auto& tmp = pData->likeitem(i);	
		if(IsSameItem(litem, tmp))
		{
			npcLike = true;	
			break;
		}
	}

	if(!npcLike)
	{
		return KKSG::ERR_FAILED;
	}

	if(IsNpcMaxLevel(pData->npcid(), pData->level()))
	{
		return KKSG::ERR_NPCFL_NPC_LEVEL_MAX;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_NpcFeeling, ItemFlow_NpcFeeling_GiveNpc);
	if(!take.TakeItem(litem.itemid(), litem.itemcount()))
	{
		take.RollBack();
		return KKSG::ERR_NPCFL_ITEM_LESS;
	}
	take.NotifyClient();

	UINT32 level = pData->level();
	UINT32 exp = pData->exp() + litem.addexp();//加经验
	pRecord->SetNpcExpLevel(pData->npcid(), exp, level);
	pRecord->AddGiveCount();

	if(litem.type() == KKSG::NPCFL_ITEM_TRIGGER_FAVOR)//一次性
	{
		pRecord->DelNpcItem(pData->npcid(), litem);
	}

	//更新到MS
	m2gRes.mutable_roleexp()->set_roleid(pRole->GetID());
	m2gRes.mutable_roleexp()->set_exp(pRecord->GetAllNpcTotalExp());
	m2gRes.mutable_npc2role()->set_npcid(npcID);
	m2gRes.mutable_npc2role()->mutable_role()->set_roleid(pRole->GetID());
	m2gRes.mutable_npc2role()->mutable_role()->set_exp(pRecord->GetOneNpcTotalExp(pRecord->GetOneNpc(npcID)));
	m2gRes.mutable_npc2role()->set_rolename(pRole->GetName());

	CheckTriggerExchange(pRole, npcID, *m2gRes.mutable_c2mres());
	CheckTriggerReturn(pRole, npcID);
	CheckTriggerFavor(pRole, npcID);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CNpcFeelingMgr::HandleUpNpcLevel(Role* pRole, UINT32 npcID)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();	
	const auto pData = pRecord->GetOneNpc(npcID);
	if(NULL == pData)
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 level = pData->level();
	UINT32 exp = pData->exp();

	if(IsNpcMaxLevel(npcID, level))
	{
		return KKSG::ERR_NPCFL_NPC_LEVEL_MAX;
	}

	UINT32 needExp = GetLevelUpNeedExp(npcID, level + 1);
	if(NpcFeelingUint32Max == needExp)
	{
		return KKSG::ERR_FAILED;
	}
	if(needExp > exp)
	{
		return KKSG::ERR_NPCFL_EXP_LESS;
	}

	exp -= needExp;
	level += 1;

	pRecord->SetNpcExpLevel(pData->npcid(), exp, level);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CNpcFeelingMgr::HandleNpcUinteLevel(Role* pRole, UINT32 uniteID)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	UINT32 curLevel = 0;//没有激活是0级
	auto pUnite = pRecord->GetNpcUnite(uniteID);
	if(pUnite)
	{
		curLevel = pUnite->level();
	}
	UINT32 toLevel = GetUniteNextLevel(uniteID, curLevel);
	if(NpcFeelingUint32Max == toLevel)
	{
		return KKSG::ERR_FAILED;
	}
	auto pConf = GetUniteAttrConf(uniteID, toLevel);	
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 uniteLevel = 0;
	for(size_t i = 0; i < pConf->npcId.size(); ++i)
	{
		UINT32 npcID = pConf->npcId[i];
		const auto pData = pRecord->GetOneNpc(npcID);
		if(pData)
		{
			uniteLevel += pData->level();
		}
	}

	if(uniteLevel < toLevel)
	{
		return KKSG::ERR_NPCFL_UNITE_LEVEL_LESS;
	}

	pRecord->SetNpcUniteLevel(uniteID, toLevel);

	return KKSG::ERR_SUCCESS;
}

UINT32 CNpcFeelingMgr::GetMaxLevelLimit()
{
	auto& conf = GetGlobalConfig().NpcFeelingSeal2Level;
	auto it = conf.find(CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType);
	if(it != conf.end())
	{
		return it->second;
	}
	return 0;
}

void CNpcFeelingMgr::BuildNpcLevel2Index()
{
	for(size_t i = 0; i < m_npcAttr.Table.size(); ++i)
	{
		auto pConf = m_npcAttr.Table[i];
		auto ret = m_npclevel2index.insert(std::make_pair(NpcAttrKey(pConf->npcId, pConf->level), i));
		if(!ret.second)
		{
			SSWarn << " npc level 2 index insert failed npcid = " << pConf->npcId << " level = " << pConf->level << " index = " << i << END;
		}
	}
}

void CNpcFeelingMgr::BuildUniteLevel2Index()
{
	for(size_t i = 0; i < m_unitAttr.Table.size(); ++i)
	{
		auto pConf = m_unitAttr.Table[i];
		auto ret = m_unitelevel2index.insert(std::make_pair(NpcAttrKey(pConf->id, pConf->level), i));
		if(!ret.second)
		{
			SSWarn << " npc unite 2 index insert failed id = " << pConf->id << " level = " << pConf->level << " index = " << i << END;
		}
	}
}

NpcFeelingAttr::RowData* CNpcFeelingMgr::GetNpcAttrConf(UINT32 npcID, UINT32 level)
{
	auto it = m_npclevel2index.find(NpcAttrKey(npcID, level));
	if(it == m_npclevel2index.end())
	{
		return NULL;
	}
	UINT32 index = it->second;
	if(index >= m_npcAttr.Table.size())
	{
		return NULL;
	}
	return m_npcAttr.Table[index];
}

NpcUniteAttr::RowData* CNpcFeelingMgr::GetUniteAttrConf(UINT32 uniteID, UINT32 level)
{
	auto it = m_unitelevel2index.find(NpcAttrKey(uniteID, level));
	if(it == m_unitelevel2index.end())
	{
		return NULL;
	}
	UINT32 index = it->second;
	if(index >= m_unitAttr.Table.size())
	{
		return NULL;
	}
	return m_unitAttr.Table[index];
}

bool CNpcFeelingMgr::IsSameItem(const KKSG::ItemBrief& item1, const KKSG::ItemBrief& item2)
{
	if(item1.itemid() == item2.itemid() && item1.itemcount() == item2.itemcount())
	{
		return true;
	}
	return false;
}

bool CNpcFeelingMgr::IsSameItem(const KKSG::NpcLikeItem& item1, const KKSG::NpcLikeItem& item2)
{
	if(item1.itemid() == item2.itemid() && 
		item1.itemcount() == item2.itemcount() && 
		item1.addexp() == item2.addexp() && 
		item1.type() == item2.type())
	{
		return true;
	}
	return false;
}

void CNpcFeelingMgr::CheckReturnMail()
{
	UINT32 now = UINT32(GameTime::GetTime()); 
	auto itBegin = RoleManager::Instance()->Begin();
	auto itEnd = RoleManager::Instance()->End();
	for(auto it = itBegin; it != itEnd; ++it)
	{
		(*it)->Get<CNpcFeelingRecord>()->CheckReturnDrop(now);
	}
}

void CNpcFeelingMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckReturnMail();
}

KKSG::ErrorCode CNpcFeelingMgr::HandleExchange(Role* pRole, UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}

	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	if(!pRecord->HaveExchange(npcID, role2npc, npc2role))
	{
		return KKSG::ERR_FAILED;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_NpcFeeling, ItemFlow_NpcFeeling_EX_Role2Npc);
	if(!take.TakeItem(role2npc.itemid(), role2npc.itemcount()))
	{
		take.RollBack();
		return KKSG::ERR_NPCFL_ITEM_LESS;
	}
	take.NotifyClient();

	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_NpcFeeling, ItemFlow_NpcFeeling_EX_Npc2Role);
	give.GiveItem(npc2role.itemid(), npc2role.itemcount(), npc2role.isbind());
	give.NotifyClient();

	pRecord->DelExchange(npcID, role2npc, npc2role);

	return KKSG::ERR_SUCCESS;
}

void CNpcFeelingMgr::CheckTriggerExchange(Role* pRole, UINT32 npcID, KKSG::NpcFlRes& roRes)
{
	if(NULL == pRole)
	{
		return;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	if(0 == pRecord->GetExchagneLeftCount())
	{
		return;
	}
	if(!RandomPro(GetGlobalConfig().NpcFeelingExchangePro))
	{
		return;
	}
	auto pData = pRecord->GetOneNpc(npcID);
	if(NULL == pData)
	{
		return;
	}
	auto pConf = GetConf(pData->npcid(), pData->level());
	if(NULL == pConf)
	{
		return;
	}
	UINT32 allPro = 0;
	for(size_t i = 0; i < pConf->exchangeGet.size(); ++i)
	{
		allPro += pConf->exchangeGet[i][2];
	}
	if(0 == allPro)
	{
		return;
	}
	UINT32 randPro = XRandom::randInt(1, allPro + 1);
	for(size_t i = 0; i < pConf->exchangeGet.size(); ++i)
	{
		if(randPro <= pConf->exchangeGet[i][2])
		{
			if(i < pConf->exchangeGive.size())
			{
				KKSG::ItemBrief role2npc;
				role2npc.set_itemid(pConf->exchangeGive[i][0]);
				role2npc.set_itemcount(pConf->exchangeGive[i][1]);
				role2npc.set_isbind(true);

				KKSG::ItemBrief npc2role;
				npc2role.set_itemid(pConf->exchangeGet[i][0]);
				npc2role.set_itemcount(pConf->exchangeGet[i][1]);
				npc2role.set_isbind(true);

				*roRes.mutable_role2npc() = role2npc;
				*roRes.mutable_npc2role() = npc2role;

				pRecord->AddExchange(npcID, role2npc, npc2role);
				pRecord->AddExchangeCount();
			}
			break;
		}
		randPro -= pConf->exchangeGet[i][2];
	}
}

void CNpcFeelingMgr::CheckTriggerReturn(Role* pRole, UINT32 npcID)
{
	if(NULL == pRole)
	{
		return;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	if(0 == pRecord->GetReturnLeftCount())
	{
		return ;
	}
	if(!RandomPro(GetGlobalConfig().NpcFeelingReturnPro))	
	{
		return;
	}
	auto pData = pRecord->GetOneNpc(npcID);
	if(NULL == pData)
	{
		return;
	}
	auto pConf = GetConf(pData->npcid(), pData->level());
	if(NULL == pConf)
	{
		return;
	}

	UINT32 allPro = 0;
	for(size_t i = 0; i < pConf->npcReturn.size(); ++i)
	{
		allPro += pConf->npcReturn[i][1];
	}
	if(0 == allPro)
	{
		return;
	}
	UINT32 randPro = XRandom::randInt(1, allPro + 1);
	for(size_t i = 0; i < pConf->npcReturn.size(); ++i)
	{
		if(randPro <= pConf->npcReturn[i][1])
		{
			pRecord->AddReturnDrop(pConf->npcReturn[i][0], UINT32(GameTime::GetTime() + GetGlobalConfig().NpcFeelingReturnDelay), pConf->npcReturnMailConf);
			pRecord->AddReturnCount();
			break;
		}
		randPro -= pConf->npcReturn[i][1];
	}
}

bool CNpcFeelingMgr::RandomPro(UINT32 pro)
{
	return XRandom::randInt(1, 101) <= pro;
}

double CNpcFeelingMgr::GetTopAvExp()
{
	const auto& roles = m_globalData.lastday_roles();
	if(roles.size() < int(GetGlobalConfig().NpcFeelingTopCount))	
	{
		return 0.0;
	}
	if(roles.size() == 0)
	{
		return 0.0;
	}
	UINT32 allExp = 0;
	for(int i = 0; i < roles.size(); ++i)
	{
		allExp += roles.Get(i).exp();
	}
	return 1.0*allExp/roles.size();
}

void CNpcFeelingMgr::SyncData(const KKSG::NpcFlGlobal& data)
{
	m_globalData = data;
	SSDebug << " sync data from ms " << END;
	InfoPrint(m_globalData);
}

KKSG::ErrorCode CNpcFeelingMgr::HandleBuyGiveGiftCount(Role* pRole)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	if(0 == pRecord->GetBuyLeftCount())
	{
		return KKSG::ERR_NPCFL_BUY_GG_COUNT_LESS;
	}

	UINT32 itemID = DRAGON_COIN;
	UINT32 itemCount = pRecord->GetBuyCostCount();
	if(0 == itemCount)
	{
		return KKSG::ERR_FAILED;//不存在免费
	}
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_NpcFeeling, ItemFlow_NpcFeeling_NpcBuyCount);
	if(!take.TakeItem(itemID, itemCount))
	{
		take.RollBack();
		return KKSG::ERR_NPCFL_DRAGON_COIN_LESS;
	}
	take.NotifyClient();

	pRecord->AddBuyCount();

	return KKSG::ERR_SUCCESS;
}

void CNpcFeelingMgr::FillNpc2Role(KKSG::NpcFlRes& roRes)
{
	*roRes.mutable_npcfavorrole() = m_globalData.lastweek_npc2role();
}

void CNpcFeelingMgr::FillBaseData(Role* pRole, KKSG::NpcFlRes& roRes)
{
	if(NULL == pRole)
	{
		return;
	}
	roRes.set_npcflleveltop(GetMaxLevelLimit());
	FillNpc2Role(roRes);
	pRole->Get<CNpcFeelingRecord>()->FillClientData(roRes);
	pRole->Get<CNpcFeelingRecord>()->ClearNpcNewFlag();
}

void CNpcFeelingMgr::CheckTriggerFavor(Role* pRole, UINT32 npcID)
{
	if(NULL == pRole)
	{
		return;
	}
	auto pRecord = pRole->Get<CNpcFeelingRecord>();	
	if(0 == pRecord->GetTriggerFavorLeftCount())
	{
		return;
	}
	if(0 == pRecord->GetGiveLeftCount())
	{
		return;
	}
	if(!RandomPro(GetGlobalConfig().NpcFeelingTriggerFavorPro))
	{
		return;
	}

	pRecord->AddNpcFavorItem(npcID);
	pRecord->AddTriggerFavorLeftCount();
}

UINT32 CNpcFeelingMgr::GetUniteNextLevel(UINT32 uniteID, UINT32 curLevel)
{
	for(size_t i = 0; i < m_unitAttr.Table.size(); ++i)
	{
		auto pConf = m_unitAttr.Table[i];
		if(pConf->id == uniteID && pConf->level > curLevel)
		{
			return pConf->level;
		}
	}
	return NpcFeelingUint32Max;
}

UINT32 CNpcFeelingMgr::GetLevelUpNeedExp(UINT32 npcID, UINT32 tolevel)
{
	if(0 == tolevel)
	{
		return NpcFeelingUint32Max;
	}
	auto pConf1 = GetNpcAttrConf(npcID, tolevel - 1);
	auto pConf2 = GetNpcAttrConf(npcID, tolevel);
	if(NULL == pConf1 || NULL == pConf2)
	{
		return NpcFeelingUint32Max;
	}
	if(pConf2->needExp <= pConf1->needExp)
	{
		return NpcFeelingUint32Max;
	}
	return pConf2->needExp - pConf1->needExp;
}

NpcFeelingSpecial::RowData* CNpcFeelingMgr::GetNpcSpecial(UINT32 npcID)
{
	return m_npcSpecial.GetBynpcId(npcID);
}

void CNpcFeelingMgr::InfoPrintTable()
{
	SSInfo << " npc table size = " << m_npcTable.Table.size() << END;
	for(size_t i = 0; i < m_npcTable.Table.size(); ++i)
	{
		auto pConf = m_npcTable.Table[i];
		//SSInfo << " npcid = " << pConf->npcId << " level low = " << pConf->feelingLevel[0] << " level high = " << pConf->feelingLevel[1] << END;
	}
	SSInfo << " npc attr table size = " << m_npcAttr.Table.size() << END;	
	for(size_t i = 0; i < m_npcAttr.Table.size(); ++i)
	{
		auto pConf = m_npcAttr.Table[i];
		//SSInfo << " npc = " << pConf->npcId << " level = " << pConf->level << END;
	}
	SSInfo << " unite table size = " << m_unitAttr.Table.size() << END;
	for(size_t i = 0; i < m_unitAttr.Table.size(); ++i)
	{
		auto pConf = m_unitAttr.Table[i];
		//SSInfo << " uniteid = " << pConf->id << " level = " << pConf->level << END;
	}
	SSInfo << " npc special table size = " << m_npcSpecial.Table.size() << END;
}

bool CNpcFeelingMgr::IsNpcMaxLevel(UINT32 npcID, UINT32 level)
{
	auto pConf = GetNpcAttrConf(npcID, level + 1);
	if(NULL == pConf)
	{
		return true;
	}
	if(level >= GetMaxLevelLimit())
	{
		return true;
	}
	return false;
}
