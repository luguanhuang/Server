#include "pch.h"
#include "rolesummarymgr.h"
#include "time.h"
#include "timeutil.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "pb/project.pb.h"
#include "network/gslink.h"
#include "network/dblink.h"
#include "commondef.h"
#include "ranklist/ranklistmgr.h"
#include "push/pushmgr.h"
#include "tsssdk/tss_sdk_uic.h"
#include "tsssdk/MsTssHandler.h"
#include "role/ptcm2d_saverolesummarydata.h"
#include "util/XCommon.h"
#include "forbid/forbidwordmgr.h"
#include "table/globalconfig.h"
#include "common/ptcm2n_updateregistercount2ns.h"
#include "network/controllink.h"
#include "leagueteam/crossleagueteammgr.h"
#include "role/rpcm2d_queryrolesummary.h"
#include "idutil.h"
#include "dragonguild/dragonguildmgr.h"
#include "activerolerecord.h"

class UpdateRegisterTimer : public ITimer
{
public:
	UpdateRegisterTimer(): m_lastNum(0) {}

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		UINT32 num = CRoleSummaryMgr::Instance()->GetAccountNum();
		if (num != m_lastNum)
		{
			PtcM2N_UpdateRegisterCount2Ns ntf;
			ntf.m_Data.set_register_account(num);
			ControlLink::Instance()->SendTo(ntf);
			m_lastNum = num;
		}
	}

private:
	UINT32 m_lastNum;
};
static UpdateRegisterTimer g_UpdateRegisterTimer;

#define SUMMARY_PARTSIZE 256*1024
#define TIME_UPDATE 30*1000
#define COUNT_UPDATE 100

INSTANCE_SINGLETON(CRoleSummaryMgr)

CRoleSummaryMgr::CRoleSummaryMgr()
:m_maxLevel(0)
{
	m_isend = false;
}

CRoleSummaryMgr::~CRoleSummaryMgr()
{

}

void CRoleSummaryMgr::RecordSize(const ::google::protobuf::MessageLite& data, std::stringstream& ss)
{
	ss << data.GetTypeName() << " : " << data.ByteSize() << "|";
}

void CRoleSummaryMgr::DumpRoleAllInfoSize()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from role";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table role failed, errorMsg: %s", poMySqlConn->GetError());
		return;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return;
		}

		std::string strData;
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'data' column failed");
			return;
		}

		if(dwLen <= 100 * 1000) continue;

		KKSG::RoleAllInfo oRoleInfo;
		if(!oRoleInfo.ParseFromString(strData))
		{
			LogError("Parse role all info data failed, role %llu", qwRoleID);
			return;
		}

		std::stringstream ss;
		RecordSize(oRoleInfo.attributes(), ss);
		RecordSize(oRoleInfo.bag(), ss);
		RecordSize(oRoleInfo.lottery(), ss);
		RecordSize(oRoleInfo.stages(), ss);
		RecordSize(oRoleInfo.checkinrecord(), ss);
		RecordSize(oRoleInfo.activityrecord(), ss);
		RecordSize(oRoleInfo.arenarecord(), ss);
		//RecordSize(oRoleInfo.pay(), ss);
		RecordSize(oRoleInfo.rewardrecord(), ss);
		RecordSize(oRoleInfo.buyinfo(), ss);
		RecordSize(oRoleInfo.shoprecord(), ss);
		RecordSize(oRoleInfo.flowerrecord(), ss);
		RecordSize(oRoleInfo.guildrecord(), ss);
		RecordSize(oRoleInfo.pkrecord(), ss);
		RecordSize(oRoleInfo.config(), ss);
		RecordSize(oRoleInfo.tshowvoterecord(), ss);
		RecordSize(oRoleInfo.camprolerecord(), ss);
		RecordSize(oRoleInfo.findbackrecord(), ss);
		RecordSize(oRoleInfo.extrainfo(), ss);
		RecordSize(oRoleInfo.towerrecord(), ss);
		RecordSize(oRoleInfo.designatinorecord(), ss);
		//RecordSize(oRoleInfo.overexpdata(), ss);
		//RecordSize(oRoleInfo.achievepointrewardinfo(), ss);
		RecordSize(oRoleInfo.loginrecord(), ss);
		RecordSize(oRoleInfo.pvpdata(), ss);
		RecordSize(oRoleInfo.qarecord(), ss);
		RecordSize(oRoleInfo.dragoninfo(), ss);
		RecordSize(oRoleInfo.fashionrecord(), ss);
		RecordSize(oRoleInfo.liverecord(), ss);
		RecordSize(oRoleInfo.payv2(), ss);
		RecordSize(oRoleInfo.petsys(), ss);
		RecordSize(oRoleInfo.firstpassrecord(), ss);
		RecordSize(oRoleInfo.ibshopitems(), ss);
		RecordSize(oRoleInfo.spriterecord(), ss);
		RecordSize(oRoleInfo.riskrecord(), ss);
		RecordSize(oRoleInfo.task_record(), ss);
		RecordSize(oRoleInfo.idiprecord(), ss);
		RecordSize(oRoleInfo.spactivityrecord(), ss);
		RecordSize(oRoleInfo.designatinorecord(), ss);
		RecordSize(oRoleInfo.levelsealdata(), ss);
		RecordSize(oRoleInfo.buffrecord(), ss);
		RecordSize(oRoleInfo.pushinfo(), ss);
		RecordSize(oRoleInfo.qqvip(), ss);

		LogInfo("Role %llu info total size: %u, details: %s", qwRoleID, dwLen, ss.str().c_str());
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all role all info data from db use time: %llu ms", qwTimeUsed);
}

