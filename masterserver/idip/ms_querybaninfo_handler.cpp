#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "account/punishmgr.h"
#include "util.h"
#include "config.h"
#include "idutil.h"
#include "other/serverinfomgr.h"


INT32 CMSIdipQueryBanInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT32 uPageNo = JsonUtil::GetJsonUint(roBodyJson, "PageNo");

	std::vector<KKSG::PunishData> list;
	CPunishMgr::Instance()->GetPunishList(KKSG::PUNISH_USER_LOGIN, list);

	INT32 iStart = (uPageNo - 1) * MAX_BANINFOLIST_NUM;
	if (iStart < 0 || (list.size() && iStart >= (INT32)list.size()))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	JsonBuilder oBuilder;
	rapidjson::Value oRoleList(rapidjson::kArrayType);

	UINT32 uTotalPageNo = (list.size() + MAX_BANINFOLIST_NUM -1) / MAX_BANINFOLIST_NUM;
	UINT32 uBanInfoListCount = 0;

	INT32 iEnd = list.size() < (uPageNo * MAX_BANINFOLIST_NUM) ? list.size() : uPageNo * MAX_BANINFOLIST_NUM;
	for (INT32 i = iStart; i < iEnd; ++ i)
	{
		rapidjson::Value oRole(rapidjson::kObjectType);
		UINT32 Partition = GetServerIDByRoleID(list[i].roleid());
		oRole.AddMember("RoleId", ToString(list[i].roleid()), oBuilder.Allocator());
		oRole.AddMember("RoleName", UrlEncode(CRoleSummaryMgr::Instance()->GetRoleName(list[i].roleid())), oBuilder.Allocator());
		oRole.AddMember("Partition", Partition, oBuilder.Allocator());
		oRole.AddMember("PartitionName", UrlEncode(ZServerInfoMgr::Instance()->GetServerName(Partition)), oBuilder.Allocator());
		oRole.AddMember("OpenId", list[i].account(), oBuilder.Allocator());
		oRole.AddMember("BanTime", list[i].bantime(), oBuilder.Allocator());
		oRole.AddMember("BanEndTime", list[i].bantime()+list[i].punishdate(), oBuilder.Allocator());
		oRole.AddMember("BanReason", UrlEncode(list[i].reason()), oBuilder.Allocator());
		oRoleList.PushBack(oRole, oBuilder.Allocator());
		uBanInfoListCount ++;
	}
	oBuilder.AddMember_Int("TotalCount", list.size());
	oBuilder.AddMember_Int("BanInfoList_count", uBanInfoListCount);
	oBuilder.AddMember_Value("BanInfoList", oRoleList);
	oBuilder.AddMember_Uint("TotalPageNo", uTotalPageNo);
	
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}

