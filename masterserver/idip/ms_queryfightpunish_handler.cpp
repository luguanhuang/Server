#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "report/reportmgr.h"
#include "idutil.h"
#include "util/gametime.h"


INT32 CMSIdipQueryFightPunishHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	std::vector<UINT64> vRoleList;
	CRoleSummaryMgr::Instance()->GetRoleIdByAccount(strOpenId, vRoleList);
	JsonBuilder oBuilder;
	rapidjson::Value oRoleList(rapidjson::kArrayType);
	for (UINT32 i = 0; i < vRoleList.size(); i ++)
	{
		UINT32 time = ReportMgr::Instance()->GetRoleBanTime(vRoleList[i], KKSG::SCENE_HEROBATTLE);
		if (time)
		{
			rapidjson::Value oRole(rapidjson::kObjectType);
			oRole.AddMember("RoleId", ToString(vRoleList[i]), oBuilder.Allocator());
			oRole.AddMember("RoleName", UrlEncode(CRoleSummaryMgr::Instance()->GetRoleName(vRoleList[i])), oBuilder.Allocator());
			oRole.AddMember("Partition", GetServerIDByRoleID(vRoleList[i]), oBuilder.Allocator());
			oRole.AddMember("OpenId", strOpenId, oBuilder.Allocator());
			oRole.AddMember("Time", time, oBuilder.Allocator());
			INT32 endtime = (INT32)GameTime::GetTime() + time;
			oRole.AddMember("BanEndTime", endtime, oBuilder.Allocator());
			oRoleList.PushBack(oRole, oBuilder.Allocator());
		}
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("RetMsg", GetIdipErrDesc(roReply.m_nErrCode));
	oBuilder.AddMember_Uint("PunishList_count", oRoleList.Size());
	oBuilder.AddMember_Value("PunishList", oRoleList);
	oBuilder.AddMember_Int("TotalCount", ReportMgr::Instance()->GetTotalNum(KKSG::SCENE_HEROBATTLE));
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}
