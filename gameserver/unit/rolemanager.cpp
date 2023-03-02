#include "pch.h"
#include <stdexcept>
#include "util.h"
#include "role.h"
#include "rolemanager.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "gamelogic/opensystemhandler.h"
#include "gamelogic/rolecommonhandler.h"
#include "gamelogic/achievementhandler.h"
#include "gamelogic/activityhandler.h"
#include "gamelogic/rolesummaryhandler.h"
#include "gamelogic/rolenethandler.h"
#include "selectrolestatemgr.h"
#include "util/XRandom.h"
#include "utility/loghelper.h"
#include "dblink.h"
#include "foreach.h"
#include "util/timespecificmgr.h"
#include "unit/accountsessionmgr.h"
#include "gamelogic/levelsealRecord.h"
#include "gamelogic/levelsealMgr.h"
#include "util/gametime.h"
#include "spactivity/spactivityhandler.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/reportdatahandler.h"


INSTANCE_SINGLETON(RoleManager)

RoleManager::RoleManager()
{
}

RoleManager::~RoleManager()
{

}

bool RoleManager::Init()
{
	TimeSpecific time(HOUR_ONE_DAY_BEGIN,0,5);
	m_dwDayPassTimeSpecific = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&RoleManager::UpdateOnDayPass, this));
	return true;
}

void RoleManager::Uninit()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_dwDayPassTimeSpecific);
}

void RoleManager::AddRole(Role* role)
{
	if (FindByRoleID(role->GetID()) != NULL)
	{
		LogError("RoleManager add role is already exist!: %s", ToString(role->GetID()).c_str());
		LogMgr::Instance()->Flush();
		assert(false);
		exit(0);
		return;
	}

	m_RoleDatas.insert(role);	
	m_name2roleid[role->GetName()] = role->GetID();
	m_nick2roleid[role->GetNickId()] = role->GetID();
	//SSDebug << "Add Role " << role << " To RoleManager!" << END;
}

void RoleManager::RemoveRole(Role* role)
{
	m_RoleDatas.erase<SessionIndex>(role->GetSessionID());

	auto i = m_name2roleid.find(role->GetName());
	if (i != m_name2roleid.end())
	{
		m_name2roleid.erase(i);
	}

	auto it = m_nick2roleid.find(role->GetNickId());
	if (it != m_nick2roleid.end())
	{
		m_nick2roleid.erase(it);
	}
}

Role * RoleManager::FindBySession(SessionType sessionID)
{
	return m_RoleDatas.find<SessionIndex>(sessionID);
}

Role * RoleManager::FindByRoleID(UINT64 roleID)
{
	return m_RoleDatas.find<RoleIDIndex>(roleID);
}

Role * RoleManager::FindByName(const std::string& name)
{
	auto i = m_name2roleid.find(name);
	if (i == m_name2roleid.end())
	{
		return NULL;
	}
	return FindByRoleID(i->second);
}

Role* RoleManager::FindByNickID(UINT64 NickID)
{
	auto i = m_nick2roleid.find(NickID);
	if (i == m_nick2roleid.end())
	{
		return NULL;
	}
	return FindByRoleID(i->second);
}

Role* RoleManager::FindByRoleIDOrNickID(UINT64 ID)
{
	Role* pRole = FindByRoleID(ID);
	if (NULL == pRole)
	{
		pRole = FindByNickID(ID);
	}
	return pRole;
}

void RoleManager::RemoveBySession(SessionType sessionID)
{
	Role *pRole = FindBySession(sessionID);
	if (pRole != NULL)
	{
		//SSDebug << "Remove Role " << pRole << " From RoleManager!" << END;
		if (pRole->GetFlag(RSF_ISWaitLogin))
		{
		}

		RemoveRole(pRole);
		pRole->Uninit();
		delete pRole;
	}
	else
	{
		SSDebug << "Remove role session [" << sessionID << "] but not found!" << END;
	}
}

static Role *GetRole(const std::string &RoleIDString)
{
	UINT64 RoleID = convert<UINT64>(RoleIDString);
	return RoleManager::Instance()->FindByRoleID(RoleID);
}

void RoleManager::_DumpRoles()
{
	std::stringstream ss;
	int k = 0;
	
	ss << "RoleManager dump:\n";
	auto i = m_RoleDatas.begin();
	auto end = m_RoleDatas.end();
	for (; i != end; ++i, ++k)
	{
		Role *pRole = *i;
		ss << k << ". " << pRole->GetName() << " [" << pRole->GetID() << "] session= " << Hex(pRole->GetSessionID());

		if (pRole->GetCurrScene())
		{
			ss << " In " << pRole->GetCurrScene() << "\n";
		}
		else if (pRole->GetLoadingScene())
		{
			ss << " Loading " << pRole->GetCurrScene() << "\n";
		}
		else
		{
			ss << "\n";
		}
	}

	ss << "Total " << m_RoleDatas.size() << " roles";
	SSDebug << ss.str() << END;
}

