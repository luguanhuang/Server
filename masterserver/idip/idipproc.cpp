#include "pch.h"
#include "idipproc.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "util.h"
#include "gm/rpcm2g_gshttpcommandnew.h"
#include "ms_updateexp_handler.h"
#include "ms_commondef_handler.h"
#include "pb/project.pb.h"
#include "util/jsonutil.h"
#include "account/punishmgr.h"


INSTANCE_SINGLETON(CIdipProcess)


	CIdipProcess::CIdipProcess()
	:m_dwNextID(0)
{
}

CIdipProcess::~CIdipProcess()
{
}
bool CIdipProcess::Init()
{
	//idip需求;
	Register(new CMSIdipUpdateExpHandler());
	Register(new CMSIdipUpdateGoldHandler());
	Register(new CMSIdipUpdateFatigueHandler());
	Register(new CMSIdipUpdateDragonCoinHandler());
	Register(new CMSIdipUpdateDiamondHandler());
	Register(new CMSIdipUpdateArenaCoinHandler());
	Register(new CMSIdipQueryRoleInfoXHandler());
	Register(new CMSIdipQueryBagHandler());
	Register(new CMSIdipQueryEmblemHandler());
	Register(new CMSIdipQueryItemInfoHandler());
	Register(new CMSIdipDelItemHandler());
	Register(new CMSIdipQueryUserInfoHandler());
	Register(new CMSIdipQueryReverseRoleHandler());
	Register(new CMSIdipSendItemHandler(0, IDIP_DO_SEND_ITEM_REQ));
	Register(new CMSIdipQueryLampNoticeHandler());
	Register(new CMSIdipSendLampNoticeHandler());
	Register(new CMSIdipDelLampNoticeHandler());
	Register(new CMSIdipPunishHandler(0, IDIP_DO_BAN_USR_REQ));
	Register(new CMSIdipPunishHandler(0, IDIP_DO_MASK_CHAT_REQ));
	Register(new CMSIdipUnPunishHandler());
	Register(new CMSIdipQueryGuildInfoHandler());
	Register(new CMSIdipQueryUserPunishInfoHandler(0, IDIP_QUERY_INDIVIDUAL_BAN_INFO_REQ));
	Register(new CMSIdipQueryBanInfoHandler());
	Register(new CMSIdipQueryGuildInfoXHandler());
	Register(new CMSIdipQueryGuildMemberInfoHandler());
	Register(new CMSIdipQueryNoticeHandler());
	Register(new CMSIdipUnMaskHandler());
	Register(new CMSIdipQuerySpriteInfoHandler());
	Register(new CMSIdipSendMailHandler());
	Register(new CMSIdipNameActHandler());
	Register(new CMSIdipControlFunctionHandler());
	Register(new CMSIdipQueryGuildTaskHandler());
	Register(new CMSIdipDoXinyueRedHandler());
	Register(new CMSIdipQueryWeekUserInfoHandler());
	Register(new CMSIdipSendPersonMailHandler());
	Register(new CMSIdipQueryFashionHandler());
	Register(new CMSIdipQueryGuildInfoBackHandler());
	Register(new CMSIdipQueryHorseInfoHandler());
	Register(new CMSIdipDoDelItemXHandler());
	Register(new CMSIdipUpdateGuildNameHandler());
	Register(new CMSIdipUpdateNameHandler());
	Register(new CMSIdipXinYueRPayHandler());
	Register(new CMSIdipItemChangeHandler());
	Register(new CMSIdipUpdateGuildInfoHandler(0, IDIP_DO_UPDATE_PRESTIGE_REQ));
	Register(new CMSIdipUpdateGuildInfoHandler(0, IDIP_DO_UPDATE_GUILD_EXP_REQ));
	Register(new CMSIdipQueryMailHandler());
	Register(new CMSIdipQueryItemIdToNameHandler());
	Register(new CMSIdipQueryFightPunishHandler());
	Register(new CMSIdipClearFightPunishHandler());
	Register(new CMSIdipDoActiveAchievementHandler());
	Register(new CMSIdipDoIdipRedHandler());
	Register(new CMSIdipQuery1v1RankReqHandler());
	Register(new CMSIdipWhiteListHandler(0, IDIP_DO_ADD_DEL_WHITE_LIST_REQ));
	Register(new CMSIdipWhiteListHandler(0, IDIP_QUERY_WHITE_LIST_REQ));
	Register(new CMSIdipDiamondConsumeToGameHandler());
	Register(new CMSIdipQueryFashionComposeHandler());
	
	//idip管理端需求;
	Register(new CMSIdipUpdateDiamondHandler(0, IDIP_DO_GM_UPDATE_DIAMOND_REQ));
	Register(new CMSIdipUpdateDragonCoinHandler(0, IDIP_DO_GM_UPDATE_DRAGON_GOLD_REQ));
	Register(new CMSIdipSendItemHandler(0, IDIP_DO_GM_SEND_ITEM_REQ));
	Register(new CMSIdipUpdateFatigueHandler(0, IDIP_DO_GM_UPDATE_PHYSICAL_REQ));
	Register(new CMSIdipUpdateGoldHandler(0, IDIP_DO_GM_UPDATE_GOLD_REQ));

	//idip安全侧需求;
	Register(new CMSIdipPunishHandler(0, IDIP_AQ_DO_BAN_USR_REQ));
	Register(new CMSIdipPunishHandler(0, IDIP_AQ_DO_MASK_CHAT_REQ));
	Register(new CMSIdipUpdateGoldHandler(0, IDIP_AQ_DO_UPDATE_GOLD_COIN_REQ));
	Register(new CMSIdipUpdateDiamondHandler(0, IDIP_AQ_DO_UPDATE_DIAMOND_REQ));
	Register(new CMSIdipQueryUserPunishInfoHandler());
	Register(new CMSIdipQueryGuildNoticeHandler());
	Register(new CMSIdipSetGuildNoticeHandler());
	Register(new CMSIdipAqQueryOpenidInfoHandler());
	Register(new CMSIdipUpdateScoreHandler(0, IDIP_AQ_DO_CLEAR_SCORE_REQ));
	Register(new CMSIdipUpdateScoreHandler(0, IDIP_AQ_DO_SET_SCORE_REQ));
	Register(new CMSIdipSendNoticeHandler());
	Register(new CMSIdipSendMsgHandler());
	Register(new CMSIdipBanOfflineRankHandler(0, IDIP_AQ_DO_BAN_RANKING_REQ));
	Register(new CMSIdipBanOfflineRankHandler(0, IDIP_AQ_DO_BAN_RANKING_OFFLINE_REQ));
	Register(new CMSIdipQueryBossBattleHandler());
	Register(new CMSIdipZeroProfitHandler());
	Register(new CMSIdipBanPlayHandler(0, IDIP_AQ_DO_BAN_PLAY_REQ));
	Register(new CMSIdipBanPlayHandler(0, IDIP_AQ_DO_BAN_ALL_PLAY_REQ));
	Register(new CMSIdipRelievePunishHandler());
	Register(new CMSIdipInitialAccountHandler());
	Register(new CMSIdipClearChatHandler());


	return true;
}

