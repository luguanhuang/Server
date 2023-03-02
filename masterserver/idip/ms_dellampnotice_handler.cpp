#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "notice/noticemgr.h"

INT32 CMSIdipDelLampNoticeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	UINT32 uID = JsonUtil::GetJsonInt(roBodyJson, "RollingMsgId");
	if(!NoticeMgr::Instance()->DelIdipNotice(uID))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_LAMPID;
	}
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