bool CRoleSummaryMgr::Init()
{
	//DumpRoleAllInfoSize();

	///> 用role表矫正rolesummary信息, 防止不一致
	// select * from role;
	// to do...

	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from rolesummary";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes, false))
	{
		LogError("Read table rolesummary failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	int allcount = 0;
	int briefcount = 0;
	int nowtime = (int)time(NULL);
	int expiretime = GetGlobalConfig().RolesummaryExpireTime;
	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'data' column failed");
			return false;
		}

		KKSG::RoleSummaryStored oDBSummary;
		if(!oDBSummary.ParseFromString(strData))
		{
			LogError("Parse role summary data failed, role %llu", qwRoleID);
			return false;
		}

		// check time expire
		bool isbrief = false;
		if (MSConfig::Instance()->IsDataClear(DataClear_Summary))
		{
			if (0 != oDBSummary.timelastlogout() && (nowtime > (expiretime + (int)oDBSummary.timelastlogout())))
			{
				isbrief = true;
				++briefcount;	 			
				DoBrief(oDBSummary);	
				SSInfo<<"expire, roleid:"<<oDBSummary.roleid()<<" last logout:"<<oDBSummary.timelastlogout()<<END;
			}
		}
		++allcount;

		InsertRoleSummary(oDBSummary, SUMMARY_ALL ^ (SUMMARY_LEVEL ^ SUMMARY_ATTR ^ SUMMARY_FASHION), 0, isbrief);
		CActiveRoleRecord::Instance()->Record(oDBSummary.timelastlogout(), qwRoleID, oDBSummary.account());
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all role summary data from db use time: %llu ms, brief count:%d, all count:%d", qwTimeUsed, briefcount, allcount);

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, TIME_UPDATE, -1, __FILE__, __LINE__);

	return true;
}

