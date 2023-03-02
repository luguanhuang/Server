#include "pch.h"
#include "linehandler.h"
#include "unit/role.h"
#include "sender.hpp"
#include "unit/enemy.h"
#include "unit/unit.h"
#include "scenefinder.h"
#include "entity/XObject.h"
#include "unit/enemymanager.h"
#include "foreach.h"
#include "component/XActionSender.h"
#include "scene/ptcg2c_unitappear.h"
#include "scene/ptcg2c_unitdisappear.h"
#include "component/XNetComponent.h"
#include "sceneline.h"
#include "sceneviewline.h"
#include "scenegroupline.h"
#include "scenebase.h"

#define FIRST_LINE 0

std::list<Role*> LineHandler::m_emptyrole;

LineHandler::LineHandler(SceneBase* scenebase)
{
	m_syncmode = SyncMode_Common;
	auto i = GetGlobalConfig().ViewGridScene.find(scenebase->GetSceneType());
	bool isviewgrid = (i != GetGlobalConfig().ViewGridScene.end());
	if (isviewgrid)
	{
		m_syncmode = SyncMode_View;
	}
	else
	{
		auto i = GetGlobalConfig().ViewGroupScene.find(scenebase->GetSceneType());
		bool isviewgroup = (i != GetGlobalConfig().ViewGroupScene.end());
		if (isviewgroup)
		{
			m_syncmode = SyncMode_Group;
		}
	}	

	m_scene = scenebase;
	m_CurrenMaxLineID = 0;
	UINT32 maxRole = scenebase->GetSceneInfo()->m_pConf->LineRoleCount;
	m_MaxRoleInLine = (0 == maxRole) ? (UINT32)(-1) : maxRole;

	m_allrole = new RoleSet();
}

LineHandler::~LineHandler()
{
	std::unordered_map<UINT64, Unit*> temp;
	temp.swap(m_NonRolesInScene);
	foreach(i in temp)
	{
		Unit *pUnit = i->second;
		pUnit->CleanUpInScene();
	}

	if (NULL != m_allrole)
	{
		delete m_allrole;
		m_allrole = NULL;
	}
}

SceneLine* LineHandler::GetAvaliableLine()
{
	switch(m_syncmode)
	{
	case SyncMode_Common:
		{
			foreach(i in m_Lines)
			{
				SceneLine *Line = *i;
				if (!Line->IsFull(m_MaxRoleInLine))
				{
					return Line;
				}
			}
			SceneLine *pline = new SceneLine();
			pline->m_ID = m_CurrenMaxLineID++;
			m_Lines.push_back(pline);
			return pline;
			break;
		}
	case SyncMode_View:
		{
			if (m_Lines.empty())
			{
				SceneLine* pline = new SceneViewLine(m_scene->GetGrid());
				pline->m_ID = m_CurrenMaxLineID++;
				m_Lines.push_back(pline);
				return pline;
			}
			else
			{
				return *m_Lines.begin();
			}
			break;
		}
	case SyncMode_Group:
		{
			if (m_Lines.empty())
			{
				SceneLine* pline = new SceneGroupLine();
				pline->m_ID = m_CurrenMaxLineID++;
				m_Lines.push_back(pline);
				return pline;
			}
			else
			{
				return *m_Lines.begin();
			}
			break;
		}
	default:
		break;
	}
	return NULL;
}

SceneLine* LineHandler::GetAvaliableLine(UINT32 leftslot)
{
	if (IsCommonSync())
	{
		foreach(i in m_Lines)
		{
			SceneLine *Line = *i;
			if (Line->HasSlot(m_MaxRoleInLine, leftslot))
			{
				return Line;
			}
		}

		SceneLine *pline = new SceneLine();
		pline->m_ID = m_CurrenMaxLineID++;
		m_Lines.push_back(pline);
		return pline;
	}
	return NULL;
}

bool LineHandler::AddUnitLine(Unit* pUnit, SceneLine* line)
{
	const std::pair<std::unordered_map<UINT64, SceneLine*>::iterator, bool> ret = m_Unit2Line.insert(std::make_pair(pUnit->GetID(), line));
	assert(ret.second);
	return ret.second;
}

SceneLine* LineHandler::GetUnitLine(UINT64 uid)
{
	auto i = m_Unit2Line.find(uid);
	if (i == m_Unit2Line.end())
	{
		//SSWarn << "not found " << uid << " in scene " << m_scene << END;
		return NULL;
	}

	return i->second;
}

