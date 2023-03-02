#include "pch.h"
#include "partnermgr.h"
#include "friend/friendmgr.h"
#include "friend/friend.h"
#include "partner/rpcm2g_makepartnerm2g.h"
#include "network/gslink.h"
#include "commondef.h"
#include "db/mysqlmgr.h"
#include "partner/ptcm2c_updatepartnertoclient.h"
#include "partner/ptcm2g_updatepartnertogs.h"
#include "table/globalconfig.h"
#include "partner/ptcm2c_makepartnerresultntf.h"
#include "role/rolesummarymgr.h"
#include "util/gametime.h"
#include "role/rolemanager.h"
#include "util/timespecificmgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "tablemgr/partnertablemgr.h"
#include "notice/notice.h"

PartnerMgr::PartnerMgr()
:m_curId(0)
,m_lastPassDayTime(0)
{

}

PartnerMgr::~PartnerMgr()
{

}

bool PartnerMgr::Init()
{
	if (!LoadFromDB())
	{
		LogError("load from db failed");
		return false;
	}
	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	m_leaveTimer.SetCallBack(std::bind(&PartnerMgr::OnLeaveTimeOut, this, std::placeholders::_1));

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&PartnerMgr::OnDayPass, this));

	auto times = PartnerTableMgr::Instance()->GetShopRefreshTime();
	for (auto it = times.begin(); it != times.end(); ++it)
	{
		UINT32 temp = *it;
		UINT32 hour =  temp / 100;
		UINT32 min = temp % 100;
		TimeSpecific time(hour, min, 0);
		CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&PartnerMgr::RefreshShop, this));
	}

	return true;
}

void PartnerMgr::Uninit()
{
	for (auto it = m_partners.begin(); it != m_partners.end(); ++it)
	{
		delete it->second;
	}
	m_partners.clear();
}

void PartnerMgr::SaveAll2DB()
{
	// 被删除的战友关系
	while (!m_delList.empty())
	{
		UINT64 id = m_delList.pop_front();
		Partner* parter = GetPartner(id);
		if (parter == NULL)
		{
			continue;
		}
		parter->DelToDB();
		delete parter;
		m_partners.erase(id);
	}

	// 需要更新的战友关系
	while (!m_dirtyList.empty())
	{
		UINT64 id = m_dirtyList.pop_front();
		Partner* parter = GetPartner(id);
		if (parter == NULL)
		{
			continue;
		}
		parter->UpdateToDB();
	}
}

void PartnerMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	m_leaveTimer.Update(GameTime::GetTime());

	// 被删除的战友关系
	while (!m_delList.empty())
	{
		UINT64 id = m_delList.pop_front();
		Partner* parter = GetPartner(id);
		if (parter == NULL)
		{
			continue;
		}
		parter->DelToDB();
		delete parter;
		m_partners.erase(id);
	}

	// 需要更新的战友关系
	UINT32 i = 0;
	while (!m_dirtyList.empty() && i < 20)
	{
		UINT64 id = m_dirtyList.pop_front();
		Partner* parter = GetPartner(id);
		if (parter == NULL)
		{
			continue;
		}
		parter->UpdateToDB();
		i++;
	}
}

bool PartnerMgr::LoadFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::string sql("select _id, base, liveness, shop from partner");
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read table partner failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// partnerId
		UINT64 partnerId;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			partnerId = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		KKSG::PartnerSaveData data;
		// base
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			data.mutable_base()->assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'base' column failed");
			return false;
		}

		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			data.mutable_liveness()->assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'liveness' column failed");
			return false;
		}

		if(poRes->GetFieldValue(3, &pVal, dwLen))
		{
			data.mutable_shop()->assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'shop' column failed");
			return false;
		}
		
		Partner* partner = new Partner;
		partner->SetID(partnerId);
		partner->LoadFromDB(data);
		AddPartner(partnerId, partner);

		// 检查
		partner->OnLoaded();
	}

	poRes->Release();
	SSInfo << "Load partner num: " << m_partners.size() << END;

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all partner data from db use time: %llu ms", qwTimeUsed);
	return true;
}

UINT64 PartnerMgr::NewID()
{
	// 16位serverid 32位时间戳 16位自增id
	UINT64 temp = MSConfig::Instance()->GetServerID() & 0xFFFF;
	temp <<= 48;
	UINT64 t = TimeUtil::GetTime();
	temp |= ((t & 0xFFFFFFFF) << 16);
	temp |= ((++m_curId) & 0xFFFF);
	return temp;
}

