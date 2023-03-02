#include "pch.h"
#include "ms_commondef_handler.h"
#include "ranklist/worldbossranklist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "ranklist/ranklistmgr.h"
#include "util/jsonutil.h"

INT32 CMSIdipQueryBossBattleHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 uRoleId = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, uRoleId))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(uRoleId);
	if (NULL == pSum || pSum->GetAccount() != strOpenId)
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(uRoleId);

	INT32 iTotalDamage = 0;
	INT32 iRanking = 0;
	BossDamageRankList* ranklist = (BossDamageRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossDamageRank);
	if (ranklist)
	{
		iRanking = ranklist->GetRank(uRoleId);
		iTotalDamage = ranklist->GetDamage(uRoleId);
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_String("RoleName", UrlEncode(pSum->GetName()));
	oBuilder.AddMember_Int("Level", pSum->GetLevel());
	oBuilder.AddMember_Int("TotalDamage", iTotalDamage);
	oBuilder.AddMember_Int("BossBattleRanking", iRanking);

	roReply.m_strResult = oBuilder.GetString();

	return IDIP_SERVER_NONE;
}