void LineHandler::AddUnitAndNotify(Unit *pUnit)
{
	if (pUnit->IsRole())
	{
		AddRoleAndNotify((Role*)pUnit);
	}
	else
	{
		AddNonRoleAndNotify(pUnit);
	}
}

void LineHandler::AddRoleAndNotify(Role *pRole)
{
	if (pRole->IsWatcher())
	{
		m_Wathcers[pRole->GetID()] = pRole;	
		AddWatcherAndNotify(pRole);
		return;
	}
	SceneLine *pLine = GetAvaliableLine();
	AddToLine(pRole, pLine);

	m_allrole->AddRole(pRole);
}

void LineHandler::AddWatcherAndNotify(Role *pRole)
{
	PtcG2C_UnitAppear oAppearToMePtc;
	int count = 0;
	for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
	{
		SceneLine* line = *i;
		for (auto k = line->m_role2viewset.begin(); k != line->m_role2viewset.end(); ++k)
		{
			Role* role = k->second->m_role;
			KKSG::UnitAppearance *pAppearance = oAppearToMePtc.m_Data.add_units();
			role->GetAppearance(*pAppearance);
			++count;
		}
		for (auto l = line->m_units.begin(); l != line->m_units.end(); ++l)
		{
			Unit* unit = l->second;
			KKSG::UnitAppearance *pAppearance = oAppearToMePtc.m_Data.add_units();
			unit->GetAppearance(*pAppearance);
			++count;
		}
	}
	for (auto j = m_NonRolesInScene.begin(); j != m_NonRolesInScene.end(); ++j)
	{
		KKSG::UnitAppearance *pAppearance = oAppearToMePtc.m_Data.add_units();
		j->second->GetAppearance(*pAppearance);
		++count;
	}

	if (count > 0)
	{
		pRole->Send(oAppearToMePtc);
	}
}

void LineHandler::AddAndNotify(Unit* pUnit)
{
	std::pair<std::unordered_map<UINT64, Unit*>::iterator, bool> ret = m_NonRolesInScene.insert(make_pair(pUnit->GetID(), pUnit));
	if (!ret.second)
	{
		SSError<<"unit has exist, uid:"<<pUnit->GetID()<<END;
	}

	RoleAppearNotifySender oSender;
	oSender.Start(pUnit, true);
	foreach (i in m_Lines)
	{
		oSender.SetNotifyMapNew((*i)->m_role2viewset);
	}
	oSender.Stop();

	BroadcastWatcher(oSender.oAppearToOthersPtc);
}

void LineHandler::AddNonRoleAndNotify(Unit *pUnit)
{
	if (IsGroupSync())
	{
		SceneLine* line = GetAvaliableLine();
		line->AddUnit(pUnit);
		AddUnitLine(pUnit, line);
	}
	else
	{
		UINT64 hostid = pUnit->GetHostId();
		if (0 == hostid)
		{
			AddAndNotify(pUnit);
		}
		else
		{
			SceneLine* line = GetUnitLine(hostid);
			if (NULL == line)
			{
				AddAndNotify(pUnit);
				SSInfo<<"not find hostid:"<<hostid<<END;
			}
			else
			{
				line->AddUnit(pUnit);
				AddUnitLine(pUnit, line);

				// appear
				RoleAppearNotifySender oSender;
				oSender.Start(pUnit, true);
				oSender.SetNotifyMapNew(line->m_role2viewset);
				oSender.Stop();

				if (FIRST_LINE == line->m_ID)
				{
					BroadcastWatcher(oSender.oAppearToOthersPtc);
				}	
			}
		}
	}
}

void LineHandler::RemoveUnitAndNotify(Unit *pUnit)
{
	if (pUnit->IsRole())
	{
		RemoveRoleAndNotify((Role*)pUnit);
	}
	else
	{
		RemoveNonRoleAndNotify(pUnit);
	}
}

void LineHandler::RemoveRoleAndNotify(Role *pRole)
{
	if (pRole->IsWatcher())
	{
		m_Wathcers.erase(pRole->GetID());
		return;
	}
	SceneLine *pLine = GetUnitLine(pRole->GetID());
	if (pLine != NULL)
	{
		RemoveFromLine(pRole, pLine);
	}
	else
	{
		SSWarn << "line handler remove role " << pRole << " failed! not found role in line" << END;
	}

	m_allrole->RemoveRole(pRole);
}