void CRoleSummaryMgr::DoBrief(KKSG::RoleSummaryStored& data)
{
	//brief
	//optional uint64 RoleID
	//optional uint32 Level
	//optional uint32 Profession
	//optional string Name
	//optional uint32 timelastlogout
	//optional uint32 nickid
	//optional string account

	// clear
	//optional Attribute Attribute
	data.clear_attribute();
	//optional RoleArenaInfo RoleArenaInfo
	data.clear_rolearenainfo();
	//repeated uint32 fashion
	data.clear_fashion();
	//repeated SkillInfo skills
	data.clear_skills();
	//repeated Item equip
	data.clear_equip();
	//optional uint32 PowerPoint
	data.clear_powerpoint();
	//repeated Item emblem
	data.clear_emblem();
	//optional uint32 viplevel
	data.clear_viplevel();
	//optional uint32 pptuptime
	data.clear_pptuptime();
	//optional uint32 leveluptime
	data.clear_leveluptime();
	//optional int32 camp
	data.clear_camp();
	//optional uint32 desigation
	data.clear_desigation();
	//optional uint32 sealtype
	data.clear_sealtype();
	//optional uint32 sealbosscount
	data.clear_sealbosscount();
	//optional uint32 titleid
	data.clear_titleid();
	//optional OutLookOp op
	data.clear_op();
	//optional RolePushInfo push_info
	data.clear_push_info();
	//optional string device_id
	data.clear_device_id();
	//repeated SpriteInfo sprites
	data.clear_sprites();
	//optional uint32 plat_type
	data.clear_plat_type();
	//repeated PetSingle pets
	data.clear_pets();
	//optional uint64 fightPetId
	data.clear_fightpetid();
	//optional uint32 fashionppt
	data.clear_fashionppt();
	//optional uint32 fashionpptuptime
	data.clear_fashionpptuptime();
	//repeated uint32 bindskills
	data.clear_bindskills();
	//repeated PayMember privilege
	data.clear_privilege();
	//optional QQVipInfo qqvip
	data.clear_qqvip();
	//optional StartUpType starttype
	data.clear_starttype();
	//optional uint32 enhancemaster
	data.clear_enhancemaster();
	//optional uint32 pkpoint
	data.clear_pkpoint();
	//optional string token
	data.clear_token();
}

void CRoleSummaryMgr::Uninit()
{
	std::unordered_map<UINT64, CRoleSummary*>::iterator it = m_oRoleSummarys.begin();
	std::unordered_map<UINT64, CRoleSummary*>::iterator itEnd = m_oRoleSummarys.end();
	for(; it != itEnd; ++it)
	{
		delete it->second;
	}
	m_oRoleSummarys.clear();

	if (INVALID_HTIMER != m_timer)
	{
		CTimerMgr::Instance()->KillTimer(m_timer);
	}
}

bool CRoleSummaryMgr::InsertRoleSummary(KKSG::RoleSummaryStored& roData, UINT32 mask, UINT32 timestamp, bool isbrief)
{
	bool isnew = false;
	UINT64 qwRoleID = roData.roleid();
	CRoleSummary* poNew = GetRoleSummary(qwRoleID);
	if (NULL == poNew)
	{
		isnew = true;
		poNew = new CRoleSummary();
	}
	poNew->SetBrief(isbrief);
	if (isnew)
	{
		poNew->Init(roData);

		m_oRoleSummarys[qwRoleID] = poNew;
		m_oName2RoleId[poNew->GetName()] = qwRoleID;
		m_oName2RoleId[ToString(poNew->GetNickID())] = qwRoleID;
		m_oNickId2RoleId[poNew->GetNickID()] = qwRoleID;
		if (!poNew->GetAccount().empty())
		{
			m_account2Role.insert(std::make_pair(poNew->GetAccount(), poNew));
			m_account2LastRole.insert(std::make_pair(poNew->GetAccount(), poNew));
		}

		poNew->SetLastLogout(roData.timelastlogout());
		UpdateRoleSummary(poNew, mask, true);
	}
	else 
	{
		if (poNew->GetName() != roData.name())
		{
			roData.set_name(poNew->GetName());
		}

		if (timestamp >= poNew->GetLastUpdateTime())
		{
			poNew->Init(roData);
			poNew->SetLastUpdateTime(timestamp);

			UpdateRoleSummary(poNew, mask, false);
		}
	}

	if (poNew->GetLevel() > m_maxLevel)
	{
		m_maxLevel = poNew->GetLevel();
		LogInfo("MaxLevel[%u]", m_maxLevel);
	}

	return true;
}

