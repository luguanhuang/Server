#include "pch.h"
#include "scenebase.h"
#include "scene.h"
#include "unit/role.h"
#include "entity/XRole.h"
#include "unit/enemy.h"
#include "unit/dummyrole.h"
#include "scene/scenecb.h"
#include "network/mslink.h"
#include "scene/scene.h"
#include "scene/ptcg2m_sceneemptynotify.h"
#include "battlewatch/ptcg2c_notifyroleempty2watcher.h"
#include "gamelogic/rolenethandler.h"
#include "timeutil.h"

union BattleIDun
{
	struct {
		UINT16 _svid;
		UINT16 _IncID;
		UINT32 _timestamp;
	} _;

	UINT64 id;
};

static UINT32 LastTime = 0;
static UINT16 BattleInc = 0;
static UINT32 sdwSceneID = (1 << 31);

namespace NumberAux
{
	bool isNAN(float x) { return x != x; }
	bool isInf(float x) { return !isNAN(x) && isNAN(x-x); }
	bool isInvalid(float x) { return isNAN(x) || isInf(x); }

	bool isNAN(double x) { return x != x; }
	bool isInf(double x) { return !isNAN(x) && isNAN(x-x); }
	bool isInvalid(double x) { return isNAN(x) || isInf(x); }

	bool isInvalid(const Vector3 &p)
	{
		return isInvalid(p.x) || isInvalid(p.y) || isInvalid(p.z);
	}
}

SceneBase::SceneBase()
{
	m_SceneID = INVALID_SCENE_ID;
	m_SyncMode = 0;
	m_BattleID = 0;
	m_bIsStaticScene = false;
	m_pSceneInfo = NULL;
	m_dontDestory = false;

	m_PrepareList = NULL;
	m_pUnitHandler = NULL;

	m_EnemyCount = 0;
	m_RoleCount = 0;
	m_DummyRoleCount = 0;

    m_IsSameServerID = true;
    m_FirstRoleServerid = 0;
    m_EnterRoleCount = 0;


	UINT32 now = (UINT32)time(NULL);
	if (now != LastTime)
	{
		LastTime = now;
		BattleInc = 0;
	}

	BattleIDun un;
	un._._svid = (UINT16)GSConfig::Instance()->GetServerID();
	un._._IncID = BattleInc++;
	un._._timestamp = LastTime;
	m_BattleID = un.id;

	m_hasRoleLoaded = false;
	m_allDisconnect = false;
	m_allDisconnectTime = 0;
}

#define SafeDeleate(p) do {if (p != NULL) { delete p; p = NULL; }} while(0)

SceneBase::~SceneBase()
{

}

void SceneBase::Init(UINT32 sceneID, SceneInfo *pSceneInfo)
{
	m_SceneID = sceneID;
	m_pSceneInfo = pSceneInfo;
	m_SyncMode = m_pSceneInfo->m_pConf->syncMode;
	m_bIsStaticScene = m_pSceneInfo->m_pConf->IsStaticScene;
	if (m_bIsStaticScene)
	{
		m_dontDestory = true;
	}

	// collide
	m_Collide.SetScene(this);

	m_PrepareList = IScenePrepareList::CreateScenePrepareList(m_pSceneInfo);
	m_PrepareList->SetListener(this);

	m_pUnitHandler = new LineHandler(this);
}

void SceneBase::Uninit()
{
	if (m_pUnitHandler)m_pUnitHandler->Uninit();
	SafeDeleate(m_pUnitHandler);

	SafeDeleate(m_PrepareList);
}

void SceneBase::Update(float deltatime)
{
	if (m_PrepareList)
	{
		m_PrepareList->Update();
	}

	if(NULL != m_pUnitHandler)
	{
		m_pUnitHandler->Update(deltatime);
	}
}