void PartnerMgr::AddPartner(UINT64 id, Partner* partner)
{
	auto it = m_partners.find(id);
	if (it != m_partners.end())
	{
		LogError("duplicate parter id[%llu]", id);
		return;
	}
	m_partners.insert(std::make_pair(id, partner));
}

void PartnerMgr::AddRolePartner(UINT64 roleId, UINT64 id)
{
	m_roleId2partnerId.insert(std::make_pair(roleId, id));
}

void PartnerMgr::RemoveRolePartner(UINT64 roleId)
{
	m_roleId2partnerId.erase(roleId);
}

Partner* PartnerMgr::GetPartner(UINT64 partnerID)
{
	auto it = m_partners.find(partnerID);
	if (it == m_partners.end())
	{
		return NULL;
	}
	return it->second;
}

UINT64 PartnerMgr::GetPartnerIdByRole(UINT64 roleID)
{
	auto it = m_roleId2partnerId.find(roleID);
	if (it == m_roleId2partnerId.end())
	{
		return 0;
	}
	return it->second;
}

Partner* PartnerMgr::GetPartnerByRole(UINT64 roleID)
{
	UINT64 partnerID = GetPartnerIdByRole(roleID);
	if (partnerID == 0)
	{
		return NULL;
	}
	return GetPartner(partnerID);
}

bool PartnerMgr::HasPartner(UINT64 roleID)
{
	return GetPartnerIdByRole(roleID) != 0;
}

bool PartnerMgr::IsPartner(UINT64 roleID1, UINT64 roleID2)
{
	if (roleID1 == roleID2)
	{
		return false;
	}
	UINT64 partnerID1 = GetPartnerIdByRole(roleID1);
	if (partnerID1 == 0)
	{
		return false;
	}
	UINT64 partnerID2 = GetPartnerIdByRole(roleID2);
	if (partnerID2 == 0)
	{
		return false;
	}
	return partnerID1 == partnerID2;
}

void PartnerMgr::StartMakePartner(std::vector<UINT64>& partnerIds)
{
	// MS上检查好友度、人数
	// GS检查等级、龙币、当天没有解除过战友关系
	UINT64 errorId = 0;
	KKSG::ErrorCode code = CheckMakePartner(partnerIds, errorId);
	if (code != KKSG::ERR_SUCCESS)
	{
		NotifyMakePartnerError(partnerIds, code, errorId);
		return;
	}
	RpcM2G_MakePartnerM2G* rpc = RpcM2G_MakePartnerM2G::CreateRpc();
	for (size_t i = 0; i < partnerIds.size(); ++i)
	{
		rpc->m_oArg.add_roleids(partnerIds[i]);
	}
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
}