void LineHandler::RemoveNonRoleAndNotify(Unit *pUnit)
{
	SceneLine *pLine = GetUnitLine(pUnit->GetID());
	if (pLine == NULL)
	{
		PtcG2C_UnitDisappear oDisappearNtf;
		KKSG::UnitAppearance *pAppearance = &oDisappearNtf.m_Data;
		pAppearance->set_uid(pUnit->GetID());
		Broadcast(pUnit, oDisappearNtf, pUnit);

		auto i = m_NonRolesInScene.find(pUnit->GetID());
		if (i != m_NonRolesInScene.end())
		{
			m_NonRolesInScene.erase(pUnit->GetID());
		}
		else
		{
			SSWarn << "remove unit " << pUnit << " failed! not found unit" << END;
		}
	}
	else
	{
		PtcG2C_UnitDisappear oDisappearNtf;
		KKSG::UnitAppearance *pAppearance = &oDisappearNtf.m_Data;
		pAppearance->set_uid(pUnit->GetID());
		pLine->Broadcast(NULL, oDisappearNtf, pUnit);

		pLine->RemoveUnit(pUnit);
		m_Unit2Line.erase(pUnit->GetID());

		BroadcastWatcher(oDisappearNtf);
	}
}

void LineHandler::Uninit()
{
	SceneLine *pLine = NULL;
	for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
	{
		pLine = *i;
		delete pLine;
	}

	m_Lines.clear();
	m_Unit2Line.clear();
	m_Wathcers.clear();
}

void LineHandler::DumpLine(std::stringstream &ss)
{
	int k = 0;
	foreach(i in m_NonRolesInScene)
	{
		ss << "  " << k << ". " << i->second << " @ " << i->second->GetXObject()->GetPosition_p() 
			<< " dir: " << i->second->GetXObject()->GetFaceDegree() << "\n";
		++k;
	}

	ForeachCall(m_Lines, PrintLine, ss);
}

void LineHandler::Update(float deltatime)
{
	if (0.0l != m_scene->GetSceneInfo()->m_pConf->DPS[1])
	{
		for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
		{
			SceneLine* line = *i;
			for (auto j = line->m_role2viewset.begin(); j != line->m_role2viewset.end(); ++j)
			{
				Role* role = j->second->m_role;
				role->Statistics().UpdateDPS();
			}
		}
	}

	switch(m_syncmode)
	{
	case SyncMode_Common:
		{
			int count = 0;
			for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
			{
				SceneLine* line = (*i);
				XActionSender sender;
				///> 通用怪
				sender.PackageData(m_NonRolesInScene);
				///> 分线怪
				sender.PackageData(line->m_units);
				///> 分线玩家
				sender.PackageData(line->m_role2viewset);
				sender.Broadcast(line->m_role2viewset);

				///> 观战默认同步，第一条线上的玩家
				if (FIRST_LINE == count && !m_Wathcers.empty())
				{
					sender.Broadcast(m_Wathcers);
				}
				++count;
			}
			break;
		}
	case SyncMode_View:
		{
			for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
			{
				SceneLine* line = *i;
				///> 视野同步目前只考虑人
				line->Update();
			}
			break;
		}
	case SyncMode_Group:
		{
			for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
			{
				SceneLine* line = *i;
				line->Update();
			}
			break;
		}
	default:
		break;
	}
}

void LineHandler::BroadCastSameGroup(UINT32 group, const CProtocol& ptc)
{
	if (m_Lines.empty())
	{
		return;
	}
	m_Lines[0]->BroadcastSameGroup(group, ptc);
}

void LineHandler::BroadCastCanBeSee(Unit* sender, const CProtocol& ptc)
{
	SceneLine *pLine = NULL;
	if (NULL != sender)
	{
		pLine = GetUnitLine(sender->GetID());
	}
	if (pLine != NULL)
	{
		pLine->BroadcastCanBeSee(sender, ptc);
	}
}

void LineHandler::Broadcast(Unit *poSender, const CProtocol &roPtc, Unit *pExludeUnit)
{
	BroadcastExWatcher(poSender, roPtc, pExludeUnit);
	BroadcastWatcher(poSender, roPtc);
}

void LineHandler::Broadcast(const CProtocol &roPtc, Unit *pExludeUnit)
{
	BroadcastExWatcher(NULL, roPtc, pExludeUnit);
	BroadcastWatcher(NULL, roPtc);
}

void LineHandler::BroadcastExWatcher(const CProtocol &roPtc, Unit *pExludeUnit)
{
	foreach (i in m_Lines)
	{
		(*i)->Broadcast(NULL, roPtc, pExludeUnit);
	}
}

