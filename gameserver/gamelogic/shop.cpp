#include "pch.h"
#include "shop.h"
#include "item.h"
#include "bagtransition.h"
#include "bag.h"
#include "time.h"
#include "pb/project.pb.h"
#include "shopcondition.h"
#include "shoprecord.h"
#include "unit/role.h"
#include "utility/uidhelper.h"
#include "foreach.h"
#include "util/XRandom.h"
#include "util/XCommon.h"
#include "math.h"

INSTANCE_SINGLETON(ShopMgr);

ShopMgr::ShopMgr()
{

}

ShopMgr::~ShopMgr()
{
	ClearFile();
}

bool ShopMgr::Init()
{
	return LoadFile();	
}

void ShopMgr::Uninit()
{

}

bool ShopMgr::CheckFile()
{
	ShopTable tmpShopTable;
	if (!tmpShopTable.LoadFile("table/Shop.txt"))
	{
		SSWarn<<"load file table/Shop.txt failed"<<END;
		return false;
	}

	for(auto i = tmpShopTable.Table.begin(); i != tmpShopTable.Table.end(); i++)
	{
		ShopTable::RowData* data = *i;
		if (data->ItemId < VIRTUAL_ITEM_MAX)
		{
			SSWarn<<"itemid:"<<data->ItemId<<"shouldn't be virtual item"<<END;
			return false;
		}
		if (NULL == ItemConfig::Instance()->GetItemConf(data->ItemId))
		{
			SSWarn<<"conf not find itemid:"<<data->ItemId<<END;
			return false;
		}
	}

	ShopTypeTable tmpShopTypeTable;
	if (!tmpShopTypeTable.LoadFile("table/ShopType.txt"))	
	{
		SSWarn<<"load file table/ShopType.txt"<<END;
		return false;
	}

	return true;
}

bool ShopMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oTable.LoadFile("table/Shop.txt");
	m_oTypeTable.LoadFile("table/ShopType.txt");

	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
		ShopTable::RowData* data = *i;
		ShopItemBreif brief;
		brief.uid = data->ID;
		brief.item = ItemDesc(data->ItemId, 1);
		brief.isbind = !data->IsNotBind;

		brief.cost = ItemDesc(data->ConsumeItem[0], data->ConsumeItem[1]);
		brief.benefit.type = data->Benefit[0];
		brief.benefit.percent = data->Benefit[1];

		m_id2type[data->ID] = data->Type;

		for (auto j = data->Price.begin(); j != data->Price.end(); ++j)
		{
			ShopPrice tempprice;
			tempprice.count = j->seq[0];
			tempprice.price = j->seq[1];
			brief.price.push_back(tempprice);
		}

		brief.condition.level = data->LevelCondition;
		brief.condition.ppt = data->PowerpointCondition;
		brief.condition.dailycount = data->DailyCountCondition;
		brief.condition.weekcount = data->WeekCountCondition;
		brief.condition.count = data->CountCondition;
		brief.condition.vip = data->VIPCondition;
		brief.condition.arena = data->ArenaCondition;
		brief.condition.pk = data->PKCondition;
		brief.condition.guild = data->GuildLevel;


		ShopChoose choose;
		choose.uid = data->ID;
		choose.percent = data->Slot[1];
		choose.levellow = data->LevelShow[0];
		choose.levelhigh = data->LevelShow[1];
		choose.cook_level = data->CookLevel;
		choose.levellimit = (0 == choose.levellow && 0 == choose.levelhigh) ? false : true;

		for (auto l = data->Profession.begin(); l != data->Profession.end(); ++l)
		{
			m_id2oneshop[data->Type].id2shopitem[*l][data->ID] = brief;
			m_id2oneshop[data->Type].id2shopchoose[*l][data->Slot[0]].push_back(choose);
		}
	}

	m_shopconditions.push_back(new LevelShopCondition());
	m_shopconditions.push_back(new PPTShopCondition());
	m_shopconditions.push_back(new DailyCountShopCondition());
	m_shopconditions.push_back(new WeekCountShopCondition());
	m_shopconditions.push_back(new CountShopCondition());
	m_shopconditions.push_back(new VIPShopCondition());
	m_shopconditions.push_back(new ArenaShopCondition());
	m_shopconditions.push_back(new PKShopCondition());
	m_shopconditions.push_back(new GuildShopCondition());	

	return true;
}