KKSG::ErrorCode PartnerMgr::CheckMakePartner(std::vector<UINT64>& partnerIds, UINT64& errorRoleId)
{
	if (partnerIds.size() != GetGlobalConfig().PartnerNum)
	{
		return KKSG::ERR_PARTNER_NUM_INVALID;
	}

	for (size_t i = 0; i < partnerIds.size(); ++i)
	{
		UINT64 roleId = partnerIds[i];
		KKSG::ErrorCode code = CheckMakePartner(roleId, partnerIds);
		if (code != KKSG::ERR_SUCCESS)
		{
			errorRoleId = roleId;
			return code;
		}
	}
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode PartnerMgr::CheckMakePartner(UINT64 roleId, std::vector<UINT64>& parnterIds)
{
	if (HasPartner(roleId))
	{
		return KKSG::ERR_PARTNER_ALREADY_HAS;
	}

	Friend* info = FriendMgr::Instance()->GetFriendInfo(roleId);
	if (info == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	for (size_t i = 0; i < parnterIds.size(); ++i)
	{
		UINT64 otherId = parnterIds[i];
		if (otherId == roleId)
		{
			continue;
		}
		if (info->GetDegree(otherId) < GetGlobalConfig().PartnerNeedFriendDegree)
		{
			return KKSG::ERR_PARTNER_FDEGREE_NOT_ENOUGH;
		}
	}
	return KKSG::ERR_SUCCESS;
}

Partner* PartnerMgr::MakePartner(std::vector<UINT64>& partnerIds)
{
	if (partnerIds.size() < 2)
	{
		NotifyMakePartnerError(partnerIds, KKSG::ERR_PARTNER_NUM_INVALID, 0);
		return NULL;
	}
	for (size_t i = 0; i < partnerIds.size(); ++i)
	{
		if (HasPartner(partnerIds[i]))
		{
			NotifyMakePartnerError(partnerIds, KKSG::ERR_PARTNER_ALREADY_HAS, partnerIds[i]);
			return NULL;
		}
	}

	UINT64 id = NewID();
	Partner* partner = new Partner;
	partner->SetID(id);
	partner->SetMember(partnerIds);
	partner->OnCreate();

	AddPartner(id, partner);
	for (size_t i = 0; i < partnerIds.size(); ++i)
	{
		AddRolePartner(partnerIds[i], id);
	}

	// 添加到DB
	partner->AddToDB();

	// 推送结拜战友结果
	PtcM2C_MakePartnerResultNtf ntf;
	ntf.m_Data.set_result(KKSG::ERR_SUCCESS);
	ntf.m_Data.set_partnerid(id);
	ntf.m_Data.set_level(partner->GetLevel());
	ntf.m_Data.set_degree(partner->GetDegree());
	for (size_t i = 0; i < partnerIds.size(); ++i)
	{
		ntf.m_Data.add_memberid(partnerIds[i]);
	}
	partner->BroadCast(ntf);

	// 通知GS，玩家自己身上存一个备份
	PtcM2G_UpdatePartnerToGS ntfToGs;
	ntfToGs.m_Data.set_type(KKSG::POT_Normal);
	ntfToGs.m_Data.set_partnerid(id);
	ntfToGs.m_Data.set_level(partner->GetLevel());
	ntfToGs.m_Data.set_degree(partner->GetDegree());
	partner->BroadCastToGS(ntfToGs);


	// notice
	Notice notice(PartnerMakeNotice);
	notice.SendPlays(partnerIds);

	// Log
	std::ostringstream oss;	
	for (size_t i = 0; i < partnerIds.size(); ++i)
	{
		if (i != 0)
		{
			oss << ",";
		}
		oss << partnerIds[i];
	}
	LogInfo("make partner[%llu], [%s]", id, oss.str().c_str());

	return partner;
}

void PartnerMgr::AddFriendDegree(UINT64 roleId1, UINT64 roleId2, UINT32 value)
{
	if (!IsPartner(roleId1, roleId2))
	{
		return ;
	}
	Partner* p = GetPartnerByRole(roleId1);
	if (p == NULL)
	{
		return ;
	}
	p->AddFriendDegree(value);
}

void PartnerMgr::NotifyMakePartnerError(std::vector<UINT64>& roleIds, KKSG::ErrorCode code, UINT64 roleId)
{
	PtcM2C_MakePartnerResultNtf ntf;
	ntf.m_Data.set_result(code);
	ntf.m_Data.set_err_roleid(roleId);
	if (roleId)
	{
		CRoleSummary* sum = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
		if (sum)
		{
			ntf.m_Data.set_err_rolename(sum->GetName());
		}
	}
	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(*it);
		if (pRole == NULL)
		{
			continue;
		}
		pRole->Send(ntf);
	}
}

void PartnerMgr::AddToLeaveTimeList(UINT64 roleId, UINT32 time)
{
	m_leaveTimer.Add(roleId, time);
}

void PartnerMgr::RemoveFromLeaveTimeList(UINT64 roleId)
{
	m_leaveTimer.Remove(roleId);
}

void PartnerMgr::OnLeaveTimeOut(UINT64 roleId)
{
	Partner* partner = GetPartnerByRole(roleId);	
	if (partner == NULL)
	{
		return;
	}
	partner->DoLeaveMember(roleId);
}

bool PartnerMgr::OnDayPass()
{
	for (auto it = m_partners.begin(); it != m_partners.end(); ++it)
	{
		it->second->OnDayPass();
	}

	m_lastPassDayTime = GameTime::GetTime();
	CGlobalConfigDBBuild::Instance()->SetConfigValue(ePartnerDailyResetTime, ToString(m_lastPassDayTime));	
	CGlobalConfigDBBuild::Instance()->UpdateToDb(ePartnerDailyResetTime);

	return true;
}

bool PartnerMgr::RefreshShop()
{
	for (auto it = m_partners.begin(); it != m_partners.end(); ++it)
	{
		it->second->GetPartnerShop().RefreshItem();
	}
	return true;
}

void PartnerMgr::CheckDayPass()
{
	const std::string temp = CGlobalConfigDBBuild::Instance()->GetConfigValue(ePartnerDailyResetTime);
	m_lastPassDayTime = convert<UINT32>(temp);
	if (!GameTime::IsInSameDay(GameTime::GetTime(), m_lastPassDayTime, true))
	{
		OnDayPass();
	}
}

