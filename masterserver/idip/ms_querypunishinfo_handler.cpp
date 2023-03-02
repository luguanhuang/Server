#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "account/punishmgr.h"
#include "login/rpcm2d_readaccountdata.h"
#include "account/punishmgr.h"
#include "util.h"
#include "config.h"
#include "idutil.h"
#include "other/serverinfomgr.h"


INT32 CMSIdipQueryUserPunishInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	std::vector<UINT64> m_vRoleList;
	CRoleSummaryMgr::Instance()->GetRoleIdByAccount(szOpenId, m_vRoleList);
	if (!m_vRoleList.size())
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	JsonBuilder oBuilder;
	INT32 nRoleCount = 0;
	rapidjson::Value oRoleList(rapidjson::kArrayType);
	if (GetCmd() == IDIP_AQ_QUERY_USER_INFO_REQ)
	{
		oBuilder.AddMember_Int("Result", IDIP_ERR_SUCCESS);
		for (UINT32 i = 0; i < m_vRoleList.size(); i ++)
		{
			rapidjson::Value oRole(rapidjson::kObjectType);
			oRole.AddMember("RoleId", ToString(m_vRoleList[i]), oBuilder.Allocator());
			oRole.AddMember("RoleName", UrlEncode(CRoleSummaryMgr::Instance()->GetRoleName(m_vRoleList[i])), oBuilder.Allocator());
			oRole.AddMember("BanStatus", CPunishMgr::Instance()->IsPunish(m_vRoleList[i], KKSG::PUNISH_USER_LOGIN)?0:1, oBuilder.Allocator());
			oRoleList.PushBack(oRole, oBuilder.Allocator());
		}

		oBuilder.AddMember_Int("RoleList1_count", m_vRoleList.size());
		oBuilder.AddMember_Value("RoleList1", oRoleList);
		oBuilder.AddMember_Int("TotalCount", m_vRoleList.size());
	}
	else
	{
		KKSG::PunishData oData;
		for (UINT32 i = 0; i < m_vRoleList.size(); i ++)
		{
			if (CPunishMgr::Instance()->GetPunishInfo(m_vRoleList[i], KKSG::PUNISH_USER_LOGIN, oData))
			{
				rapidjson::Value oRole(rapidjson::kObjectType);
				UINT32 Partition = GetServerIDByRoleID(m_vRoleList[i]);
				oRole.AddMember("RoleId", ToString(m_vRoleList[i]), oBuilder.Allocator());
				oRole.AddMember("RoleName", UrlEncode(CRoleSummaryMgr::Instance()->GetRoleName(m_vRoleList[i])), oBuilder.Allocator());
				oRole.AddMember("Partition", Partition, oBuilder.Allocator());
				oRole.AddMember("PartitionName", UrlEncode(ZServerInfoMgr::Instance()->GetServerName(Partition)), oBuilder.Allocator());
				oRole.AddMember("OpenId", szOpenId, oBuilder.Allocator());
				oRole.AddMember("BanTime", oData.bantime(), oBuilder.Allocator());
				oRole.AddMember("BanEndTime", oData.bantime()+oData.punishdate(), oBuilder.Allocator());
				oRole.AddMember("BanReason", UrlEncode(oData.reason()), oBuilder.Allocator());
				oRoleList.PushBack(oRole, oBuilder.Allocator());
				nRoleCount ++;
			}
		}
		oBuilder.AddMember_Int("TotalCount", nRoleCount);
		oBuilder.AddMember_Int("BanInfoList_count", nRoleCount);
		oBuilder.AddMember_Value("BanInfoList", oRoleList);
	}
	roReply.m_strResult = oBuilder.GetString();
	return IDIP_SERVER_NONE;
}

