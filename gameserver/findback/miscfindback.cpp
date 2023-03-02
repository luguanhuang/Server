#include "pch.h"
#include "findbackrecord.h"
#include "itemfindback.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include "findback/ptcg2c_expfindbackntf.h"
#include <time.h>
#include "findbackconfig.h"
#include "foreach.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/towerrecord.h"
#include "gamelogic/towerconfig.h"
#include "unit/systeminfo.h"
#include "findback/ptcg2c_itemfindbackntf.h"
#include "table/OpenSystemMgr.h"
#include "gamelogic/superriskrecord.h"
#include "gamelogic/levelsealMgr.h"
#include "util/gametime.h"
#include "define/systemiddef.h"
#include "miscfindback.h"


OneDayGuildInfo::OneDayGuildInfo(BaseFindBackInfo* baseFindBack, int day):OneDayTypeInfo(baseFindBack, baseFindBack->m_type, day)
{
}

bool OneDayGuildInfo::Convert2KKSGBackInfo(Role* role, KKSG::ItemFindBackInfo2Client& temp)
{
	if (!role->getGuildId())
	{
		return false;
	}

	return OneDayTypeInfo::Convert2KKSGBackInfo(role, temp);
}

GuildFindBackInfo::GuildFindBackInfo(int type, CItemFindBackInfo* info):BaseFindBackInfo(type, info)
{
}

OneDayTypeInfo* GuildFindBackInfo::CreateOneDayBackInfo(int day)
{
	return new OneDayGuildInfo(this, day);
}

OneDayFatigueBuyInfo::OneDayFatigueBuyInfo(BaseFindBackInfo* baseFindBackInfo, int day) : OneDayTypeInfo(baseFindBackInfo, KKSG::FATIGUE_BUY, day)
{
}

bool OneDayFatigueBuyInfo::ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount)
{
	bool result = OneDayTypeInfo::ConvertSingleSubtype(role, temp, leftCount);
	if (result)
	{
		temp.clear_goldcoinfindbackitems();
	}

	return result;
}

FatigueBuyFindBackInfo::FatigueBuyFindBackInfo(CItemFindBackInfo* info):BaseFindBackInfo(KKSG::FATIGUE_BUY, info)
{
}

OneDayTypeInfo* FatigueBuyFindBackInfo::CreateOneDayBackInfo(int day)
{
	return new OneDayFatigueBuyInfo(this, day);
}
