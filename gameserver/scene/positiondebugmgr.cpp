#include "pch.h"
#include "positiondebugmgr.h"
#include "unit/unit.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/ptcg2c_positiondebug.h"
#include "entity/XRole.h"
#include "unit/enemy.h"
#include "unit/enemymanager.h"
#include "unit/dummyrolemanager.h"
#include "unit/dummyrole.h"

#define TRACE_POSITION

INSTANCE_SINGLETON(PositionDebugMgr)

PositionDebugMgr::PositionDebugMgr()
{
	m_handler = INVALID_HTIMER;
}

PositionDebugMgr::~PositionDebugMgr()
{

}

bool PositionDebugMgr::Init()
{
#ifdef TRACE_POSITION
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 32, -1, __FILE__, __LINE__);
#endif
	return true;
}

void PositionDebugMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void PositionDebugMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	for (auto i = m_Traces.begin(); i != m_Traces.end(); )
	{
		Role *pRole = RoleManager::Instance()->FindByRoleID(i->qwRole);
		if (pRole == NULL)
		{
			m_Traces.erase(i++);
		}
		else
		{
			i->DoTrace(pRole);
			++i;
		}
	}
}

void PositionDebugMgr::TraceUnit(UINT64 qwRole, UINT64 qwUnit)
{
	Traceinfo *info = NULL;
	auto i = FindTraceInfo(qwRole);
	if (i == m_Traces.end())
	{
		m_Traces.push_back(Traceinfo());
		info = &m_Traces.back();
		info->qwRole = qwRole;
	}
	else
	{
		info = &*i;
	}
	
	switch(Unit::GetUnitType(qwUnit))
	{
	case KKSG::Category_Role:
		info->m_traceRole.insert(qwUnit);
		break;
	case KKSG::Category_Enemy:
		info->m_traceEnemy.insert(qwUnit);
		break;
	case KKSG::Category_DummyRole:
		info->m_traceDummyRole.insert(qwUnit);
		break;
	default:
		break;
	}
}

void PositionDebugMgr::TraceRemoveUnit(UINT64 qwRole, UINT64 qwUnit)
{
	auto i = FindTraceInfo(qwRole);
	if (i != m_Traces.end())
	{
		Traceinfo *info = &*i;
		info->m_traceRole.erase(qwUnit);
		info->m_traceEnemy.erase(qwUnit);
		info->m_traceDummyRole.erase(qwUnit);
	}
}

void PositionDebugMgr::TraceClear(UINT64 qwRole)
{
	auto i = FindTraceInfo(qwRole);
	if (i != m_Traces.end())
	{
		Traceinfo *info = &*i;
		info->m_traceRole.clear();
		info->m_traceEnemy.clear();
		info->m_traceDummyRole.clear();
	}
}

std::list<Traceinfo>::iterator PositionDebugMgr::FindTraceInfo(UINT64 qwRole)
{
	for (auto i = m_Traces.begin(); i != m_Traces.end(); ++i)
	{
		if (i->qwRole == qwRole)
		{
			return i;
		}
	}

	return m_Traces.end();
}

std::string PositionDebugMgr::TraceList(UINT64 qwRole)
{
	auto i = FindTraceInfo(qwRole);
	if (i != m_Traces.end())
	{
		std::stringstream ss;
		ss << "roleid: " << qwRole << "\n";

		if (!i->m_traceEnemy.empty())
		{
			ss << "trace enemy:\n";
			for (auto j = i->m_traceEnemy.begin(); j != i->m_traceEnemy.end(); ++j)
			{
				ss << *j << "\n";
			}
		}
		
		if (!i->m_traceRole.empty())
		{
			ss << "trace role:\n";
			for (auto j = i->m_traceRole.begin(); j != i->m_traceRole.end(); ++j)
			{
				ss << *j << "\n";
			}
		}

		if (!i->m_traceDummyRole.empty())
		{
			ss << "trace dummyrole:\n";
			for (auto j = i->m_traceDummyRole.begin(); j != i->m_traceDummyRole.end(); ++j)
			{
				ss << *j << "\n";
			}
		}

		return ss.str();
	}
	else
	{
		return "not found trace record";
	}
}

void Traceinfo::DoTrace(Role *pTracer)
{
	int Count = 0;
	PtcG2C_PositionDebug oPtc;

	for (auto i = m_traceRole.begin(); i != m_traceRole.end(); ++i)
	{
		Role *pRole = RoleManager::Instance()->FindByRoleID(*i);
		if (pRole && pRole->GetCurrScene() == pTracer->GetCurrScene())
		{
			++Count;
			auto *plist = oPtc.m_Data.add_positions();
			plist->set_uid(pRole->GetID());
			plist->set_face(pRole->GetXRole()->GetFaceDegree());
			plist->mutable_position()->set_x(pRole->GetXRole()->GetPosition_p().x);
			plist->mutable_position()->set_y(pRole->GetXRole()->GetPosition_p().y);
			plist->mutable_position()->set_z(pRole->GetXRole()->GetPosition_p().z);
		}
	}
	
	for (auto i = m_traceEnemy.begin(); i != m_traceEnemy.end(); ++i)
	{
		Enemy *pEnemy = EnemyManager::Instance()->FindEnemy(*i);
		if (pEnemy && pEnemy->GetCurrScene() == pTracer->GetCurrScene())
		{
			++Count;
			auto *plist = oPtc.m_Data.add_positions();
			plist->set_uid(pEnemy->GetID());
			plist->set_face(pEnemy->GetXObject()->GetFaceDegree());
			plist->mutable_position()->set_x(pEnemy->GetXObject()->GetPosition_p().x);
			plist->mutable_position()->set_y(pEnemy->GetXObject()->GetPosition_p().y);
			plist->mutable_position()->set_z(pEnemy->GetXObject()->GetPosition_p().z);
		}
	}

	for (auto i = m_traceDummyRole.begin(); i != m_traceDummyRole.end(); ++i)
	{
		DummyRole *pDummyRole = DummyRoleManager::Instance()->Find(*i);
		if (pDummyRole && pDummyRole->GetCurrScene() == pTracer->GetCurrScene())
		{
			++Count;
			auto *plist = oPtc.m_Data.add_positions();
			plist->set_uid(pDummyRole->GetID());
			plist->set_face(pDummyRole->GetXObject()->GetFaceDegree());
			plist->mutable_position()->set_x(pDummyRole->GetXObject()->GetPosition_p().x);
			plist->mutable_position()->set_y(pDummyRole->GetXObject()->GetPosition_p().y);
			plist->mutable_position()->set_z(pDummyRole->GetXObject()->GetPosition_p().z);
		}
	}

	if (Count > 0)
	{
		pTracer->Send(oPtc);
	}
}
