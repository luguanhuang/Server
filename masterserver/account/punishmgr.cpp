#include "pch.h"
#include "role/rolesummarymgr.h"
#include "network/dblink.h"
#include "network/gslink.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "timeutil.h"
#include "punishmgr.h"
#include "role/rolemanager.h"
#include <time.h>
#include "account/ptcm2d_savepunishdata.h"
#include "ranklist/ranklistmgr.h"
#include "idip/idip.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "ranklist/flowerranklist.h"
#include "common/ptcm2c_idippunishinfomsntf.h"
#include "accountkick.h"
#include "util/gametime.h"

INSTANCE_SINGLETON(CPunishMgr);

CPunishMgr::CPunishMgr()
{
	m_vIdipRankType[BanRankTypeWorldBoss] = KKSG::PUNISH_USER_WORLDBOSS_RANK;
	m_vIdipRankType[BanRankTypeRoleGuildBoss] = KKSG::PUNISH_USER_ROLE_GUILDBOSS;

	m_vIdipRankOfflineType[BanOfflineRankType_Pk] = KKSG::PUNISH_USER_PK_RANK;
	m_vIdipRankOfflineType[BanOfflineRankType_Arena] = KKSG::PUNISH_USER_ARENA_RANK;
	m_vIdipRankOfflineType[BanOfflineRankType_Tower] = KKSG::PUNISH_USER_TOWER;
	m_vIdipRankOfflineType[BanOfflineRankType_Flower] = KKSG::PUNISH_USER_FLOWER_RANK;
	m_vIdipRankOfflineType[BanOfflineRankType_Guild] = KKSG::PUNISH_USER_GUILD_RANK;
	m_vIdipRankOfflineType[BanOfflineRankType_GuildBoss] = KKSG::PUNISH_USER_GUILDBOSS_RANK;
}

CPunishMgr::~CPunishMgr()
{

}

bool CPunishMgr::Init()
{
	if (!ReadAllPunishListFromDB())return false;

	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 86000*1000, -1, __FILE__, __LINE__);
	return true;
}

void CPunishMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
}

void CPunishMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	for (auto i = m_oPunishInfo.begin(); i != m_oPunishInfo.end();)
	{
		if (IsExpireTime(i->second.bantime(), i->second.punishdate()))
		{
			m_oPunishInfo.erase(i++);
		}
		else
		{
			i ++;
		}
	}
}
std::string CPunishMgr::GetKey(UINT64 qwRoleID, INT32 nType)
{
	std::stringstream ss;
	ss << qwRoleID << "#" << nType;
	return ss.str();
}
bool CPunishMgr::DoPunishUser(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear)
{
	std::string key = GetKey(qwRoleID, oData.punishtype());
	auto it = m_oPunishInfo.find(key);
	if (it == m_oPunishInfo.end())
	{
		m_oPunishInfo.insert(std::make_pair(key, oData));
	}
	else
	{
		it->second.CopyFrom(oData);
	}

	//排行榜封禁需要判断下是否清除
	if (isClear)
	{
		CRankListMgr::Instance()->DeleteTheOneInRankList(qwRoleID, (KKSG::PunishType)oData.punishtype());
	}
	if (oData.punishtype() != KKSG::PUNISH_USER_TEMP)//临时封禁不写db;
	{
		PtcM2D_SavePunishData ptc;
		ptc.m_Data.CopyFrom(oData); 
		DBLink::Instance()->SendTo(ptc);
	}
	return true;
}

void CPunishMgr::DoPunishTemp(UINT64 qwRoleID)
{
	KKSG::PunishData oData;
	oData.set_roleid(qwRoleID);
	oData.set_punishdate(UINT32(time(NULL)));
	oData.set_punishtype(KKSG::PUNISH_USER_TEMP);
	oData.set_bantime(MAX_LOCK_ROLE_TIME);
	std::string key = GetKey(qwRoleID, oData.punishtype());
	auto it = m_oPunishInfo.find(key);
	if (it == m_oPunishInfo.end())
	{
		m_oPunishInfo.insert(std::make_pair(key, oData));
	}
	else
	{
		it->second.CopyFrom(oData);
	}
}

