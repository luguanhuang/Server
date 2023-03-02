#include "pch.h"
#include "idipcmdproc.h"
#include "commondef_handler.h"


INSTANCE_SINGLETON(CIdipCmdProcess)


CIdipCmdProcess::CIdipCmdProcess()
:m_dwNextID(0)
{
}

CIdipCmdProcess::~CIdipCmdProcess()
{
}

bool CIdipCmdProcess::Init()
{
	//idip
	Register(new CIdipUpdateGoldHandler());
	Register(new CIdipUpdateDiamondHandler());
	Register(new CIdipQueryRoleInfoHandler());
	Register(new CIdipUpdateExpHandler());
	Register(new CIdipUpdateFatigueHandler());
	Register(new CIdipDelItemHandler());
	Register(new CIdipUpdateDragonCoinHandler());
	Register(new CIdipUpdateAreanCoinHandler());
	Register(new CIdipQueryItemHandler());
	Register(new CIdipQueryEmblemHandler());
	Register(new CIdipQueryBagHandler());
	Register(new CIdipQuerySpriteInfoHandler());
	Register(new CIdipNameActHandler());
	Register(new CIdipDoXinyueRedHandler());
	Register(new CIdipQueryFashionHandler());
	Register(new CIdipQuerySpriteInfoHandler());
	Register(new CIdipQueryHorseInfoHandler());
	Register(new CIdipUpdateNameHandler());
	Register(new CIdipItemChangeHandler());
	Register(new CIdipDoIdipRedHandler());
	Register(new CIdipDiamondConsumeToGameHandler());
	Register(new CIdipQueryFashionComposeHandler());

	//°²È«²àidip;
	Register(new CIdipUpdateGoldHandler(0, IDIP_AQ_DO_UPDATE_GOLD_COIN_REQ));
	Register(new CIdipUpdateDiamondHandler(0, IDIP_AQ_DO_UPDATE_DIAMOND_REQ));
	Register(new CIdipUpdateScoreHandler(0, IDIP_AQ_DO_CLEAR_SCORE_REQ));
	Register(new CIdipUpdateScoreHandler(0, IDIP_AQ_DO_SET_SCORE_REQ));
	Register(new CIdipZeroProfitHandler());
	Register(new CIdipBanPlayHandler(0, IDIP_AQ_DO_BAN_PLAY_REQ));
	Register(new CIdipBanPlayHandler(0, IDIP_AQ_DO_BAN_ALL_PLAY_REQ));
	Register(new CIdipRelievePunishHandler(0, IDIP_AQ_DO_RELIEVE_PUNISH_REQ));
	return true;
}

void CIdipCmdProcess::Uninit()
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

CIdipHandler* CIdipCmdProcess::Process(const std::string& strJson, CIdipReply& roReply)
{
	rapidjson::Document doc;
	if (doc.Parse<0>(strJson.c_str()).HasParseError())
	{
		LogError("Json string: [%s] format error", strJson.c_str());
		return NULL;
	}

	INT32 nCmdID = doc["head"]["Cmdid"].GetInt();
	CIdipHandler* poHandler = Create(nCmdID);
	if(poHandler == NULL)
	{
		LogWarn("Invalid command: %d, json string: [%s]", nCmdID, strJson.c_str());
		return NULL;
	}

	bool bFinished = poHandler->Process(doc["head"], doc["body"], roReply);
	if(bFinished)
	{
		Release(poHandler);
		return NULL;
	}
	else
	{
		return poHandler;
	}
}

CIdipHandler* CIdipCmdProcess::Create(INT32 nCmd)
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

void CIdipCmdProcess::Release(CIdipHandler* poHandler)
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

void CIdipCmdProcess::Register(CIdipHandler* poHandler)
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

void CIdipCmdProcess::AddHandler(CIdipHandler* poHandler)
{
	if(poHandler == NULL) return;

	std::pair<CHandlerMap::iterator, bool> pr = m_oHandlerMap.insert(std::make_pair(poHandler->GetID(), poHandler));
	if(!pr.second)
	{
		LogError("Handle %u has been exist, not replied", poHandler->GetID());
	}
}

void CIdipCmdProcess::OnAsyncReply(UINT32 dwID, const CUserData& roUserData)
{
	CHandlerMap::iterator it = m_oHandlerMap.find(dwID);
	if(it == m_oHandlerMap.end())
	{
		LogError("Can't find handler %u when async replied", dwID);
		return;
	}

	CIdipHandler* poHandler = it->second;
	if(poHandler->OnAsyncReply(roUserData))
	{
		m_oHandlerMap.erase(poHandler->GetID());
		Release(poHandler);
	}
}
