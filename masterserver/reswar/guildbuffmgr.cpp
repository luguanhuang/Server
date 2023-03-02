#include "pch.h"
#include "guildbuffmgr.h"
#include "reswarmgr.h"
#include "util/XRandom.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "reswar/reswarpvemgr.h"


INSTANCE_SINGLETON(GuildBuffMgr);

GuildBuffMgr::GuildBuffMgr()
{
	m_nTime = 0;
}

GuildBuffMgr::~GuildBuffMgr()
{
}

bool GuildBuffMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	return true;
}



void GuildBuffMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

GuildBuffInfo* GuildBuffMgr::GetGuildBuffInfo(UINT64 nGuildID,UINT32 nBuffID)
{
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();iter++)
	{
		GuildBuffInfo *pInfo = *iter;
		if (pInfo->nGuildID == nGuildID && pInfo->nID == nBuffID)
		{
			return pInfo;
		}
	}
	return NULL;
}

UINT32 GuildBuffMgr::GetAddBuffTime(UINT64 nGuildID)
{
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();iter++)
	{
		GuildBuffInfo *pInfo = *iter;
		if (pInfo->nGuildID == nGuildID && pInfo->nID == eAddSelfRes)
		{
			return pInfo->nTime;
		}
	}
	return 0;
}



bool GuildBuffMgr::DelBuff(GuildBuffInfo* pGuildBuff,UINT32 nID)
{
	if (!pGuildBuff) return false;
	GuildBuffTable::RowData *pConfig = ResWarAwardMgr::Instance()->GetGuildBuffID(nID);
	UINT64 nGuildID = pGuildBuff->nGuildID;
	if (!pConfig) 
		return false;
	GuildTeam* pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
	if (!pGuildTeam) 
		return false;
	//恢复数值
	switch(pConfig->type)
	{
		case eAddEnemyTime:  //2目标公会探索时间延长X秒时间
			{
				pGuildTeam->nCD =  pGuildTeam->nCD - pConfig->param[0];
				LogInfo("##DelBuff eAddEnemyTime----guildid=%llu name=%s cd=%d----",pGuildTeam->nGuildID, pGuildTeam->szName.c_str(),pGuildTeam->nCD);
				pGuildBuff->bDel				=  true;
			}
			break;
		case eSubSelfTime:   //3己方公会探索时间缩短X秒时间
			{
				pGuildTeam->nCD = pGuildTeam->nCD + pConfig->param[0];
				LogInfo("##DelBuff eSubSelfTime----guildid=%llu name=%s cd=%d----",pGuildTeam->nGuildID, pGuildTeam->szName.c_str(),pGuildTeam->nCD);
				pGuildBuff->bDel				=  true;
			}
			break;
		case eAddSelfRes:     //4己方公会成员探索获得的资源量增加X倍时间
			{
				pGuildTeam->nAddition			    = 0;
				pGuildBuff->bDel				=  true;
			}
			break;
		case eEnemyUnKnown:  //5敌方公会所有成员探索结果变为“？”时间
			{
				pGuildBuff->bDel				=  true;
			}
			break;
	}
	SendBuff(pGuildTeam->nGuildID);
	return true;
}