void LineHandler::BroadcastExWatcher(Unit *poSender, const CProtocol &roPtc, Unit *pExludeUnit)
{
	SceneLine *pLine = NULL;
	if (NULL != poSender)
	{
		pLine = GetUnitLine(poSender->GetID());
	}
	if (pLine != NULL)
	{
		pLine->Broadcast(poSender, roPtc, pExludeUnit);
	}
	else
	{
		foreach (i in m_Lines)
		{
			(*i)->Broadcast(poSender, roPtc, pExludeUnit);
		}
	}
}

void LineHandler::BroadcastWatcher(Unit* poSender, const CProtocol &roPtcL)
{
	SceneLine *pLine = NULL;
	if (NULL != poSender)
	{
		pLine = GetUnitLine(poSender->GetID());
	}
	if (NULL == pLine || FIRST_LINE == pLine->m_ID)
	{
		for (auto i = m_Wathcers.begin(); i != m_Wathcers.end(); ++i)	
		{
			Role* role = i->second;
			role->Send(roPtcL);
		}
	}
}

void LineHandler::BroadcastWatcher(const CProtocol &roPtcL)
{
	for (auto i = m_Wathcers.begin(); i != m_Wathcers.end(); ++i)	
	{
		Role* role = i->second;
		role->Send(roPtcL);
	}
}

void LineHandler::GetUnitByCondition(IFindUnitCondition &cond, std::vector<Unit *> &output)
{
	foreach(i in m_NonRolesInScene)
	{
		if (cond(i->second))
		{
			output.push_back(i->second);
		}
	}

	foreach (i in m_Lines)
	{
		SceneLine *pLine = *i;
		foreach (j in pLine->m_role2viewset)
		{
			if (cond(j->second->m_role))
			{
				output.push_back(j->second->m_role);
			}
		}
		foreach (k in pLine->m_units)
		{
			if (cond(k->second))
			{
				output.push_back(k->second);
			}
		}
	}
}

void LineHandler::ForEachPlayer(SceneRoleCB cb, void* arg)
{
	foreach(i in m_Lines)
	{
		SceneLine* line = *i;
		foreach(j in line->m_role2viewset)
		{
			cb(j->second->m_role, arg);
		}
	}
}

void LineHandler::GetDummyRoles(std::vector<DummyRole *> &output)
{
	foreach(i in m_NonRolesInScene)
	{
		if (i->second->IsDummyRole())
		{
			output.push_back((DummyRole*)i->second);
		}
	}
}

void LineHandler::GetSameLineUnits(Unit* unit, std::vector<Unit*> &units)
{
	foreach(i in m_NonRolesInScene)
	{
		units.push_back(i->second);
	}
	SceneLine* line= GetUnitLine(unit->GetID());
	if (NULL != line)
	{
		line->FillAll(units);
	}
	else
	{
		for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
		{
			SceneLine* line = *i;
			line->FillAll(units);
		}
	}
}

SceneLine* LineHandler::GetLine(UINT32 line)
{
	if (!IsCommonSync())
	{
		return NULL;
	}
	SceneLine* sceneline = NULL;
	for (auto i = m_Lines.begin(); i != m_Lines.end(); ++i)
	{
		if ((*i)->m_ID == line)
		{
			sceneline = *i;
			break;
		}
	}
	if (NULL == sceneline)
	{
		sceneline = new SceneLine();
		sceneline->m_ID = line;
		m_Lines.push_back(sceneline);
		///> 如果线越界，需要修改maxline
		if (line >= m_CurrenMaxLineID)
		{
			m_CurrenMaxLineID = line + 1;
		}
	}
	return sceneline;
}

void LineHandler::Disappear(Role* role, Unit* unit)
{
	PtcG2C_UnitDisappear oDisappearNtf;
	KKSG::UnitAppearance *pAppearance = &oDisappearNtf.m_Data;
	pAppearance->set_uid(unit->GetID());
	role->Send(oDisappearNtf);
}

void LineHandler::ForceOneLine(std::vector<Role*>& roles, SceneLine* sceneline)
{
	if (NULL == sceneline)
	{
		return;
	}
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		SceneLine* lastline = GetUnitLine(role->GetID());
		if (NULL == lastline || sceneline == lastline)
		{
			continue;
		}
		else
		{
			// leave last line
			RemoveFromLine(role, lastline);
			// notify self disappear
			for (auto i = lastline->m_role2viewset.begin(); i != lastline->m_role2viewset.end(); ++i)
			{
				Disappear(role, (Unit*)(i->second->m_role));
			}
			for (auto j = lastline->m_units.begin(); j != lastline->m_units.end(); ++j)
			{
				Disappear(role, j->second);
			}

			// enter new line
			AddToLine(role, sceneline);
		}
	}
}

