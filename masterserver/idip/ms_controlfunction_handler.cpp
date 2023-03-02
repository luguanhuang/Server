#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "table/OpenSystemMgr.h"
#include "misc/ptcm2g_controlfunctionntf.h"
#include "network/gslink.h"

INT32 CMSIdipControlFunctionHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	INT32 id = JsonUtil::GetJsonInt(roBodyJson, "FunctionId");
	UINT32 isopen = JsonUtil::GetJsonUint(roBodyJson, "IsOpen");
	if (!id || (isopen != 0 && isopen != 1))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	bool isFlag = isopen==0?true:false;
	OpenSystemMgr::Instance()->IdipOpSystem(id, isFlag);
	PtcM2G_ControlFunctionNtf ptc;
	ptc.m_Data.set_systemid(id);
	ptc.m_Data.set_isopen(isFlag);
	GSLink::Instance()->SendToAllLine(ptc);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);

	return IDIP_SERVER_NONE;
}
