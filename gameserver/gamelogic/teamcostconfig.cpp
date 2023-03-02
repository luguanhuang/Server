#include "pch.h"
#include "teamcostconfig.h"
#include "config/itemdropconfig.h"
#include "team/rpcg2m_tryteamcostreward.h"
#include "network/mslink.h"
#include "unit/role.h"

INSTANCE_SINGLETON(TeamCostConfig);

TeamCostConfig::TeamCostConfig()
{

}

TeamCostConfig::~TeamCostConfig()
{

}

bool TeamCostConfig::Init()
{
	if (!m_table.LoadFile("table/TeamCostDaily.txt"))
	{
		SSWarn<<"load file table/TeamCostDaily.txt failed"<<END;
		return false;
	}
	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		TeamCostDaily::RowData* data = *i;
		m_activity2reward[data->Activity].push_back(data);
	}
	return true;
}

void TeamCostConfig::Uninit()
{

}

UINT32 TeamCostConfig::GiveReward(Role* pRole, UINT32 activity, UINT32 lastvalue)
{
	UINT32 ret = 0;
	if (UINT32(-1) == lastvalue)
	{
		return ret;
	}
	if(NULL == pRole)
	{
		return ret;
	}
	UINT64 roleid = pRole->GetID();
	UINT32 level = pRole->GetLevel();
	bool ishit = false;
	for (auto i = m_activity2reward.rbegin(); i != m_activity2reward.rend(); ++i)
	{
		///> 玩家活跃度达到配置，并且没有随机过这个区间的奖励
		if (i->first > lastvalue && activity >= i->first)
		{
			ret = i->first;			
			RpcG2M_TryTeamCostReward* rpc = RpcG2M_TryTeamCostReward::CreateRpc();
			for (auto j = i->second.begin(); j != i->second.end(); ++j)
			{
				TeamCostDaily::RowData* data = *j;
				///> 随机成功退出(-1表示随机成功)
				UINT32 random = XRandom::randInt(0, 101);
				if (random < data->Possibility)
				{
					const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(data->DropId, level);
					if (NULL != item)
					{
						rpc->m_oArg.add_minrank(data->Level[0]);
						rpc->m_oArg.add_maxrank(data->Level[1]);
						KKSG::ItemBrief* brief = rpc->m_oArg.add_item();
						item->ToKKSG(brief);
						rpc->m_oArg.add_delaytime(data->Time);
						ishit = true;
					}
				}
			}
			if (0 == rpc->m_oArg.item_size())
			{
				delete rpc;
			}
			else
			{
				rpc->m_oArg.set_roleid(roleid);
				rpc->m_oArg.set_value(i->first);
				MSLink::Instance()->SendTo(*rpc);
			}
			break;
		}
	}
	return ishit ? (UINT32)(-1) : ret;
}