void CRoleSummaryMgr::UpdateRoleSummary(CRoleSummary* roleSummary, UINT32 mask, bool isnew)
{
	const KKSG::RoleSummaryStored& roData = roleSummary->GetData();
	if (mask & SUMMARY_LASTLOGOUT)
	{
		if (!isnew)
		{
			roleSummary->SetLastLogout(0);
		}
		UpdateLastLoginRole(roleSummary->GetAccount());
	}
	if (mask & SUMMARY_PUSHINFO)
	{
		PushMgr::Instance()->UpdateRolePushItem(roleSummary, roData.push_info());
	}
	if (!isnew)
	{
		if ((mask & SUMMARY_LEVEL) || (mask & SUMMARY_ATTR))
		{
			CRankListMgr::Instance()->OnChangeBrief(roleSummary->GetID(), 
													roData.powerpoint(), roData.pptuptime(), roData.level(), roData.leveluptime());
		}
		if (mask & SUMMARY_FASHION)
		{
			CRankListMgr::Instance()->OnChangeFashionPower(roleSummary->GetID(), roData.fashionppt(), 0, roData.fashionpptuptime());
		}
		if (mask & SUMMARY_ATTR)
		{
			DragonGuildMgr::Instance()->OnChangePPT(roleSummary->GetID(), roData.powerpoint());
		}
	}
}

void CRoleSummaryMgr::UpdateLastLoginRole(const std::string& account)
{
	CRoleSummary* oldLastLoginRole = GetLastLoginRole(account);
	CRoleSummary* lastLoginRole = NULL;
	auto itPair = m_account2Role.equal_range(account);
	for (auto it = itPair.first; it != itPair.second; ++it)
	{
		CRoleSummary* role = it->second;
		if (lastLoginRole == NULL)
		{
			lastLoginRole = role;
		} 
		else if (role->GetLastLogout() < lastLoginRole->GetLastLogout())
		{
			lastLoginRole = role;
		}
	}

	// 更新最近登录
	if (lastLoginRole && lastLoginRole != oldLastLoginRole)
	{
		m_account2LastRole[account] = lastLoginRole;
		m_plat2LastRole[lastLoginRole->GetPlatType()].insert(lastLoginRole);
	}

	// 删掉旧的最近登录
	if (oldLastLoginRole && oldLastLoginRole != lastLoginRole)
	{
		m_plat2LastRole[oldLastLoginRole->GetPlatType()].erase(oldLastLoginRole);
		PushMgr::Instance()->RemoveRoleAllPushItem(oldLastLoginRole->GetID());
	}
}

CRoleSummary* CRoleSummaryMgr::GetLastLoginRole(const std::string& account)
{
	auto it = m_account2LastRole.find(account);
	if (it != m_account2LastRole.end())
	{
		return it->second;
	}
	return NULL;
}

void CRoleSummaryMgr::GetRoleIdByAccount(const std::string& account, std::vector<UINT64>& rolelist)
{
	auto itPair = m_account2Role.equal_range(account);
	for (auto it = itPair.first; it != itPair.second; ++it)
	{
		CRoleSummary* pSum = it->second;
		if (pSum)
		{
			rolelist.push_back(pSum->GetID());
		}
	}
}

void CRoleSummaryMgr::GetPushRole(UINT32 platType, FindRoleFilter filter, std::list<CRoleSummary*>& roles)
{
	auto it1 = m_plat2LastRole.find(platType);
	if (it1 == m_plat2LastRole.end())
	{
		return;
	}
	for (auto it = it1->second.begin(); it != it1->second.end(); ++it)
	{
		if (filter(*it))
		{
			roles.push_back(*it);
		}
	}
}

CRoleSummary* CRoleSummaryMgr::GetRoleSummary(UINT64 qwRoleID, bool checkbrief)
{
	auto it = m_oRoleSummarys.find(qwRoleID);
	if(it == m_oRoleSummarys.end())
	{
		return NULL;
	}
	if (checkbrief && it->second->IsBrief())
	{
		QuerySummaryFromDb(qwRoleID);	
	}
	return it->second;
}

CRoleSummary* CRoleSummaryMgr::GetRoleSummary(const std::string& strName, bool checkbrief)
{
	auto it = m_oName2RoleId.find(strName);
	if(it == m_oName2RoleId.end())
	{
		return NULL;
	}
	return GetRoleSummary(it->second, checkbrief);
}

const std::string &CRoleSummaryMgr::GetRoleName(UINT64 qwRoleID)
{
	CRoleSummary* poSummary = GetRoleSummary(qwRoleID);
	if(poSummary == NULL)
	{
		static std::string empty;
		return empty;
	}
	return poSummary->GetName();
}

