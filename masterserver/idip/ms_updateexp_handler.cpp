#include "pch.h"
#include "ms_updateexp_handler.h"
#include "util/jsonutil.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rpcm2d_readroledata.h"
#include "network/dblink.h"
#include "pb/project.pb.h"
#include "role/rpcm2d_mssaveroledata.h"
#include "network/dblink.h"
#include "util/dbutil.h"
#include "table/PlayerLevelManager.h"
#include "ms_commondef_handler.h"

IDIP_REPLY_FROMGS_IMPLEMETION(UpdateExp)
bool CMSIdipUpdateExpHandler::ReadDataQequest()
{
	RpcM2D_ReadRoleData* rpc = RpcM2D_ReadRoleData::CreateRpc();
	rpc->m_oArg.set_index(GetID());
	rpc->m_oArg.set_roleid(GetRoleID());
	rpc->m_oArg.set_param(ReadAndModify);
	rpc->m_oArg.set_readtype(KKSG::ROLE_DATA_READ_IDIP);
	DBLink::Instance()->SendTo(*rpc);
	return true;
}
INT32 CMSIdipUpdateExpHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	m_nNum	= JsonUtil::GetJsonInt64(roBodyJson, "Num");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	szMailTitle = JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	szMailContent = JsonUtil::GetJsonString(roBodyJson, "MailContent");

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	if(m_nNum == 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (m_nNum > 0)//添加的话，直接邮件
	{
		roReply.m_nErrCode = IDIP_ERR_SUCCESS;
		std::vector<ItemDesc> items;
		ItemDesc pdesc(EXP, (INT32)m_nNum);
		items.push_back(pdesc);

		MailData oData(szMailTitle, szMailContent, items, MAIL_EXPIRE_TIME);
		oData.SetReason(ItemFlow_IDIP_Oper);
		if (!CMailMgr::Instance()->SendMail(roleid, oData))
		{
			roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
		}
		else
		{
			//加上日志
			IdipLog();
		}
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(GetRoleID());
	if (NULL == pRole)//不在线ms读取数据扣除;
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
//修改数据
bool CMSIdipUpdateExpHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	UINT64 nNumber = m_nNum>0?m_nNum:-1*m_nNum;
	if (pInfo->brief().exp() >= nNumber)
	{
		pInfo->mutable_brief()->set_exp(pInfo->brief().exp() - nNumber);
	}
	else
	{
		while (pInfo->brief().level() > 1 && pInfo->brief().exp() < nNumber)
		{
			PlayerLevelTable::RowData *row  = PlayerLevelManager::Instance()->GetLevelInfo(pInfo->brief().level());
			if (NULL == row)
			{
				return false;
			}
			pInfo->mutable_brief()->set_maxexp(row->Exp);
			pInfo->mutable_brief()->set_exp(pInfo->brief().exp() + row->Exp);
			if (nNumber > pInfo->brief().exp())
			{
				nNumber -= pInfo->brief().exp();
				pInfo->mutable_brief()->set_exp(0);
			}
			else
			{
				pInfo->mutable_brief()->set_exp(pInfo->brief().exp() - nNumber);
				nNumber = 0;
			}
			pInfo->mutable_brief()->set_level(pInfo->brief().level() - 1);
		}
	}
	return true;
}
//写数据
bool CMSIdipUpdateExpHandler::SaveDataToDB(const CUserData& roUserData)
{

	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;

	KKSG::RoleBriefInfo brief;
	brief.set_roleid(pInfo->brief().roleid());
	brief.set_name(pInfo->brief().name());
	brief.set_type((KKSG::RoleType)pInfo->brief().type());
	brief.set_level(pInfo->brief().level());
	RpcM2D_MsSaveRoleData* rpc = RpcM2D_MsSaveRoleData::CreateRpc();
	rpc->m_oArg.set_account(pInfo->brief().accountid());
	rpc->m_oArg.set_roleid(pInfo->brief().roleid());
	rpc->m_oArg.set_slot(MapProfessionToSlot(pInfo->brief().type()));
	rpc->m_oArg.set_param(GetID());

	for (int i = 0; i < pInfo->fashionrecord().bodyfashion_size(); i ++)
	{
		brief.add_fashion(pInfo->fashionrecord().bodyfashion(i).itemid());
	}

	CRoleDataFields oFields(pInfo);
	if(!CRoleDataSaveUtil::SaveAll(oFields, rpc->m_oArg))
	{
		LogError("Pack role data failed, roleid=%llu", pInfo->brief().roleid());
		delete rpc;
		return false;
	}
	DBLink::Instance()->SendTo(*rpc);
	return true;
}
bool CMSIdipUpdateExpHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		EIdipErrCode nErrCode = IDIP_ERR_SUCCESS;
		std::string str;
		CIdipReply oReply(str);
		oReply.m_nErrCode = nErrCode;
		oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);

		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);

		//ms,gs上扣除加上日志
		IdipLog();
	}
	return true;
}
void CMSIdipUpdateExpHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	TIdipMoneyLog oLog("IdipUpdateExpFlow");
	oLog.m_uRoleId = GetRoleID();
	oLog.m_nNum = m_nNum;
	oLog.szOpenId = szOpenId;
	oLog.Do(doc["head"], doc["body"]);
}

