#include "pch.h"
#include "photo/rpcc2g_photographeffect.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "photo/photo_manager.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/payconfig.h"
#include "photo/rpcg2m_getcharm.h"
#include "network/mslink.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/12/14 21:18:54

#define CHARM_POINT 1
#define LEVEL		2
#define AMOUNT		3
#define PRIVILEGE	4

RPC_SERVER_IMPLEMETION(RpcC2G_PhotographEffect, PhotographEffectArg, PhotographEffect)

	void RpcC2G_PhotographEffect::OnCall(const PhotographEffectArg &roArg, PhotographEffect &roRes)
{
	Role * role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (role == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	RpcG2M_GetCharm* msg = RpcG2M_GetCharm::CreateRpc();
	msg->m_oArg.set_rpc_id(DelayRpc());
	msg->m_oArg.set_role_id(role->GetID());
	MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *msg);
}

void RpcC2G_PhotographEffect::OnDelayReplyRpc(const PhotographEffectArg &roArg, PhotographEffect &roRes, const CUserData &roUserData)
{
	Role * role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (role == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	GetCharmRes* res = (GetCharmRes*)roUserData.m_pUserPtr;
	if (NULL == res)
	{
		return ;
	}

	UINT32 charm = res->charm();
	roRes.set_charm(charm);
	std::set<UINT32> effects;
	effects.clear();
	vector<Sequence<uint, 2>>	condition;
	std::unordered_map<UINT32,vector<Sequence<uint, 2>>> effect_condition;

	// 归并获取特效的条件
	{	
		const PhotographEffectCfg& cfg = PhotoManager::Instance()->GetEffectCfg();
		auto cfg_it = cfg.Table.begin();
		auto cfg_it_end = cfg.Table.end();
		for (; cfg_it != cfg_it_end; ++cfg_it)
		{
			auto it = (*cfg_it)->Condition.begin();
			auto it_end = (*cfg_it)->Condition.end();

			UINT32 effect_id = (*cfg_it)->EffectID;
			Sequence<uint, 2>  sec ;
			sec.seq[0] = it->seq[0];
			sec.seq[1] = it->seq[1];

			auto exist_it = effect_condition.find(effect_id);
			if (exist_it == effect_condition.end())
			{
				condition.clear();			
				condition.push_back(sec);
				effect_condition.insert(std::make_pair(effect_id,condition));
			}
			else
			{
				exist_it->second.push_back(sec);
			}	
		}
	}
	// 逐一核对条件
	{	
		auto it_cond =  effect_condition.begin();
		auto it_cond_end = effect_condition.end();
		for (; it_cond != it_cond_end; ++it_cond)
		{
			bool effect = false;
			auto it = it_cond->second.begin();
			auto it_end = it_cond->second.end();
			for (; it != it_end; ++it)
			{
				// 魅力值
				if (it->seq[0] == CHARM_POINT)
				{
					if (charm < it->seq[1] )
					{
						effect = false;
						break;
					}
					else
					{
						effect = true;
					}
				} // 等级
				else if (it->seq[0] == LEVEL )
				{
					if ((UINT32)role->GetLevel() <  it->seq[1])
					{
						effect = false;
						break;
					}
					else
					{
						effect = true;
					}
				} 
				// 充值			
				else if (it->seq[0] == AMOUNT )
				{
					CPayV2Record* record = role->Get<CPayV2Record>();
					if (NULL != record)
					{
						if ( record->m_TotalPay / 100 < it->seq[1])
						{
							effect = false;
							break;
						}
						else
						{
							effect = true;
						}
					}
					else
					{
						effect = false;
						break;
					}
				} 
				else if (it->seq[0] == PRIVILEGE)
				{
					CPayV2Record* record = role->Get<CPayV2Record>();
					auto member_it = std::find_if(record->memberInfo.begin(), record->memberInfo.end(), EqualPayMemberID(it->seq[1]));
					if (member_it != record->memberInfo.end())
					{
						if (member_it->expiretime() > INT32(GameTime::GetTime()))
						{	
							effect = true;
						}
						else
						{
							effect = false;
							break;
						}
					}
					else
					{
						effect = false;
						break;
					}
				}				
			}
			if (effect)
			{
				effects.insert(it_cond->first);
			}
		}
	}
	auto it = effects.begin();
	auto it_end = effects.end();
	for (; it != it_end; ++it)
	{
		roRes.add_photograph_effect(*it);
	}	
}