CRoleSummary* CRoleSummaryMgr::GetRoleSummaryByNickId(UINT64 nickid, bool checkbrief)
{
	auto it = m_oNickId2RoleId.find(nickid);
	if(it == m_oNickId2RoleId.end())
	{
		return NULL;
	}
	return GetRoleSummary(it->second, checkbrief);
}

void CRoleSummaryMgr::SetChanged(UINT64 roleid)
{
	m_queuechanges.push(roleid);
	CrossLeagueTeamMgr::Instance()->OnRoleSummaryChanged(roleid);
}

void CRoleSummaryMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_queuechanges.empty())
		return;

	int bytes = 0;
	PtcM2D_SaveRoleSummaryData ptc;
	ptc.m_Data.set_is_cross_rolesummary(false);
	ptc.SetDontCompress();
	UINT32 count = 0;
	while (!m_queuechanges.empty())
	{
		UINT64 roleid = m_queuechanges.front();
		m_queuechanges.pop();

		CRoleSummary *pSummary = GetRoleSummary(roleid);
		if (pSummary == NULL)
			continue;

		KKSG::RoleSummaryStored* dbsummary = ptc.m_Data.add_data();
		*dbsummary = pSummary->GetData(); 
		dbsummary->set_timelastlogout((UINT32)pSummary->LogoutTime());
		bytes += dbsummary->ByteSize();

		if (bytes >= SUMMARY_PARTSIZE)
		{
			SSWarn<<"send ptc:"<<ptc.m_Data.ByteSize()<<" count:"<<count<<" left count:"<<m_queuechanges.size()<<END;
			bytes = 0;
			DBLink::Instance()->SendTo(ptc);
			ptc.m_Data.Clear();
		}
		if (++count >= COUNT_UPDATE && !m_isend)
		{
			break;
		}
	}
	if (0 != ptc.m_Data.data_size())
	{
		SSInfo<<"send ptc:"<<ptc.m_Data.ByteSize()<<" count:"<<count<<" left count:"<<m_queuechanges.size()<<END;
		DBLink::Instance()->SendTo(ptc);
	}
}

void CRoleSummaryMgr::TestSave()
{
	PtcM2D_SaveRoleSummaryData ptc;
	ptc.m_Data.set_is_cross_rolesummary(false);
	ptc.SetDontCompress();
	UINT32 count = 0;
	UINT32 bytes = 0;
	for (auto it = m_oRoleSummarys.begin(); it != m_oRoleSummarys.end(); ++it)
	{
		CRoleSummary *pSummary = it->second;
		KKSG::RoleSummaryStored* dbsummary = ptc.m_Data.add_data();
		*dbsummary = pSummary->GetData(); 
		bytes += dbsummary->ByteSize();

		if (bytes >= SUMMARY_PARTSIZE)
		{
			SSWarn<<"send ptc:"<<ptc.m_Data.ByteSize()<<" count:"<<count<<" left count:"<<m_queuechanges.size()<<END;
			bytes = 0;
			DBLink::Instance()->SendTo(ptc);
			ptc.m_Data.Clear();
		}
		if (++count >= COUNT_UPDATE && !m_isend)
		{
			break;
		}
	}
	if (0 != ptc.m_Data.data_size())
	{
		SSInfo<<"send ptc:"<<ptc.m_Data.ByteSize()<<" count:"<<count<<" left count:"<<m_queuechanges.size()<<END;
		DBLink::Instance()->SendTo(ptc);
	}
}

void CRoleSummaryMgr::UpdateRankList()
{
	for (auto i = m_oRoleSummarys.begin(); i != m_oRoleSummarys.end(); ++i)
	{
		CRoleSummary* summary = i->second;
		UpdateRoleSummary(summary, SUMMARY_ATTR | SUMMARY_LEVEL | SUMMARY_FASHION, false);	
	}
}

void CRoleSummaryMgr::LockName(const std::string& strName)
{
	m_oNameLocked.insert(strName);
}

void CRoleSummaryMgr::UnlockName(const std::string& strName)
{
	m_oNameLocked.erase(strName);
}

bool CRoleSummaryMgr::IsNameLocked(const std::string& strName) const
{
	return m_oNameLocked.find(strName) != m_oNameLocked.end();
}