const char* SceneBase::SceneTypeStr(UINT32 dwType)
{
	switch(dwType)
	{
		///> 主城 1 （非战斗）
	case KKSG::SCENE_HALL: return "[Hall]";
		///> 工会大厅 10（非战斗）
	case KKSG::SCENE_GUILD_HALL: return "[GuildHall]";
		///> 家园 15（非战斗）
	case KKSG::SCENE_FAMILYGARDEN: return "[FamilyGarden]";
		///> 天空竞技场等待场景 24（非战斗）
	case KKSG::SKYCITY_WAITING: return "[SkyCityWait]";
		///> 单机关卡 2（单机）
	case KKSG::SCENE_BATTLE: return "[Battle]";
		///> 竞技场 5（单机）
	case KKSG::SCENE_ARENA: return "[Arena]";
		///> 世界boss 7
	case KKSG::SCENE_WORLDBOSS: return "[WorldBoss]";
		///> 工会boss 11
	case KKSG::SCENE_GUILD_BOSS: return "[GuildBoss]";
		///> bossrush 9
	case KKSG::SCENE_BOSSRUSH: return "[BossRush]";
		///> 巢穴 3
	case KKSG::SCENE_NEST: return "[Nest]";
		///> 深渊 13
	case KKSG::SCENE_ABYSSS: return "[Abyss]";
		///> 黑暗神殿 16
	case KKSG::SCENE_TOWER: return "[Tower]";
		///> 龙本 18
	case KKSG::SCENE_DRAGON: return "[Dragon]";
		///> 女神试炼 20
	case KKSG::SCENE_GODDESS: return "[GodNess]";
		///> 龙之远征 21
	case KKSG::SCENE_DRAGON_EXP: return "[DragonExp]";
		///> 大冒险 22
	case KKSG::SCENE_RISK: return "[Risk]";
		///> 无尽深渊 23
	case KKSG::SCENE_ENDLESSABYSS: return "[Endless]";
		///> 转职试炼 26
	case KKSG::SCENE_PROF_TRIALS: return "[PROF_TRIALS]";
		///> 天梯赛 12 PVP
	case KKSG::SCENE_PK: return "[PK]";
		///> 天梯赛2v2 46 PVP
	case KKSG::SCENE_PKTWO: return "[PKTWO]";
		///> 全民PK PVP
	case KKSG::SCENE_CUSTOMPK: return "[CustomPK]";
		///> 全民PK PVP
	case KKSG::SCENE_CUSTOMPKTWO: return "[CustomPKTwo]";
		///> 保卫队长 17 PVP
	case KKSG::SCENE_PVP: return "[PVP]";
		///> 工会擂台战 19 PVP
	case KKSG::SCENE_GMF: return "[GMF]";
		///> 工会擂台积分战 27 PVP
	case KKSG::SCENE_GPR: return "[GPR]"; 
		///> 天空竞技场 25 PVP
	case KKSG::SKYCITY_FIGHTING: return "[SkyCityFight]";
		///> 矿脉争夺战 29 
	case KKSG::SCENE_RESWAR_PVE: return "[ResWardPVE]";
		///> 矿脉争夺战 28 
	case KKSG::SCENE_RESWAR_PVP: return "[ResWardPVP]";
		///> 30 
	case KKSG::SCENE_GUILD_CAMP: return "[GuildCamp]";
		///> 31 
	case KKSG::SCENE_AIRSHIP: return "[AirShip]";
		///> 32
	case KKSG::SCENE_WEEK_NEST: return "[WeekNest]";
		///> 34
	case KKSG::SCENE_HORSE: return "[horce]";
		///> 35
	case KKSG::SCENE_HORSE_RACE: return "[horcerace]";
		///> 36
	case KKSG::SCENE_HEROBATTLE: return "[herobattle]";
		///> 37
	case KKSG::SCENE_INVFIGHT: return "[invfight]";
		///> 38
	case KKSG::SCENE_CASTLE_WAIT: return "[castlewait]";
		///> 39
	case KKSG::SCENE_CASTLE_FIGHT: return "[castlefight]";
		///> 40
	case KKSG::SCENE_LEAGUE_BATTLE: return "[leaguebattle]";
		///> 41
	case KKSG::SCENE_ACTIVITY_ONE: return "[activity_one]";
		///> 42
	case KKSG::SCENE_ACTIVITY_TWO: return "[activity_two]";
		///> 43
	case KKSG::SCENE_ACTIVITY_THREE: return "[activity_three]";
	case KKSG::SCENE_ABYSS_PARTY:return "[abyssparty]";
        ///> 48
    case KKSG::SCENE_WEEKEND4V4_MONSTERFIGHT: return "[weekend4v4_monsterfight]";
        ///> 49
    case KKSG::SCENE_WEEKEND4V4_GHOSTACTION: return "[weekend4v4_ghostaction]";
        ///> 54
    case KKSG::SCENE_WEEKEND4V4_DUCK: return "[weekend4v4_duck]";
        ///> 50
    case KKSG::SCENE_WEEKEND4V4_LIVECHALLENGE: return "[weekend4v4_livechallenge]";
        ///> 51
    case KKSG::SCENE_WEEKEND4V4_CRAZYBOMB: return "[weekend4v4_crazybomb]";
        ///> 52
    case KKSG::SCENE_WEEKEND4V4_HORSERACING: return "[weekend4v4_horseracing]";
	    ///> 64
	case KKSG::SCENE_BIOHELL: return "[biohell]";
		///> 65
	case KKSG::SCENE_DUCK: return "[crazyduck]";
		///> 66
	case KKSG::SCENE_COUPLE: return "[couple]";
		///> 69
	case KKSG::SCENE_COMPETEDRAGON: return "[CompeteDragon]";
		///> 70
	case KKSG::SCENE_SURVIVE: return "[survive]";
		///> 71
	case KKSG::SCENE_LEISURE: return "[leisure]";
	default: return "[Unknow]";
	}
}