void CIdipProcess::Uninit()
{
	for(CHandlerInfoMap::iterator it = m_oInfoMap.begin(); it != m_oInfoMap.end(); ++it)
	{
		CHandlerInfo& roInfo = it->second;
		delete roInfo.first;
		for(CHandlerList::iterator iter = roInfo.second.begin(); iter != roInfo.second.end(); ++iter)
		{
			delete *iter;
		}
	}
	m_oInfoMap.clear();
}

CIdipHandler* CIdipProcess::Process(const std::string& strJson, CIdipReply& roReply, const KKSG::GMHttpArg &roArg)
{
	rapidjson::Document doc;
	if (doc.Parse<0>(strJson.c_str()).HasParseError())
	{
		LogError("Json string: [%s] format error", strJson.c_str());
		return NULL;
	}

	if(!doc.HasMember("head"))
	{
		LogError("Json string: [%s] format error, no head member", strJson.c_str());
		return NULL;
	}
	if(!doc.HasMember("body"))
	{
		LogError("Json string: [%s] format error, no body member", strJson.c_str());
		return NULL;
	}
	if(!doc["head"].HasMember("Cmdid"))
	{
		LogError("Json string: [%s] format error, no Cmdid member in head", strJson.c_str());
		return NULL;
	}

	INT32 nCmdID = JsonUtil::GetJsonInt(doc["head"], "Cmdid");
	CIdipHandler* poHandler = Create(nCmdID);
	if(poHandler == NULL)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_CMDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		LogWarn("Invalid command: %d, json string: [%s]", nCmdID, strJson.c_str());
		return NULL;
	}
	UINT32 ServerID = JsonUtil::GetJsonUint(doc["body"], "Partition");
	poHandler->SetJson(strJson);
	poHandler->SetServerID(ServerID);
	INT32 iServerType = IDIP_SERVER_NONE;
	iServerType = poHandler->HandlerRequest(doc["head"], doc["body"], roReply);//检查功能是哪里处理，是否在线
	poHandler->SetServerType(iServerType);
	return poHandler;
}