bool GuildBuffMgr::AddBuff(UINT64 nGuildID,UINT64 nTargetGuildID,UINT32 nID)
{
	GuildBuffTable::RowData *pConfig = ResWarAwardMgr::Instance()->GetGuildBuffID(nID);
	if (!pConfig)
		return false;
	GuildTeam* pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
	if (!pGuildTeam)
		return false;
	LogInfo("AddBuff----nGuildId=%llu nTargetID=%llu nBuffID=%d", nGuildID,nTargetGuildID ,pConfig->type);
	switch(pConfig->type)
	{
	case eStealGuildRes: //1偷取目标公会资源总量5%~10%
		{
			GuildTeam* pTargetTeam = ResWarMgr::Instance()->GetGuildTeam(nTargetGuildID);
			if (!pTargetTeam) 
				return false; 
			double nRand = XRandom::randInt(pConfig->param[0],pConfig->param[1])/100.0;
			UINT32 nResValue = (UINT32)((double)pTargetTeam->nResValue*nRand);
			if (nResValue)
			{
				pTargetTeam->nResValue -= nResValue;
				pGuildTeam->nResValue  += nResValue; 
				LogInfo("----AddBuff eStealGuildRes guildid=%llu guildname=%s  nres=%d targetguildid=%llu targetname=%s targetres=%d",
					pGuildTeam->nGuildID,pGuildTeam->szName.c_str(),pGuildTeam->nResValue,pTargetTeam->nGuildID,pTargetTeam->szName.c_str(),pTargetTeam->nResValue);
				ResWarMgr::Instance()->AddNotice(pGuildTeam, nTargetGuildID, eStealGuildRes, nResValue);
				ResWarMgr::Instance()->SyncGuildMineValue(pGuildTeam->pGroupGuildInfo, pGuildTeam->nGuildID, pGuildTeam->nResValue);
				ResWarMgr::Instance()->SyncGuildMineValue(pTargetTeam->pGroupGuildInfo, pTargetTeam->nGuildID, pTargetTeam->nResValue);
			}
			SendBuff(pGuildTeam->nGuildID);
			return true;
		}
		break;
	case eAddEnemyTime://2目标公会探索时间延长X秒时间
		{
			GuildTeam* pTargetTeam = ResWarMgr::Instance()->GetGuildTeam(nTargetGuildID);
			if (!pTargetTeam) 
				return false;
			GuildBuffInfo* pGuildBuff	= GetGuildBuffInfo(pTargetTeam->nGuildID, pConfig->id);
			if (pGuildBuff)
			{
				pGuildBuff->nTime =  GameTime::GetTime()+pConfig->time;
			}else
			{
				pGuildBuff                      = new GuildBuffInfo();
				pGuildBuff->nGuildID			= pTargetTeam->nGuildID;
				pGuildBuff->nID					= pConfig->id;
				pGuildBuff->nTime				= GameTime::GetTime()+pConfig->time;
				pTargetTeam->nCD			    = pConfig->param[0] + pTargetTeam->nCD;
				LogInfo("eAddEnemyTime---guildid=%llu--guildname=%s targeid=%llu targetname=%s cd=%d",
					pGuildTeam->nGuildID,pGuildTeam->szName.c_str(),pTargetTeam->nGuildID,pTargetTeam->szName.c_str(),pTargetTeam->nCD);
				m_setGuildBuff.insert(pGuildBuff);
			}
			ResWarMgr::Instance()->AddNotice(pGuildTeam, nTargetGuildID, eAddEnemyTime, 0);
			SendBuff(pGuildTeam->nGuildID);
			return true;
		}
		break;
	case eSubSelfTime://3己方公会探索时间缩短X秒时间
		{
			GuildBuffInfo* pGuildBuff	= GetGuildBuffInfo(pGuildTeam->nGuildID, pConfig->id);
			if (pGuildBuff)
			{
				pGuildBuff->nTime =  GameTime::GetTime()+pConfig->time;
			}else
			{
				pGuildBuff                      = new GuildBuffInfo();
				pGuildBuff->nGuildID			= pGuildTeam->nGuildID;
				pGuildBuff->nID					= pConfig->id;
				pGuildBuff->nTime				= GameTime::GetTime()+pConfig->time;
				pGuildTeam->nCD					= pGuildTeam->nCD - pConfig->param[0];
				m_setGuildBuff.insert(pGuildBuff);
			}
			LogInfo("eSubSelfTime---guildid=%llu--guildname=%s cd=%d nTime=%d",pGuildTeam->nGuildID,pGuildTeam->szName.c_str(),pGuildTeam->nCD,pGuildBuff->nTime);
			ResWarMgr::Instance()->AddNotice(pGuildTeam, 0, eSubSelfTime, 0);
			SendBuff(pGuildTeam->nGuildID);
			return true;
		}
		break;
	case eAddSelfRes://4己方公会成员探索获得的资源量增加X倍时间
		{
			GuildBuffInfo* pGuildBuff	= GetGuildBuffInfo(pGuildTeam->nGuildID, pConfig->id);
			if (pGuildBuff)
			{
				pGuildBuff->nTime =  GameTime::GetTime()+pConfig->time;
			}else
			{
				pGuildBuff                      = new GuildBuffInfo();
				pGuildBuff->nGuildID			= pGuildTeam->nGuildID;
				pGuildBuff->nID					= pConfig->id;
				pGuildBuff->nTime				= GameTime::GetTime()+pConfig->time;
				pGuildTeam->nAddition			= pConfig->param[0];
				m_setGuildBuff.insert(pGuildBuff);
			}
			ResWarMgr::Instance()->AddNotice(pGuildTeam, 0, eAddSelfRes, 0);
			SendBuff(pGuildTeam->nGuildID);
			return true;
		}
		break;
	case eEnemyUnKnown://5敌方公会所有成员探索结果变为“？”时间
		{
			GuildBuffInfo* pGuildBuff	= GetGuildBuffInfo(nTargetGuildID, pConfig->id);
			GuildTeam* pTargetTeam = ResWarMgr::Instance()->GetGuildTeam(nTargetGuildID);
			if (!pTargetTeam) 
				return false;
			if (pGuildBuff)
			{
				pGuildBuff->nTime =  GameTime::GetTime()+pConfig->time;
			}else
			{
				pGuildBuff                      = new GuildBuffInfo();
				pGuildBuff->nGuildID			= pTargetTeam->nGuildID;
				pGuildBuff->nID					= pConfig->id;
				pGuildBuff->nTime				= GameTime::GetTime()+pConfig->time;
				m_setGuildBuff.insert(pGuildBuff);
			}
			ResWarMgr::Instance()->AddNotice(pGuildTeam, nTargetGuildID, eEnemyUnKnown, 0);
			SendBuff(pGuildTeam->nGuildID);
			return true;
		}
		break;
	case eClearDeBuff://6清除所有减益仪式卡效果
		{
			ClearDeBuff(pGuildTeam->nGuildID);	
			ResWarMgr::Instance()->AddNotice(pGuildTeam, 0, eClearDeBuff, 0);
			SendBuff(pGuildTeam->nGuildID);
			return true;
		}
		break;
	}
	return false;
}