const char* SceneBase::SceneTypeStr()
{
	return SceneTypeStr(m_pSceneInfo->m_pConf->type);
}

UINT32 NewSceneID()
{
	if(sdwSceneID == 0)
	{
		sdwSceneID = (1 << 31);
	}
	return sdwSceneID++;
}

void SceneBase::AddUnitAndNotify(Unit *pUnit)
{
	if (pUnit->IsRole())
	{
		AddRoleAndNotify((Role *)pUnit);
	}
	else
	{
		AddNonRoleAndNotify(pUnit);
	}
}

///> 角色进入场景，要等客户端Load场景成功了，回复之后才真正放入场景
void SceneBase::AddRoleAndNotify(Role *pRole)
{
	std::pair<std::map<UINT64, Role*>::iterator, bool> ret = m_RoleWaitLoadScene.insert(std::make_pair(pRole->GetID(), pRole));
	assert(ret.second);

	m_PrepareList->AddPrepareRole(pRole->GetID(), pRole->GetName(), pRole->IsWatcher());

	///> dispatch enter scene
	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnPreEnterScene((Scene*)this, pRole);
	}

	pRole->OnPreEnterScene((Scene*)this);

	SSInfo<<"add role to prepare list, role:"<<pRole->GetID()<<" name:"<<pRole->GetName()<<" scene:"<<GetSceneTemplateID()<<" sceneuid:"<<GetSceneID()<<END;
}

void SceneBase::AddNonRoleAndNotify(Unit *pUnit)
{
	pUnit->EnterScene((Scene*)this);
	m_pUnitHandler->AddUnitAndNotify(pUnit);

	if (pUnit->IsEnemy())
	{
		++m_EnemyCount;

	}
	else if (pUnit->IsDummyRole())
	{
		++m_DummyRoleCount;

	}

	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnAddNonRole((Scene*)this, pUnit);
	}
}

///> 客户端load成功，服务器preparelist处理
void SceneBase::AddRoleAfterLoadedScene(Role *pRole, UINT32 delayid)
{
	if (!m_PrepareList->ChangeRoleState(pRole->GetID(), Loaded, delayid))
	{
		SSWarn<<"change role state failed, roleid:"<<pRole->GetID()<<END;
		return;
	}
}

void SceneBase::OnEnterSceneFinish()
{

}