bool CRoleSummaryMgr::IsValidAcountRole(const std::string& account, UINT64& qwRoleID)
{
	auto it = m_oRoleSummarys.find(qwRoleID);
	if (it != m_oRoleSummarys.end())
	{
		if (it->second->GetAccount() == account)
		{
			return true;
		}
	}
	
	CRoleSummary* pSum = GetRoleSummaryByNickId(qwRoleID);
	if (pSum && pSum->GetAccount() == account)
	{
		qwRoleID = pSum->GetID();
		return true;
	}
	return false;
}

UINT32 CRoleSummaryMgr::IsNameInvalid(const std::string& name)
{
	//请求腾讯sdk验证名字是否合法;
	KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(name);
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		return nErrCode;
	}

	if(ForbidWordMgr::Instance()->HaveForbidWord(name))
	{
		return KKSG::ERR_INVALID_NAME;
	}
	
	UINT32 nameLen = (UINT32)XCommon::CountUTF8Char(name);
	if(nameLen < GetGlobalConfig().RoleNameLenMin)
	{
		return KKSG::ERR_NAME_TOO_SHORT;
	}
	if(nameLen > GetGlobalConfig().RoleNameLenMax)
	{
		return KKSG::ERR_NAME_TOO_LONG;
	}

	bool isAllNumber = true;
	for(UINT32 i = 0; i < name.length(); i++)
	{
		char c = name.at(i);
		if(c < '0' || c > '9')
		{
			isAllNumber = false;
			break;
		}
	}
	if(isAllNumber)
	{
		return KKSG::ERR_NAME_ALLNUM;
	}

	//Check Name/NickID
	if(CRoleSummaryMgr::Instance()->GetRoleSummary(name) != NULL)
	{
		return KKSG::ERR_NAME_EXIST;
	}

	if(CRoleSummaryMgr::Instance()->IsNameLocked(name))
	{
		return KKSG::ERR_NAME_EXIST;
	}

	return KKSG::ERR_SUCCESS;
}

bool CRoleSummaryMgr::ChangeName(const std::string& lastname, const std::string& nowname)
{
	auto i = m_oName2RoleId.find(lastname);
	if (i == m_oName2RoleId.end())
	{
		return false;
	}
	UINT64 roleid = i->second;
	m_oName2RoleId.erase(i);
	m_oName2RoleId[nowname] = roleid;
	return true;
}

void CRoleSummaryMgr::QuerySummaryFromDb(UINT64 roleid)
{
	RpcM2D_QueryRolesummary* rpc = RpcM2D_QueryRolesummary::CreateRpc();
	rpc->m_oArg.set_roleid(roleid);
	DBLink::Instance()->SendTo(*rpc);

	SSInfo<<"query role summary from db, role:"<<roleid<<END;
}

bool CRoleSummaryMgr::IsExpire(UINT64 roleid, int expiretime, int level)
{
	if (!MSConfig::Instance()->IsDataClear(DataClear_Summary))
	{
		return false;
	}
	CRoleSummary* summary = GetRoleSummary(roleid);
	if (NULL == summary)
	{
		SSInfo<<"not find role summary, roleid:"<<roleid<<END;
		return true;
	}
	int nowtime = (int)time(NULL);
	if (0 == level || (int)summary->GetLevel() <= level)
	{
		if (0 != summary->LogoutTime() && (nowtime > (expiretime + (int)summary->LogoutTime())))
		{
			SSInfo<<"expire, roleid:"<<roleid<<" level:"<<summary->GetLevel()<<" last logout:"<<summary->LogoutTime()<<END;
			return true;
		}
	}
	return false;
}

UINT32 CRoleSummaryMgr::GetAccountRoleNumSameServer(UINT64 roleId)
{
	CRoleSummary* pSu = GetRoleSummary(roleId);
	if (pSu == NULL)
	{
		return 0;
	}
	UINT32 num = 0;
	auto itPair = m_account2Role.equal_range(pSu->GetAccount());
	for (auto it = itPair.first; it != itPair.second; it++)
	{
		CRoleSummary* pOtherSu = it->second;
		if (::GetServerIDByRoleID(roleId) == ::GetServerIDByRoleID(pOtherSu->GetID()))
		{
			num++;
		}
	}
	return num;
}