void ShopMgr::ClearFile()
{
	m_oTable.Clear();
	m_oTypeTable.Clear();

	m_id2type.clear();
	m_id2oneshop.clear();
	for (auto i = m_shopconditions.begin(); i != m_shopconditions.end(); ++i)
	{
		if (NULL != *i)
		{
			delete *i;
			*i = NULL;
		}
	}
	m_shopconditions.clear();
}

std::map<UINT32, XItem> ShopMgr::RefreshShop(UINT32 type, UINT32 rolelevel, UINT32 roleprofession)
{
	std::map<UINT32, XItem> retitems;
	auto i = m_id2oneshop.find(type);
	if (i == m_id2oneshop.end())
	{
		return retitems;
	}

	std::map<UINT32, ShopItemBreif>* items = i->second.GetItems(roleprofession);
	std::map<UINT32, std::vector<ShopChoose>>* chooses = i->second.GetChooses(roleprofession);
	if (NULL == items || NULL == chooses)
	{
		return retitems;
	}

	for (auto k = (*chooses).begin(); k != (*chooses).end(); ++k)
	{
		UINT32 uid = RandomItem(rolelevel, k->second);
		if (0 != uid)
		{
			auto l = (*items).find(uid);
			if (l == (*items).end())
			{
				SSWarn<<"random shop item error! uid:"<<uid<<END;
			}
			else
			{
				UINT32 itemid = l->second.item.itemID;
				XItem item;
				ItemConf* conf = ItemConfig::Instance()->GetItemConf(itemid);
				if (NULL != conf)
				{
					if (conf->ItemType == EQUIP_PROTO)
					{
						itemid = EquipProtoCalc::ConvertToProfessionItemID(itemid, roleprofession);
					}
					item.Init(itemid, 1);
					item.uid = uid;
					item.isbind = l->second.isbind;
				}
				else
				{
					SSError<<"not find in conf, itemid:"<<itemid<<END;
				}
				retitems[uid] = item;
			}
		}
		else
		{
			//SSInfo<<"random shop item empty! slot:"<<k->first<<END;
		}
	}
	return retitems;
}

UINT32 ShopMgr::RandomItem(UINT32 rolelevel, std::vector<ShopChoose>& choooses)
{
	std::vector<ShopChoose> tempchooses;
	UINT32 allpercent = 0;
	for (auto i = choooses.begin(); i != choooses.end(); ++i)
	{
		if ((!i->levellimit || (rolelevel >= i->levellow && rolelevel <= i->levelhigh)))
		{
			allpercent += i->percent;
			tempchooses.push_back(*i);
		}
	}
	UINT32 random = XRandom::randInt(1, allpercent + 1);
	UINT32 allrandom = 0;
	for (auto j = tempchooses.begin(); j != tempchooses.end(); ++j)
	{
		allrandom += j->percent;
		if (random <= allrandom)
		{
			return j->uid;
		}
	}
	return 0;
}

const ShopItemBreif* ShopMgr::GetBreif(UINT32 type, UINT32 uid, UINT32 profession)
{
	auto i = m_id2oneshop.find(type);
	if (i == m_id2oneshop.end())
	{
		return NULL;
	}

	std::map<UINT32, ShopItemBreif>* items = i->second.GetItems(profession);
	if (NULL == items)
	{
		return NULL;
	}
	auto j = items->find(uid);
	if (j == items->end())
	{
		return NULL;
	}
	return &j->second;
}

UINT32 ShopMgr::IsLimit(Role* role, UINT32 type, UINT32 uid, const ShopItemBreif* brief, const ShopRoleParam& param)
{
	for (auto i = m_shopconditions.begin(); i != m_shopconditions.end(); ++i)
	{
		UINT32 error = (*i)->Check(role, &brief->condition, type, uid, param);
		if (0 != error)
		{
			return error;
		}
	}
	return 0;
}

const ItemDesc* ShopMgr::GetCost(const ShopItemBreif* brief)
{
	return &brief->cost;
}