void GuildBuffMgr::ClearDeBuff(UINT64 nGuildID)
{
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();iter++)
	{
		GuildBuffInfo		*pBuff  = *iter;
		GuildBuffTable::RowData *pConfig = ResWarAwardMgr::Instance()->GetGuildBuffID(pBuff->nID);
		if (pConfig&& pConfig->self == eGuildBuffCampEnemy&&pBuff->nGuildID == nGuildID)
		{
			switch(pConfig->type)
			{
			case eAddEnemyTime:
				{
					DelBuff(pBuff,pConfig->id);
				}
				break;
			case eEnemyUnKnown:
				{
					DelBuff(pBuff,pConfig->id);
				}
				break;
			}
		}
	}
}

void GuildBuffMgr::GetBuff(UINT64 nGuild, std::set<GuildBuffInfo *> &setGuildBuffInfo)
{
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();iter++)
	{
		GuildBuffInfo *pInfo = *iter;
		if (pInfo->nGuildID == nGuild && !pInfo->bDel&&pInfo->nTime!=0)
		{
			setGuildBuffInfo.insert(pInfo);
		}
	}
}

bool GuildBuffMgr::IsUnknown(UINT64 nGuildID)
{
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();iter++)
	{
		GuildBuffInfo *pInfo = *iter;
		if (pInfo->nGuildID == nGuildID &&!pInfo->bDel&&pInfo->nTime!=0)
		{
			if(pInfo->nID == eEnemyUnKnown)
			{
				 return true;
			}
		}
	}
	return false;
}

void GuildBuffMgr::SendBuff(UINT64 nGuildID)
{
	GuildTeam* pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
	ResWarMgr::Instance()->SyncGuildBuff(pGuildTeam);
}

void GuildBuffMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	m_nTime = GameTime::GetTime();
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();iter++)
	{

		GuildBuffInfo *pBuff = *iter;		
		if (m_nTime >= pBuff->nTime)
		{
			DelBuff(pBuff, pBuff->nID);
		}
	}
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();)
	{
		GuildBuffInfo *pBuff = *iter;
		if (pBuff->bDel)
		{
			delete pBuff;
			m_setGuildBuff.erase(iter++);
		}else
		{
			iter++;
		}
	}
}

void GuildBuffMgr::Clear()
{
	for (auto iter = m_setGuildBuff.begin(); iter!=m_setGuildBuff.end();iter++)
	{
		GuildBuffInfo *pBuff = *iter;
		delete pBuff;
	}
	m_setGuildBuff.clear();
	m_setRole.clear();
}

void GuildBuffMgr::AddRoleID(UINT64 roleID)
{
	m_setRole.insert(roleID);
}

void GuildBuffMgr::GetRoleID(std::set<UINT64> &setRole)
{
	setRole = m_setRole;
}

void GuildBuffMgr::DelRoleID(UINT64 roleID)
{
	auto iter = m_setRole.find(roleID);
	if (iter!=m_setRole.end())
	{
		m_setRole.erase(iter++);
	}
}