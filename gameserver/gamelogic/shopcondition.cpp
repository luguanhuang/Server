#include "pch.h"
#include "shopcondition.h"
#include "shop.h"
#include "unit/role.h"
#include "shoprecord.h"
#include "payv2Record.h"
#include "arenamgr.h"
#include "pkrecord.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "pb/project.pb.h"

UINT32 LevelShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	return (UINT32)role->GetLevel() >= condition->level ? KKSG::ERR_SUCCESS : KKSG::ERR_SHOP_LEVELLIMIT;
}

UINT32 PPTShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	return (UINT32)role->GetAttr(TOTAL_POWERPOINT) >= condition->ppt ? KKSG::ERR_SUCCESS : KKSG::ERR_SHOP_PPTLIMIT;
}

UINT32 DailyCountShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	if (0 == condition->dailycount)
	{
		return KKSG::ERR_SUCCESS;
	}
	CShopRecord* record = role->Get<CShopRecord>();
	record->Update();
	UINT32 count = record->GetDailyBuyCount(type, uid);
	if (count + param.count > condition->dailycount)
	{
		return KKSG::ERR_SHOP_WEEKCOUNTLIMIT;
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 WeekCountShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	if (0 == condition->weekcount)
	{
		return KKSG::ERR_SUCCESS;
	}
	CShopRecord* record = role->Get<CShopRecord>();
	record->Update();
	UINT32 count = record->GetWeekBuyCount(type, uid);
	if (count + param.count > condition->weekcount)
	{
		return KKSG::ERR_SHOP_DAILYCOUNTLIMIT;
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 CountShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	if (0 == condition->count)
	{
		return KKSG::ERR_SUCCESS;
	}
	CShopRecord* record = role->Get<CShopRecord>();
	record->Update();
	UINT32 count = record->GetBuyCount(type, uid);
	if (count + param.count > condition->count)
	{
		return KKSG::ERR_SHOP_COUNTLIMIT;
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 VIPShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	if (0 == condition->vip)
	{
		return KKSG::ERR_SUCCESS;
	}
	CPayV2Record* record = role->Get<CPayV2Record>();
	if (record->GetVipLevel() < (int)condition->vip)
	{
		return KKSG::ERR_SHOP_VIPLIMIT;
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 ArenaShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	if (0 == condition->arena)
	{
		return KKSG::ERR_SUCCESS;
	}
#if ARENA
	UINT32 rank = CArenaMgr::Instance()->GetRank(role->GetID());
	if (rank > condition->arena)
	{
		return KKSG::ERR_SHOP_ARENALIMIT;
	}
#endif
	return KKSG::ERR_SUCCESS;
}

UINT32 PKShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	if (0 == condition->pk)
	{
		return KKSG::ERR_SUCCESS;
	}
	CPkRecord* record = role->Get<CPkRecord>();
	if (record->GetPoint() < condition->pk)
	{
		return KKSG::ERR_SHOP_PKLIMIT;
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 GuildShopCondition::Check(Role* role, const ShopConditionParam* condition, UINT32 type, UINT32 uid, const ShopRoleParam& param)
{
	if (0 == condition->guild)
	{
		return KKSG::ERR_SUCCESS;
	}
	
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (NULL == guild)
	{
		return KKSG::ERR_SHOP_GUILDLIMIT;
	}
	if (guild->GetGuildLvl() < (int)condition->level)
	{
		return KKSG::ERR_SHOP_GUILDLIMIT;
	}
	return KKSG::ERR_SUCCESS;
}