bool CPunishMgr::UnDoPunishUser(UINT64 qwRoleID, INT32 nType)
{
	std::string key = GetKey(qwRoleID, nType);
	auto it = m_oPunishInfo.find(key);
	if (it != m_oPunishInfo.end())
	{
		if (nType != KKSG::PUNISH_USER_TEMP)
		{
			PtcM2D_SavePunishData ptc;
			ptc.m_Data.CopyFrom(it->second);
			ptc.m_Data.set_op(KKSG::PUNISH_DATA_DELETE);
			DBLink::Instance()->SendTo(ptc);
		}
		m_oPunishInfo.erase(it);
		return true;
	}
	return false;
}

bool CPunishMgr::IsExpireTime(UINT32 uBanTime, UINT32 uPunishDate)
{
	if ((uBanTime + uPunishDate) <= (UINT32)GameTime::GetTime())
	{
		return true;
	}
	return false;
}
bool CPunishMgr::IsPunish(UINT64 qwRoleID, INT32 nType)
{
	std::string key = GetKey(qwRoleID, nType);
	auto it = m_oPunishInfo.find(key);
	if (it != m_oPunishInfo.end())
	{
		if (IsExpireTime(it->second.bantime(), it->second.punishdate()))
		{
			m_oPunishInfo.erase(it);
			return false;
		}
		return true;
	}
	return false;
}

bool CPunishMgr::ReadAllPunishListFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();


	std::stringstream ss;
	ss << "select account,roleid,punishtype,punishdate,bantime,reason,rankdata from punishmentlist where punishdate+bantime>" << (UINT32)time(NULL);
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table punishmentlist failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;

		std::string strAccount;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			strAccount.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'account' column failed");
			return false;
		}

		UINT64 roleID = 0;
		if (poRes->GetFieldValue(1, &pVal, dwLen))
		{
			roleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'roleid' column failed");
			return false;
		}

		UINT32 punishType = 0;
		if (poRes->GetFieldValue(2, &pVal, dwLen))
		{
			punishType = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'punishtype' column failed");
			return false;
		}

		UINT32 punishDate = 0;
		if (poRes->GetFieldValue(3, &pVal, dwLen))
		{
			punishDate = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'punishdate' column failed");
			return false;
		}

		UINT32 banTime = 0;
		if (poRes->GetFieldValue(4, &pVal, dwLen))
		{
			banTime = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'bantime' column failed");
			return false;
		}

		std::string strReason;
		if (poRes->GetFieldValue(5, &pVal, dwLen))
		{
			strReason.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'reason' column failed");
			return false;
		}
		

		std::string strRankData;
		if (poRes->GetFieldValue(6, &pVal, dwLen))
		{
			strRankData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'rankdata' column failed");
			return false;
		}
		KKSG::PunishData oData;
		oData.set_account(strAccount);
		oData.set_roleid(roleID);
		oData.set_punishdate(punishDate);
		oData.set_punishtype(punishType);
		oData.set_bantime(banTime);
		oData.set_reason(strReason);
		oData.set_rankdata(strRankData);
		m_oPunishInfo.insert(std::make_pair(GetKey(roleID, punishType), oData));
	}

	poRes->Release();

	ss.str("");
	ss << "delete from punishmentlist where punishdate+bantime<="<<(UINT32)time(NULL);
	poMySqlConn->Query(ss.str().c_str(), ss.str().size(), NULL);
	LogInfo("delete expired punish info from db");

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all punish data from db use time: %llu ms", qwTimeUsed);
	return true;
}
void CPunishMgr::GetPunishList(INT32 nType, std::vector<KKSG::PunishData>& vList)
{
	for (auto i = m_oPunishInfo.begin(); i != m_oPunishInfo.end(); i ++)
	{
		if ( i->second.punishtype() != nType)
		{
			continue;
		}
		if (!IsExpireTime(i->second.bantime(), i->second.punishdate()))
		{
			vList.push_back(i->second);
		}
	}
}
bool CPunishMgr::GetPunishInfo(UINT64 qwRoleID, INT32 nType, KKSG::PunishData& oData)
{
	auto it = m_oPunishInfo.find(GetKey(qwRoleID, nType));
	if (it != m_oPunishInfo.end())
	{
		if (IsExpireTime(it->second.bantime(), it->second.punishdate()))
		{
			m_oPunishInfo.erase(it);
			return false;
		}
		oData.CopyFrom(it->second);
		return true;
	}
	return false;
}

