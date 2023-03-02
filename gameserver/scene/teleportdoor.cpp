#include "pch.h"
#include "teleportdoor.h"
#include "unit/role.h"
#include "scene.h"
#include "entity/XObject.h"
#include "entity/XEntity.h"
#include "common/ptcg2c_teleportnotice.h"
#include "scenefinder.h"
#include "scene/scenedragon.h"
#include "battle/ptcg2c_ontransferwall.h"
#include "sceneteam.h"
#include "scenecastle.h"
#include "gamelogic/XLevelWave.h"
#include "scene/scenecompetedragon.h"

TeleportBase::TeleportBase(const Vector3& telePos , float teleR , const Vector3& desPos , float desFace , Scene* scene)
{
	m_telePos = telePos;
	m_teleR = teleR;
	m_desPos = desPos;
	m_desFace = desFace;
	
	m_scene = scene;

	m_isRunning = true;
	m_levelCallBackNo = 0;
}

TeleportBase::~TeleportBase()
{
}

void TeleportBase::SetLevelScriptFunc(std::string& func)
{
	m_levelScriptFunc = func;
}

bool TeleportBase::InTeleportDoor(const Vector3& pos )
{
	//pos.y = m_telePos.y;//忽略高度对距离的影响 只考虑平面
	if(Vector3::Distance(pos , m_telePos) <= m_teleR)
	{
		return true;
	}
	return false;
}


TeleportDoor::TeleportDoor( const Vector3& telePos , float teleR , const Vector3& desPos , float desFace , Scene* scene)
	:TeleportBase(telePos, teleR, desPos, desFace, scene)
{
	if (m_scene->GetSceneType() == KKSG::SCENE_AIRSHIP)
	{
		m_deadTime = time(NULL) + 10;
	}
	else
	{
		m_deadTime = time(NULL) + 40;//存活时间40秒
	}
}

void TeleportDoor::Update()
{
	if(!IsFinished())
	{
		if((time(NULL) > m_deadTime) || CheckCondition())
		{
			EndNotice();
			DoTeleport();
			m_isRunning = false;
			OnFinish();
		}else
		{
			CheckNotice();
		}
	}
}

bool TeleportDoor::IsFinished()
{
	return (!m_isRunning);
}

void TeleportDoor::SetLevelCallBack(int callbackNo)
{
	m_levelCallBackNo = callbackNo;
}

bool TeleportDoor::CheckCondition()
{
	if(m_scene)
	{
		const std::list<Role*>& roles = m_scene->GetAllRoles();
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			if(role && role->GetXObject() && !role->IsDead())
			{
				if(!InTeleportDoor(role->GetXObject()->GetPosition()))
				{
					return false;
				}
			}
		}
		if (m_scene->GetSceneType() == KKSG::SCENE_DRAGON && m_desPos == Vector3(0, 0, 0))
		{
			return m_scene->GetSceneDragon()->CheckTeleportChangeScene();
		}
	}
	return true;
}

void TeleportDoor::DoTeleport()
{
	if(m_scene)
	{
		if (m_desPos == Vector3(0, 0, 0))
		{
			if (m_scene->GetSpawner())
			{
				m_scene->GetSpawner()->GetLevelScript()->AddRoleExecutedScript(m_levelScriptFunc);
			}

			OnSceneTrans();
			return;
		}
		const std::list<Role*>& roles = m_scene->GetAllRoles();
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			if(!role)
			{
				continue;
			}

			XEntity* entity = dynamic_cast<XEntity*>(role->GetXObject());

			if(!entity)
			{
				continue;
			}

			entity->TransferLocation(m_desPos, m_desFace);
		}

		FindDummyRoleFightGroupCondition cond((UINT32)(KKSG::FightRole));;
		std::vector<Unit*> dummys;
		m_scene->GetUnitByCondition(cond , dummys);
		for(size_t i = 0; i < dummys.size(); ++i)
		{
			Unit* dummy = dummys[i];
			if(!dummy || !dummy->IsDummyRole())
			{
				continue;
			}

			XEntity* entity = dynamic_cast<XEntity*>(dummy->GetXObject());
			if(!entity)
			{
				continue;
			}

			entity->TransferLocation(m_desPos , m_desFace );
		}

		if (m_scene->GetSpawner())
		{
			m_scene->GetSpawner()->GetLevelScript()->AddRoleExecutedScript(m_levelScriptFunc);
		}
	}
}

void TeleportDoor::CheckNotice()
{
	const std::list<Role*>& roles = m_scene->GetAllRoles();
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		if(role && InTeleportDoor(role->GetXObject()->GetPosition()) && m_noticeHistory.find(role->GetID()) == m_noticeHistory.end())
		{
			PtcG2C_TeleportNotice tn;
			tn.m_Data.set_onnotice(true);
			role->Send(tn);
			m_noticeHistory.insert(role->GetID());
		}
	}
}

void TeleportDoor::EndNotice()
{
	PtcG2C_TeleportNotice tn;
	tn.m_Data.set_onnotice(false);
	m_scene->Broadcast(tn);
}

void TeleportDoor::OnFinish()
{
	if (m_levelCallBackNo && m_scene->GetSpawner() && m_scene->GetRoleCount())
	{
		m_scene->GetSpawner()->OnCallBackFuncFinish(m_levelCallBackNo);
	}
}

void TeleportDoor::OnSceneTrans()
{
	if (m_scene->GetSceneDragon())//龙本
	{
		m_scene->GetSceneDragon()->ChangeScene();
	}
	else if (m_scene->GetSceneCompeteDragon())//竞技龙本
	{
		m_scene->GetSceneCompeteDragon()->ChangeScene();
	}
	else if (m_scene->GetSceneType() == KKSG::SCENE_AIRSHIP) //飞艇
	{
		if(m_scene->GetSceneTeam())
		{
			m_scene->GetSceneTeam()->ChangeScene();
		}
	}
}

TeleportOne::TeleportOne(const Vector3& telePos , float teleR , const Vector3& desPos , float desFace , Scene* scene)
	:TeleportBase(telePos, teleR, desPos, desFace, scene)
{

}

void TeleportOne::Update()
{
	CheckDelay(); 

	const std::list<Role*>& roles = m_scene->GetAllRoles();
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		if(role && role->GetXObject() && !role->IsDead() && !InDelay(role->GetID()))
		{
			if(InTeleportDoor(role->GetXObject()->GetPosition()))
			{
				DoTeleport(role);
			}
		}
	}
}

bool TeleportOne::IsFinished()
{
	return false;//一直存在
}

void TeleportOne::DoTeleport(Role* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}

	AddDelay(pRole->GetID());

	if (m_desPos == Vector3(0, 0, 0))
	{
		if(m_scene->GetCastleHandler())
		{
			//发协议
			m_scene->GetCastleHandler()->BackToReady(pRole->GetID());
		}
		return;
	}

	XEntity* entity = dynamic_cast<XEntity*>(pRole->GetXObject());

	if(!entity)
	{
		return ;
	}

	entity->TransferLocation(m_desPos, m_desFace);
}

void TeleportOne::AddDelay(UINT64 roleID)
{
	m_delay[roleID] = time(NULL);
}

bool TeleportOne::InDelay(UINT64 roleID)
{
	return (m_delay.find(roleID) != m_delay.end());	
}

void TeleportOne::CheckDelay()
{
	time_t now = time(NULL);
	for(auto it = m_delay.begin(); it != m_delay.end(); )
	{
		if(it->second + 5 < now)//5s
		{
			m_delay.erase(it++);
		}
		else
		{
			it++;
		}
	}
}