bool SceneBase::OnClientLoaded(UINT64 qwRoleID, UINT32 delayid)
{
	auto i = m_RoleWaitLoadScene.find(qwRoleID);
	if (i == m_RoleWaitLoadScene.end())
	{
		SSWarn<<"client loaded, roleid:"<<qwRoleID<<" not find"<<END;
		return false;
	}
	Role *pRole = i->second;
	m_RoleWaitLoadScene.erase(i);

	pRole->EnterScene((Scene*)this);
	if (!pRole->IsWatcher())
	{
		++m_RoleCount;

        if(0 == m_FirstRoleServerid)
        {
            m_FirstRoleServerid = pRole->GetFromServerId();
        }else{
            if(m_FirstRoleServerid != pRole->GetFromServerId())
            {
                m_IsSameServerID = false;
            }
        }
        m_EnterRoleCount++;
	}

	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnPreEnterSceneFinally((Scene*)this, pRole);
	}

	// reply do enter scene
	ClientLoadedInfo info;

	CUserData data;
	info.role = pRole;
	info.scene = (Scene*)(this);
	data.m_pUserPtr = (void*)(&info);

	CRpc::ReplyDelayRpc(delayid, data);	

	// notify scene unit
	m_pUnitHandler->AddUnitAndNotify(pRole);

	///> dispatch enter scene finally 
	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnEnterSceneFinally((Scene*)this, pRole);
	}

	m_hasRoleLoaded = true;
	return true;
}

void SceneBase::RemoveUnitAndNotify(Unit *pUnit)
{
	if (pUnit->IsRole())
	{
		Role* role = (Role*)(pUnit);

		if (role->IsLeavingScene())
		{
			SSError << " is leaving scene role = " << role->GetID() << " mapid = " << GetSceneTemplateID() << " sceneid = " << GetSceneID() << END;
		}
		role->SetLeavingScene(true);

		if (!role->IsWatcher())
		{
			--m_RoleCount;
			if (m_RoleCount <= 0)
			{
				PtcG2C_NotifyRoleEmpty2Watcher ntf;
				BroadcastWatcher(ntf);
			}
			if (IsSceneNoRole())
			{
				OnSceneEmpty();
			}
		}

		RemoveWaitingRole(role);

		///> dispatch leave scene
		for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
		{
			(*i)->OnLeaveScene((Scene*)this, role);
		}
	}
	else if(pUnit->IsEnemy())
	{
		--m_EnemyCount;
	}
	else if(pUnit->IsDummyRole())
	{
		--m_DummyRoleCount;
	}

	pUnit->LeaveScene();

	m_pUnitHandler->RemoveUnitAndNotify(pUnit);

	pUnit->ResetFightGroup();

	if (pUnit->IsRole())
	{
		((Role*)(pUnit))->SetLeavingScene(false);
	}

	///> dispatch leave scene
	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnRemoveUnit((Scene*)this, pUnit);
	}
}

void SceneBase::RemoveWaitingRole(Role *pRole)
{
	bool waitFlag = false;
	auto i = m_RoleWaitLoadScene.find(pRole->GetID());
	if (i != m_RoleWaitLoadScene.end())
	{
		waitFlag = true;
		if (IsSceneNoRole())
		{
			OnSceneEmpty();
		}
		m_RoleWaitLoadScene.erase(i);
	}
	m_PrepareList->RemovePrepareRole(pRole->GetID());

	if(waitFlag)
	{
		for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
		{
			(*i)->OnRemoveWaitingRole((Scene*)this, pRole);
		}

		SSInfo<<"remove role from prepare list, role:"<<pRole->GetID()<<" name:"<<pRole->GetName()<<" scene:"<<GetSceneTemplateID()<<" sceneuid:"<<GetSceneID()<<END;
	}
}

bool SceneBase::IsSceneNoRole()
{
	if (!m_RoleWaitLoadScene.empty())
	{
		return false;
	}
	if (m_pUnitHandler->GetWatcherNum())
	{
		return false;
	}
	if (m_RoleCount > 0)
	{
		return false;
	}
	return true;
}

