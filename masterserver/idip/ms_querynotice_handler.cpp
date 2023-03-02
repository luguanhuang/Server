#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "idip/platnotice.h"

INT32 CMSIdipQueryNoticeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	m_AreaID = JsonUtil::GetJsonUint(roBodyJson, "AreaId");
	m_PlatID = JsonUtil::GetJsonUint(roBodyJson, "PlatId");
	m_PageNo = JsonUtil::GetJsonUint(roBodyJson, "PageNo");
	m_AreaID = GetIdipAreaID(m_AreaID);
	m_Start = (m_PageNo - 1) * MAX_NOTICELIST_NUM;
	if (m_Start < 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	return IDIP_SERVER_LOGIN;
}
bool CMSIdipQueryNoticeHandler::OnAsyncReply(const CUserData& roUserData)
{
	if (roUserData.m_dwUserData == 0 && roUserData.m_pUserPtr)
	{
		KKSG::GetPlatNoticeRes* pInfo = (KKSG::GetPlatNoticeRes*)roUserData.m_pUserPtr;
		JsonBuilder Builder;

		CPlatNoticeInfoMap list = CPlatNoticeMgr::Instance()->GetNoticeList();
		rapidjson::Value oList(rapidjson::kArrayType);
		std::vector<KKSG::PlatNotice> vList;//用于后面分页;
		for(auto i = list.begin(); i != list.end(); i ++)
		{
			if (i->second.isopen() && m_AreaID == i->second.areaid() && m_PlatID == i->second.platid())
			{
				vList.push_back(i->second);
			}
		}
		for (INT32 i = 0; i < pInfo->noticelist_size(); i ++)
		{
			if (pInfo->noticelist(i).isopen() && m_AreaID == pInfo->noticelist(i).areaid() && m_PlatID == pInfo->noticelist(i).platid())
			{
				vList.push_back(pInfo->noticelist(i));
			}
		}
		UINT32 uTotalPageNo = (vList.size() + MAX_NOTICELIST_NUM - 1)/MAX_NOTICELIST_NUM;
		std::string str;
		EIdipErrCode errCode = IDIP_ERR_SUCCESS;
		if (vList.size() && m_Start >= vList.size())
		{
			errCode = IDIP_ERR_INVALID_PARA;
			str = PackErrResponse(errCode);
		}
		else
		{
			UINT32 uEnd = vList.size() < (m_PageNo * MAX_NOTICELIST_NUM) ? vList.size() : m_PageNo * MAX_NOTICELIST_NUM;
			for (UINT32 i = m_Start; i < uEnd; i ++)
			{
				rapidjson::Value notice(rapidjson::kObjectType);
				notice.AddMember("NoticeId", vList[i].noticeid(), Builder.Allocator());
				notice.AddMember("NoticeContent", UrlEncode(vList[i].content()), Builder.Allocator());
				notice.AddMember("NoticeType", vList[i].type(), Builder.Allocator());
				oList.PushBack(notice, Builder.Allocator());
			}
			Builder.AddMember_Int("Result", errCode);
			Builder.AddMember_String("RetMsg", GetIdipErrDesc(errCode).c_str());
			Builder.AddMember_Uint("NoticeList_count", oList.Size());
			Builder.AddMember_Value("NoticeList", oList);
			Builder.AddMember_Uint("TotalPageNo", uTotalPageNo);
			str = Builder.GetString();
		}

		CIdipReply oReply(str);
		CUserData data;
		oReply.m_nErrCode = errCode;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	else
	{
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	return true;
}