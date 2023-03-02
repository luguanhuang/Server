#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/OpenSystemMgr.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(DoIdipRed)
INT32 CMSIdipDoIdipRedHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	m_uItemID = JsonUtil::GetJsonUint(roBodyJson, "SystemId");
	auto it = OpenSystemMgr::Instance()->GetSystemRowData(m_uItemID);
	if (NULL == it || OpenSystemMgr::Instance()->IsClosedSystem(m_uItemID))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == pRole)//不在线ms读取数据扣除;
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
bool CMSIdipDoIdipRedHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	bool isFind = false;
	for (INT32 i = 0; i < pInfo->idiprecord().hintdata_size(); i ++)
	{
		if (pInfo->idiprecord().hintdata(i).systemid() == m_uItemID)
		{
			pInfo->mutable_idiprecord()->mutable_hintdata(i)->set_ishint(true);
			isFind = true;
			break;
		}
	}
	if (!isFind)
	{
		KKSG::IdipHintData* pHiniData = pInfo->mutable_idiprecord()->add_hintdata();
		pHiniData->set_systemid(m_uItemID);
		pHiniData->set_ishint(true);
	}
	return true;
}
void CMSIdipDoIdipRedHandler::IdipLog()
{

}