void SceneBase::OnSceneEmpty()
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		PtcG2M_SceneEmptyNotify emptyntf;
		emptyntf.m_Data.set_mapid(GetSceneTemplateID());
		emptyntf.m_Data.set_sceneuid(GetSceneID());
		MSLink::Instance()->SendTo(emptyntf);
	}
}

bool SceneBase::CheckDestory()
{
	if (m_dontDestory)
	{
		return false;
	}
	if (IsSceneNoRole())
	{
		return true;
	}
	return false;
}

bool SceneBase::CheckMoveReach( Unit *pUnit, const Vector3& stPos, const Vector3 &delta )
{
	if (NumberAux::isInvalid(delta))
	{
		return 0;
	}

	// 移动直达阻挡检测
	float step = 1.0f;
	MoveCollideParam param(pUnit, stPos, delta);
	m_Collide.MoveCollide(param, step);

	return (1.0 == step);
}

bool SceneBase::TryMove(Unit *pUnit, const Vector3 &delta)
{
	if (NumberAux::isInvalid(delta))
	{
		return 0;
	}

	// 移动主档碰撞
	float step = 1.0f;
	MoveCollideParam param(pUnit, delta);
	m_Collide.MoveCollide(param, step);

	return (1.0f == step);
}

bool SceneBase::SetPosition(Unit *pUnit, const Vector3 &position)
{
	if (NumberAux::isInvalid(position))
	{
		return false;
	}

	StandCollideParam param(pUnit, position);
	if (m_Collide.StandCollide(param))
	{
		return false;
	}

	float y = m_Collide.GetHeight(position.x, position.z);
	if (position.y > y)
	{
		y = position.y;
	}

	pUnit->GetXObject()->SetPosition(position.x, y, position.z);
	return true;
}

void SceneBase::GetRolesCanSeeSorted(Role* role, std::vector<Role*>& roles)
{
	std::vector<Role*> temproles;
	m_pUnitHandler->GetRolesCanSee(role, temproles);

	std::set<Role*> pushed;
	for (auto i = temproles.begin(); i != temproles.end(); ++i)
	{
		Role* role = *i;
		if (role->GetRoleView()->GetIsNeedPre())
		{
			Role* bindrole = IsInScene(role->GetRoleView()->GetBindRole());
			if (NULL != bindrole)
			{
				bool isin = false;
				for (auto j = i; j != temproles.end(); ++j)
				{
					if (*j == bindrole)
					{
						isin = true;
						break;
					}
				}
				if (isin)
				{
					auto i = pushed.find(bindrole);
					if (i == pushed.end())
					{
						roles.push_back(bindrole);
						pushed.insert(bindrole);
					}
				}
			}
		}
		auto k = pushed.find(role);
		if (k == pushed.end())
		{
			roles.push_back(role);
			pushed.insert(role);
		}
	}
}

bool SceneBase::IsAllRoleDisconnect()
{
	bool dis = true;
	const std::list<Role*>& allroles = GetAllRoles();
	for (auto it = allroles.begin(); it != allroles.end(); ++it)
	{
		Role* pRole = *it;
		if (pRole->GetFlag(RSF_ISReconnecting) == false)
		{
			dis = false;
			break;
		}
	}

	if (m_allDisconnect && dis == false)
	{
		m_allDisconnectTime = 0;
	}
	else if (m_allDisconnect == false && dis)
	{
		m_allDisconnectTime = TimeUtil::GetTime();
	}
	m_allDisconnect = dis;
	return dis;
}

bool SceneBase::IsPosInvalid(const Vector3& pos)
{
	if (NULL == GetGrid())
	{
		return false;
	}
	float y = 0.0f;
	if (!GetGrid()->TryGetHeight(pos.x, pos.z, y))
	{
		SSWarn<<"invalid pos, x:"<<pos.x<<" z:"<<pos.z<<" y:"<<y<<END;
		return false;
	}
	if (y < 0.0f)
	{
		SSWarn<<"invalid pos, x:"<<pos.x<<" z:"<<pos.z<<" y:"<<y<<END;
		return false;
	}
	return true;
}
