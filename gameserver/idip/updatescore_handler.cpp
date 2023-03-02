#include "pch.h"
#include "commondef_handler.h"
#include "util/jsonutil.h"
#include "gamelogic/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "unit/rolemanager.h"
#include "utility/tlogger.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/bag.h"
#include "gamelogic/arenarecord.h"


static void SetItemCount(Role* pRole, INT32 itemID, INT32 Count)
{
	INT32 iTempCount = 0;
	INT64 lCurCount = XItem::IsVirtualItem(itemID)?pRole->Get<Bag>()->CountVirtualItem(itemID):pRole->Get<Bag>()->CountItemInBag(itemID);
	iTempCount = (INT32)abs(lCurCount-Count);
	if (Count > (INT32)lCurCount)//add
	{
		BagGiveItemTransition give(pRole);
		give.SetReason(ItemFlow_IDIP, ItemFlow_IDIP_Oper);
		give.GiveItem(itemID, iTempCount);
		give.NotifyClient();
	}
	else	//reduce
	{
		BagTakeItemTransition take(pRole);
		take.SetReason(ItemFlow_IDIP, ItemFlow_IDIP_Oper);
		take.TakeItem(itemID, iTempCount);
		take.NotifyClient();
	}

}
bool CIdipUpdateScoreHandler::Process(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT32 uScoreType = JsonUtil::GetJsonUint(roBodyJson, "ScoreType");
	UINT64 qwRoleID = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");

	Role* pRole = RoleManager::Instance()->FindByRoleIDOrNickID(qwRoleID);
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return true;
	}
	INT32 iNumber = 0;
	if (GetCmd() == IDIP_AQ_DO_SET_SCORE_REQ)
	{
		iNumber	= JsonUtil::GetJsonInt64(roBodyJson, "Num");//ms上已经判断过是>0
	}
	if (uScoreType == SCORE_TYPE_DRAGON_COIN || uScoreType == SCORE_TYPE_ALL)
	{
		SetItemCount(pRole, DRAGON_COIN, iNumber);
	}
	if (uScoreType == SCORE_TYPE_ARENA_COIN || uScoreType == SCORE_TYPE_ALL)
	{
		SetItemCount(pRole, ARENA_COIN, iNumber);
	}
	if (uScoreType == SCORE_TYPE_GODDNESS_TEARS || uScoreType == SCORE_TYPE_ALL)
	{
		SetItemCount(pRole, GODDNESS_TEARS_ID, iNumber);
	}
	if (uScoreType == SCORE_TYPE_GUILD_CONTRIBUTE || uScoreType == SCORE_TYPE_ALL)
	{
		SetItemCount(pRole, GUILD_CONTRIBUTE, iNumber);
	}

	if (uScoreType == SCORE_TYPE_PKPOINT || uScoreType == SCORE_TYPE_ALL)
	{
		pRole->Get<CPkRecord>()->SetPoint((UINT32)iNumber);
	}

	if (uScoreType == SCORE_TYPE_ARENA_POINT || uScoreType == SCORE_TYPE_ALL)
	{
		pRole->Get<CArenaRecord>()->SetPoint((UINT32)iNumber);
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}
