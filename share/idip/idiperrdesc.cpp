#include "pch.h"
#include "idip/idiperrdesc.h"
#include "define/platnotice_def.h"


static CIdipErrDesc g_soIdipErrDescObj;


CIdipErrDesc::CIdipErrDesc()
{
	Bind(IDIP_ERR_SUCCESS,				"success");
	Bind(IDIP_ERR_PLAYER_NOT_EXIST,		"account or role does not exist");
	Bind(IDIP_ERR_ROLE_OFFLINE,			"role is offline");
	Bind(IDIP_ERR_INVALID_PARA,			"invalid request para");
	Bind(IDIP_ERR_ALREADY_ACTIVE,		"already active");
	Bind(IDIP_ERR_SEND_MAIL,			"send mail fail");
	Bind(IDIP_ERR_INVALID_CMDID,		"invalid cmdid");
	Bind(IDIP_ERR_UNKNOWN,				"error unknown");
	Bind(IDIP_ERR_INVALID_REQ_NUM,		"invalid request number");
	Bind(IDIP_ERR_INVALID_ITEM_ID,		"invalid item id");
	Bind(IDIP_ERR_INVALID_OPEN_ID,		"invalid open id");
	Bind(IDIP_ERR_INVALID_TIME,			"invalid time format");
	Bind(IDIP_ERR_INVALID_LAMPID,		"invalid lampid");
	Bind(IDIP_ERR_NONE_LAMPNOTICE,		"none lampnotice");
	Bind(IDIP_ERR_INVALID_GUILDID,		"invalid guildid");
	Bind(IDIP_ERR_INVALID_SERVERID,		"invalid serverid");
	Bind(IDIP_ERR_MAX_UPDATE_NUM,		"over max update num");
	Bind(IDIP_ERR_NOTJOIN_GUILD,		"haven't joined a guild");
	Bind(IDIP_ERR_TIMEOUT,				"timeout");
	Bind(IDIP_ERR_NUM_NOT_ENOUGH,		"num not enough");
	Bind(IDIP_ERR_INVALID_ORDERID,		"invalid pay orderid");
	Bind(IDIP_ERR_KICKOUT,				"kickout fail");
	Bind(IDIP_ERR_NOLOGIN,				"no login server link");
	Bind(IDIP_ERR_NOMS,					"no ms server link");
	Bind(IDIP_ERR_SAME_LAMPID,			"exist same lampid");
}

CIdipErrDesc::~CIdipErrDesc()
{
}

const std::string& CIdipErrDesc::GetErrDesc(INT32 nErrCode) const
{
	std::map<INT32, std::string>::const_iterator it = m_oDescMap.find(nErrCode);
	if(it == m_oDescMap.end())
	{
		static std::string soEmpty("error unknown");
		return soEmpty;
	}

	return it->second;
}

void CIdipErrDesc::Bind(INT32 nErrCode, const char* pszErrDesc)
{
	m_oDescMap.insert(std::make_pair(nErrCode, pszErrDesc));
}

const std::string& GetIdipErrDesc(EIdipErrCode nErrCode)
{
	return g_soIdipErrDescObj.GetErrDesc(nErrCode);
}

UINT32 GetIdipAreaID(UINT32 AreaID)
{
	if (AreaID == 999)
	{
		return KKSG::GAME_APP_QQ;
	}
	if (AreaID == 1000)
	{
		return KKSG::GAME_APP_WECHAT;
	}
	if (AreaID == 1001)
	{
		return GAME_GUEST;
	}
	return AreaID;
}

bool GetPageNo(INT32 size, INT32 pageno, INT32 pagenum, INT32& start, INT32& end)
{
	start = (pageno - 1) * pagenum;
	if (start < 0 || (size && start >= size))
	{
		return false;
	}
	//UINT32 uTotalPageNo = (size + pagenum -1) / pagenum;

	end = size < (pageno * pagenum) ? size : pageno * pagenum;
	return true;
}