UINT32 ShopMgr::GetCostCount(const ShopItemBreif* brief, UINT32 count)
{
	UINT32 cost = 0;
	for (auto i = brief->price.rbegin(); i != brief->price.rend(); ++i)
	{
		if (count >= i->count)
		{
			UINT32 tempcount = count - i->count;
			cost += tempcount*i->price;
			count = i->count;
		}
	}
	cost += count*brief->cost.itemCount;
	double percent = 1.0f;
	if (brief->benefit.type == 2)
	{
		percent = (double)brief->benefit.percent/(double)(100);
	}
	return (UINT32)(cost*(percent));
}

ItemDesc ShopMgr::GetRefreshCost(UINT32 type, UINT32 count)
{
	ItemDesc item;
	ShopTypeTable::RowData* data = m_oTypeTable.GetByShopID(type);
	if (NULL == data)
	{
		return item;
	}
	if (data->RefreshCost.empty())
	{
		return item;
	}
	count = count >= data->RefreshCost.size() ? data->RefreshCost.size() - 1 : count;
	item.itemID = data->RefreshCost[count].seq[0];
	item.itemCount = data->RefreshCost[count].seq[1];
	return item;
}

bool ShopMgr::IsNeedRefresh(UINT32 type)
{
	ShopTypeTable::RowData* data = m_oTypeTable.GetByShopID(type);
	return NULL != data && !data->ShopCycle.empty();
}

bool ShopMgr::GetRefreshTime(UINT32 type, UINT32& lastday, UINT32& lasttime, UINT32& lefttime)
{
	UINT32 nexttime  = 0;
	ShopTypeTable::RowData* data = m_oTypeTable.GetByShopID(type);
	if (NULL == data || data->ShopCycle.empty())
	{
		return false;
	}

	UINT32 today = XCommon::GetTodayUnique();
	time_t nowtime = time(NULL);
	tm* timenode = localtime(&nowtime);
	UINT32 daytime = (UINT32)(timenode->tm_hour * 100 + timenode->tm_min);

	if (lastday != today)
	{
		UINT32 cycle = 0;
		for (auto i = data->ShopCycle.begin(); i!= data->ShopCycle.end(); ++i)
		{
			if (daytime >= *i)	
			{
				cycle = *i;
			}
		}
		lastday = today;
		lasttime = cycle;
		return true;
	}
	else
	{
		UINT32 rettime = 0;
		for (auto i = data->ShopCycle.begin(); i!= data->ShopCycle.end(); ++i)
		{
			if (*i > lasttime && daytime >= *i)
			{
				rettime = *i;	
			}
		}
		if (0 != rettime)
		{
			lasttime = rettime;
			return true;
		}
		return false;
	}
}

bool ShopMgr::IsOpen(Role* role, UINT32 type)
{
	UINT32 systemid = GetSystemId(type);
	if (!role->Get<CRoleSystem>()->IsSystemOpened(systemid))
	{
		return false;
	}

	ShopTypeTable::RowData* data = m_oTypeTable.GetByShopID(type);
	if (NULL == data)
	{
		return false;
	}
	if (data->ShopOpen.empty())
	{
		return true;
	}
	for (auto i = data->ShopOpen.begin(); i != data->ShopOpen.end(); ++i)
	{
		UINT32 begin = i->seq[0];
		UINT32 end = i->seq[1];
		if (XCommon::IsWhithinTime((int)(begin/100), begin%100, 0, (int)(end/100), end%100, 0))
		{
			return true;
		}
	}
	return false;
}

UINT32 ShopMgr::GetType(UINT32 uid)
{
	auto i = m_id2type.find(uid);
	if (i == m_id2type.end())
	{
		return (UINT32)(-1);
	}
	return i->second;
}

UINT32 ShopMgr::GetSystemId(UINT32 type)
{
	ShopTypeTable::RowData* data = m_oTypeTable.GetByShopID(type);
	if (NULL == data)
	{
		return 0;
	}
	return data->SystemId;
}

void ShopMgr::Hint(Role* role, bool isremove, UINT32 type)
{
	UINT32 systemid = GetSystemId(type);
	if (0 != systemid)
	{
		role->HintNotify(systemid, isremove);
	}
	else
	{
		SSWarn<<"not find systemid, type:"<<type<<END;
	}
}

void ShopMgr::Test()
{

}