bool CPunishMgr::CheckPunishAndNotify(CRole* pRole,  UINT64 qwID, INT32 nType)
{
	if (NULL == pRole)
	{
		return false;
	}
	auto it = m_oPunishInfo.find(GetKey(qwID, nType));
	if (it != m_oPunishInfo.end())
	{
		if (IsExpireTime(it->second.bantime(), it->second.punishdate()))
		{
			m_oPunishInfo.erase(it);
			return false;
		}
		PtcM2C_IdipPunishInfoMsNtf ptc;
		ptc.m_Data.set_type(nType);
		ptc.m_Data.set_endtime((INT32)(it->second.punishdate() + it->second.bantime()));
		ptc.m_Data.set_bantime(it->second.bantime());
		ptc.m_Data.set_lefttime(INT32(ptc.m_Data.endtime() - (INT32)time(NULL)));
		pRole->Send(ptc);
		return true;
	}
	return false;
}

bool CPunishMgr::PunishRank(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear/* =false */)
{
	if (oData.punishtype() == BanRankTypeALL)
	{
		for(auto i = m_vIdipRankType.begin(); i != m_vIdipRankType.end(); i ++)
		{
			oData.set_punishtype(i->second);
			DoPunishUser(qwRoleID, oData, isClear);
		}
	}
	else
	{
		auto it = m_vIdipRankType.find(oData.punishtype());
		if (it == m_vIdipRankType.end())
		{
			return false;
		}
		oData.set_punishtype(it->second);
		DoPunishUser(qwRoleID, oData, isClear);
	}
	return true;
}

bool CPunishMgr::PunishOfflineRank(UINT64 qwRoleID, KKSG::PunishData oData, bool isClear/* =false */)
{
	if (oData.punishtype() == BanOfflineRankType_ALL)
	{
		for (auto i = m_vIdipRankOfflineType.begin(); i != m_vIdipRankOfflineType.end(); i ++)
		{
			if (OperRole(qwRoleID, i->first, oData, isClear))
			{
				oData.set_punishtype(i->second);
				DoPunishUser(oData.roleid(), oData, isClear);
			}
		}
	}
	else
	{
		auto it = m_vIdipRankOfflineType.find(oData.punishtype());
		if (it == m_vIdipRankOfflineType.end())
		{
			return false;
		}
		if (OperRole(qwRoleID, it->first, oData, isClear))
		{
			oData.set_punishtype(it->second);
			DoPunishUser(oData.roleid(), oData, isClear);
		}
	}
	return true;
}
bool CPunishMgr::OperRole(UINT64 qwRoleID, INT32 iType, KKSG::PunishData& oData, bool isClear)
{
	switch (iType)
	{
	case BanOfflineRankType_Guild:
	case BanOfflineRankType_GuildBoss:
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(qwRoleID);
			if (NULL == pGuild)
			{
				return false;
			}
			oData.set_roleid(pGuild->GetID());
			if (iType == BanOfflineRankType_Guild && !isClear)
			{			
				KKSG::IdipGuildInfo roInfo;
				//roInfo.set_exp(pGuild->GetExp());
				//roInfo.set_level(pGuild->GetLevel());
				roInfo.set_prestige(pGuild->GetPrestige());
				oData.set_rankdata(roInfo.SerializeAsString());
			}
		}
		break;
	case BanOfflineRankType_Flower:
		{
			FlowerRankList* pList = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerRank);
			if (pList == NULL)
			{
				return false;
			}
			if (!isClear)
			{
				oData.set_rankdata(pList->GetPunishedRankInfo(qwRoleID));
			}
		}
		break;
	default:
		break;
	}
	return true;
}

void CPunishMgr::UpdatePunishRankData()
{
	for (auto i = m_oPunishInfo.begin(); i != m_oPunishInfo.end(); i ++)
	{
		if (i->second.punishtype() == KKSG::PUNISH_USER_FLOWER_RANK)
		{
			FlowerRankList* pList = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerRank);
			if (pList && !i->second.rankdata().empty())
			{
				pList->InsertPunishedRankInfo(i->second.rankdata());
			}
		}
	}
	return;
}

void CPunishMgr::UnPunishALLRank(UINT64 qwRoleID)
{
	for(auto i = m_vIdipRankType.begin(); i != m_vIdipRankType.end(); i ++)
	{
		UnDoPunishUser(qwRoleID, i->second);
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(qwRoleID);
	for (auto i = m_vIdipRankOfflineType.begin(); i != m_vIdipRankOfflineType.end(); i ++)
	{
		if ((i->first == BanOfflineRankType_Guild || i->first == BanOfflineRankType_GuildBoss) && pGuild)
		{
			UnDoPunishUser(pGuild->GetID(), i->second);
		}
		else
		{
			UnDoPunishUser(qwRoleID, i->second);
		}
	}
}
