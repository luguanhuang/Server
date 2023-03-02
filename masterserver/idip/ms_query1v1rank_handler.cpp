#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/pkranklist.h"
#include "idutil.h"

INT32 CMSIdipQuery1v1RankReqHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	PkRealTimeRankList* pRankList = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(KKSG::PkRealTimeRank);
	if(NULL == pRankList)
	{
		roReply.m_nErrCode = IDIP_ERR_UNKNOWN;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	std::vector<PkRankInfo*> vRankList;
	pRankList->ASGetTopInfo(100, vRankList);
	JsonBuilder oBuilder;
	rapidjson::Value oRoleList(rapidjson::kArrayType);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(roReply.m_nErrCode));
	for (UINT32 i = 0; i < vRankList.size(); i ++)
	{
		if (NULL == vRankList[i])
		{
			continue;
		}
		CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(vRankList[i]->GetUniqueId());
		if (NULL == pSum)
		{
			continue;
		}
		rapidjson::Value oRole(rapidjson::kObjectType);
		oRole.AddMember("RoleId", ToString(vRankList[i]->GetUniqueId()), oBuilder.Allocator());
		oRole.AddMember("RoleName", UrlEncode(pSum->GetName()), oBuilder.Allocator());
		oRole.AddMember("Partition", GetServerIDByRoleID(vRankList[i]->GetUniqueId()), oBuilder.Allocator());
		oRole.AddMember("Rank", i+1, oBuilder.Allocator());
		oRole.AddMember("OpenId", pSum->GetAccount(), oBuilder.Allocator());
		oRole.AddMember("Occupation", pSum->GetProfession(), oBuilder.Allocator());
		oRole.AddMember("Score", vRankList[i]->m_point, oBuilder.Allocator());
		oRoleList.PushBack(oRole, oBuilder.Allocator());
	}
	oBuilder.AddMember_Uint("RankList_count", oRoleList.Size());
	oBuilder.AddMember_Value("RankList", oRoleList);
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;

}