CIdipHandler* CIdipProcess::Create(INT32 nCmd)
{
	CHandlerInfoMap::iterator it = m_oInfoMap.find(nCmd);
	if(it == m_oInfoMap.end())
	{
		return NULL;
	}

	CHandlerInfo& roInfo = it->second;
	CIdipHandler* poNew = NULL;
	if(roInfo.second.empty())
	{
		poNew = roInfo.first->New();
		poNew->SetCmd(nCmd);
	}
	else
	{
		poNew = roInfo.second.front();
		roInfo.second.pop_front(); 
		poNew->SetID(roInfo.first->GetID());
		poNew->SetCmd(roInfo.first->GetCmd());
		poNew->SetRpcID(roInfo.first->GetRpcID());
	}
	poNew->SetID(++m_dwNextID);
	return poNew;
}

void CIdipProcess::Release(CIdipHandler* poHandler)
{
	if(poHandler == NULL) return;

	CHandlerInfoMap::iterator it = m_oInfoMap.find(poHandler->GetCmd());
	if(it == m_oInfoMap.end())
	{
		return;
	}

	CHandlerInfo& roInfo = it->second;
	poHandler->SetID(0);
	poHandler->SetCmd(0);
	roInfo.second.push_back(poHandler);
}

void CIdipProcess::Register(CIdipHandler* poHandler)
{
	if(poHandler == NULL) return;

	CHandlerInfoMap::iterator it = m_oInfoMap.find(poHandler->GetCmd());
	if(it != m_oInfoMap.end())
	{
		LogWarn("Idip cmd %d has been registered", poHandler->GetCmd());
		return;
	}

	CHandlerInfo oInfo;
	oInfo.first = poHandler;
	m_oInfoMap.insert(std::make_pair(poHandler->GetCmd(), oInfo));
}

void CIdipProcess::AddHandler(CIdipHandler* poHandler)
{
	if(poHandler == NULL) return;

	std::pair<CHandlerMap::iterator, bool> pr = m_oHandlerMap.insert(std::make_pair(poHandler->GetID(), poHandler));
	if(!pr.second)
	{
		LogError("Handle %u has been exist, not replied", poHandler->GetID());
	}
}

bool CIdipProcess::ModifyData(UINT32 dwID, CUserData& roUserData)
{
	CHandlerMap::iterator it = m_oHandlerMap.find(dwID);
	if(it == m_oHandlerMap.end())
	{
		LogError("Can't find handler %u when async replied", dwID);
		return false;
	}

	CIdipHandler* poHandler = it->second;
	if(!poHandler->ModifyMsData(roUserData))
	{
		return false;
	}
	//向DB请求存数据
	poHandler->SaveDataToDB(roUserData);
	return true;
}
bool CIdipProcess::KickoutHandler(UINT32 dwID)
{
	CHandlerMap::iterator it = m_oHandlerMap.find(dwID);
	if(it == m_oHandlerMap.end())
	{
		LogError("Can't find handler %u when async replied", dwID);
		return false;
	}
	CIdipHandler* poHandler = it->second;
	if (poHandler->KickOutReply())//踢下线后立即返回的，需要erase对应的handler
	{
		m_oHandlerMap.erase(poHandler->GetID());
		Release(poHandler);
	}
	return true;
}

void CIdipProcess::OnAsyncReply(UINT32 dwID, const CUserData& roUserData)
{
	CHandlerMap::iterator it = m_oHandlerMap.find(dwID);
	if(it == m_oHandlerMap.end())
	{
		LogError("Can't find handler %u when async replied", dwID);
		return;
	}

	CIdipHandler* poHandler = it->second;
	if (poHandler->GetServerType() == IDIP_SERVER_MS)
	{
		if (poHandler->GetRoleID())
		{
			CPunishMgr::Instance()->UnDoPunishUser(poHandler->GetRoleID(), KKSG::PUNISH_USER_TEMP);
		}
		poHandler->OnAsyncReply(roUserData);
	}
	else if (poHandler->GetServerType() == IDIP_SERVER_GS)
	{
		poHandler->OnAsyncReplyFromGs(roUserData);
	}
	else if (poHandler->GetServerType() == IDIP_SERVER_OTHER)
	{
		poHandler->OnAsyncReplyFromOther(roUserData);
	}
	else 
	{
		poHandler->OnAsyncReply(roUserData);
	}
	m_oHandlerMap.erase(poHandler->GetID());
	Release(poHandler);
}