void LineHandler::ForceOneLine(std::vector<Role*>& roles)
{
	if (!IsCommonSync())
	{
		SSWarn<<"view grid scene change line, sceneid:"<<m_scene->GetSceneTemplateID()<<END;
		return;
	}
	SceneLine* line = GetAvaliableLine(roles.size());
	if (NULL != line)
	{
		ForceOneLine(roles, line);
	}
}

void LineHandler::ChangeLine(std::vector<Role*>& roles, UINT32 line)
{
	if (!IsCommonSync())
	{
		SSWarn<<"view grid scene change line, sceneid:"<<m_scene->GetSceneTemplateID()<<END;
		return;
	}
	SceneLine* sceneline = GetLine(line);
	if (NULL != sceneline)
	{
		ForceOneLine(roles, sceneline);
	}
}

void LineHandler::RemoveFromLine(Role* role, SceneLine* line)
{
	PtcG2C_UnitDisappear oDisappearNtf;
	KKSG::UnitAppearance *pAppearance = &oDisappearNtf.m_Data;
	pAppearance->set_uid(role->GetID());
	line->Broadcast(role, oDisappearNtf, role);

	line->RemoveRole(role);
	m_Unit2Line.erase(role->GetID());

	if (FIRST_LINE == line->m_ID)
	{
		BroadcastWatcher(oDisappearNtf);
	}
}

void LineHandler::AddToLine(Role* role, SceneLine* line)
{
	AddUnitLine(role, line);
	line->AddRole(role);

	///> 假如是视野同步，进入场景时先不通知其他玩家，下帧会选择你可以看见的人
	///> 能否被人看见自己无法决定，只能被动被选择，每个人根据策略获取能看见的人
	switch (m_syncmode)
	{
	case SyncMode_Common:
		{
			RoleAppearNotifySender oSender;
			oSender.Start(role, true);
			oSender.SetNotifyMapNew(line->m_role2viewset);
			oSender.SetNotifyMap(line->m_units);
			oSender.SetNotifyMap(m_NonRolesInScene);
			oSender.Stop();

			if (FIRST_LINE == line->m_ID)
			{
				BroadcastWatcher(oSender.oAppearToOthersPtc);
			}
			break;
		}
	case SyncMode_View:
		{
			RoleAppearNotifySender oSender;
			oSender.Start(role, true);
			oSender.SetNotifyMap(m_NonRolesInScene);
			oSender.Stop();
			break;
		}
	case SyncMode_Group:
		{
			break;
		}
	default:
		break;
	}
}

void LineHandler::GetRolesCanSee(Role* role, std::vector<Role*>& roles)
{
	SceneLine* line = NULL;
	if (role->IsWatcher())
	{
		line = m_Lines.empty() ? NULL : m_Lines[0];		
	}
	else
	{
		line = GetUnitLine(role->GetID());
	}
	if (NULL != line)
	{
		line->GetCanSee(role, roles);
	}
}

void LineHandler::GetRolesCanBeSee(Role* role, std::vector<Role*>& roles, int limit)
{
	SceneLine* line = GetUnitLine(role->GetID());
	if (NULL != line)
	{
		line->GetCanBeSee(role, roles, limit);
	}
}

void LineHandler::GetUnitsCanSee(Role* role, std::vector<Unit*>& units)
{
	for (auto i = m_NonRolesInScene.begin(); i != m_NonRolesInScene.end(); ++i)
	{
		units.push_back(i->second);
	}
	SceneLine* line = NULL;
	if (role->IsWatcher())
	{
		line = m_Lines.empty() ? NULL : m_Lines[0];		
	}
	else
	{
		line = GetUnitLine(role->GetID());
	}
	if (NULL != line)
	{
		line->GetUnitsCanSee(role, units);
	}
}

void LineHandler::UpdateViewGrid(Unit* unit)
{
	if (IsViewGrid())
	{
		if (unit->IsRole())
		{
			SceneLine* line = GetUnitLine(unit->GetID());
			if (NULL != line)
			{
				line->UpdateViewGrid((Role*)unit);
			}
		}
	}
}

Role* LineHandler::IsInScene(UINT64 roleid)
{
	SceneLine* line = GetUnitLine(roleid);
	if (NULL == line)
	{
		return NULL;
	}
	else
	{
		return line->IsIn(roleid);
	}
}

void LineHandler::UpdateBindRole(Role* role, Role* other)
{
	if (m_syncmode != SyncMode_View)
	{
		return;
	}
	SceneLine* line = GetUnitLine(role->GetID());
	if (NULL != line )
	{
		((SceneViewLine*)line)->UpdateBindRoleView(role, other);
	}
}
