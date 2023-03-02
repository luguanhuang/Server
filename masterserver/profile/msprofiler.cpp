#include "pch.h"
#include "msprofiler.h"
#include "scene/scenemanager.h"
#include "role/rolesummarymgr.h"
#include "event/eventmgr.h"
#include "pay/paymgr.h"
#include "herobattle/herobattlemgr.h"
#include "auction/auctionmgr.h"
#include "guildauct/guildauctmgr.h"

INSTANCE_SINGLETON(CMsProfiler)


CMsProfiler::CMsProfiler()
{
}

CMsProfiler::~CMsProfiler()
{
}

bool CMsProfiler::Init()
{
	StartTimer();
	SetFileName("masterserver");
	return true;
}

void CMsProfiler::Uninit()
{
	StopTimer();
}

void CMsProfiler::DoProfile(FILE* fp)
{
	fprintf(fp, "Scene count: %u\n", CSceneManager::Instance()->GetSceneNum());
	fprintf(fp, "RoleSummary count: %u\n", CRoleSummaryMgr::Instance()->GetCount());
	fprintf(fp, "RoleSummary queue count: %u\n", CRoleSummaryMgr::Instance()->GetQueueCount());
	fprintf(fp, "EventMgr count: %u\n",EventMgr::Instance()->GetSize());
	fprintf(fp, "Midas Request count: %u\n", CPayMgr::Instance()->GetRequestCount());

	fprintf(fp, "Garden Scene count: %u\n", CSceneManager::Instance()->GetSceneCount(FAMILYGARDEN_MAP_ID));
	fprintf(fp, "Guild Scene count: %u\n", CSceneManager::Instance()->GetSceneCount(GUILD_MAP_ID));

	fprintf(fp, "HeroBattleMatchNum: %u\n", HeroBattleMgr::Instance()->GetHeroBattleMatchUnitNum());

	fprintf(fp, "Auction ItemNum: %d,ShoppingCartRoleNum: %d,AuctionSCAuctUnitNum: %d\n",
		AuctionMgr::Instance()->GetAllSaleCount(),
		AuctionMgr::Instance()->GetUseCartCount(),
		AuctionMgr::Instance()->GetAllSeeUnitCount());

	fprintf(fp, "GuildAuct ItemNum: %u\n",GuildAuctMgr::Instance()->GetItemNum());
}