void RoleManager::Update(float delta)
{
	static float elapse = 0;

	elapse += delta;
	if (elapse > 60)
	{
		elapse -= 60;
	}

	ForeachCall(m_RoleDatas, Update, delta);
	
	///>SSWarn << "RoleManager Update Here!!" << END;
}

void RoleManager::ConvertSession(Role* pRole, SessionType oldSession, SessionType newSession)
{
	if (pRole == NULL)
	{
		return;
	}
	LogInfo("role[%llu] name[%s] convert session from[%llu] to[%llu]", pRole->GetID(), pRole->GetName().c_str(), oldSession, newSession);
	RemoveRole(pRole);
	pRole->SetSessionID(newSession);
	AddRole(pRole);
}

Role * RoleManager::CreateRole(const KKSG::RoleAllInfo *pInfo, UINT64 sessionID, int index, const KKSG::ClientInfo* poCliInfo, const EnterSceneInfo& info, UINT32 routerConnId)
{
	Role *pRole = new Role;
	// 如果是跨服GS，记录来的router连接id
	pRole->SetRouterConnId(routerConnId);
	pRole->Init(*pInfo, sessionID, index, info);
	pRole->AddListener(&RoleSummaryHandler::GlobalHandler);
	pRole->AddListener(&RoleCommonHandler::GlobalHandler);
	pRole->AddListener(&OpenSystemHandler::GlobaHandler);
	pRole->AddListener(&AChievementHandler::GlobalHandler);
	pRole->AddListener(&CActivityHandler::GlobalHandler);
	pRole->AddListener(&SpActivityHandler::GlobalHandler);	
	pRole->AddListener(&ReportDataHandler::GlobalHandler);
	AddRole(pRole);

	pRole->SetClientConf(*poCliInfo);
	
	return pRole;
}

void RoleManager::Foreach(IRoleTravelCallback *cb, void *parg)
{
	foreach(i in m_RoleDatas)
	{
		cb->OnIteratorRole(*i, parg);
	}
}

void RoleManager::BroadCast(const CProtocol& ptc)
{
	foreach(i in m_RoleDatas)
	{
		Role* role= *i;
		if (NULL != role)
		{
			role->Send(ptc);
		}
	}
}
void RoleManager::LevelSealBroadCast(INT32 iType)
{
	//所有玩家解除封印并通知;
	foreach(i in m_RoleDatas)
	{
		Role* role= *i;
		if (NULL != role)
		{
			CLevelSealMgr::Instance()->NotifyClient(role);
		}
	}
	return ;
}
bool RoleManager::UpdateOnDayPass()
{
	ForeachCall(m_RoleDatas, UpdateOnDayPass);
	return true;
}

void RoleManager::GetFilterRole(Role* me, FilterRole filter, std::vector<UINT64>& roles, std::vector<UINT64>& exroles)
{
	for (auto i = m_RoleDatas.begin(); i != m_RoleDatas.end(); ++i)
	{
		Role* role = *i;
		UINT32 ret = filter(me, role);
		if (0 == ret)
		{
			roles.push_back(role->GetID());
		}
		else if (1 == ret)
		{
			exroles.push_back(role->GetID());
		}
	}
}

void RoleManager::OnRouterClose(UINT32 line, UINT32 connId)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		return;
	}
	LogWarn("router[%u] conn[%u] close, kick all relation role", line, connId);

	std::list<Role*> removeRoles;
	for (auto it = Begin(); it != End(); ++it)
	{
		Role* pRole = *it;
		if (pRole->GetRouterConnId() != connId)
		{
			continue;
		}
		removeRoles.push_back(pRole);
	}

	for (auto it = removeRoles.begin(); it != removeRoles.end(); ++it)
	{
		Role* pRole = *it;
		SSWarn << "roleId: " << pRole->GetID() << ", name: " << pRole->GetName() << ", force leave cross gs" << END;
		pRole->OnLeaveGs(0);
		UINT64 qwRoleSessionID = pRole->GetSessionID();
		UINT64 roleId = pRole->GetID();
		RemoveBySession(qwRoleSessionID);
		RoleSummaryMgr::Instance()->RemoveRoleSummary(roleId);
	}
}

			
