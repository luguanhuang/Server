#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "notice/noticemgr.h"
#include "notice/notice.h"

INT32 CMSIdipSendNoticeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT32 uType = JsonUtil::GetJsonUint(roBodyJson, "Type");
	std::string szContent = JsonUtil::GetJsonString(roBodyJson, "NoticeContent");
	switch (uType)
	{

	case GAME_NOTICE_TYPE_SYSTEM:
		NoticeMgr::Instance()->SendNotice(KKSG::SystemChannel, 0, szContent);
		break;

	case GAME_NOTICE_TYPE_WORLD:
		NoticeMgr::Instance()->SendNotice(KKSG::WorldChannel, 0, szContent);
		break;
	case GAME_NOTICE_TYPE_LAMP:
		NoticeMgr::Instance()->SendNotice(KKSG::LampShortChannel, 0, szContent);
		break;
	case GAME_NOTICE_TYPE_ALL:
		NoticeMgr::Instance()->SendNotice(KKSG::SystemChannel, 0, szContent);
		NoticeMgr::Instance()->SendNotice(KKSG::WorldChannel, 0, szContent);
		NoticeMgr::Instance()->SendNotice(KKSG::LampShortChannel, 0, szContent);
		break;
	default:
		{
			roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		break;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
