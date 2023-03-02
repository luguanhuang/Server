#include "pch.h"
#include "aionlinedebugmgr.h"
#include "scene/scene.h"

#ifdef WIN32
#include <io.h>
#else
#include <dirent.h>
#endif
#include "singleton.h"
#include "battle/ptcg2c_aidebuginfo.h"
#include "ainode.h"

INSTANCE_SINGLETON(AIOnlineDebugMgr)

AIOnlineDebugMgr::AIOnlineDebugMgr()
{
}

AIOnlineDebugMgr::~AIOnlineDebugMgr()
{

}

void AIOnlineDebugMgr::AddDebugUnit(Unit* unit)
{
	if (!unit->GetCurrScene())
		return;

	if (mMsgUnit.empty() || mMsgUnit.find(unit->GetCurrScene()->GetSceneID()) == mMsgUnit.end())
	{
		std::unordered_map<UINT64, Unit*> debugUnits;
		debugUnits[unit->GetID()] = unit;
		mMsgUnit[unit->GetCurrScene()->GetSceneID()] = debugUnits;
	}
	else
	{
		std::unordered_map<UINT64, Unit*> debugUnits = mMsgUnit[unit->GetCurrScene()->GetSceneID()];

		if (debugUnits.find(unit->GetID()) == debugUnits.end())
			debugUnits[unit->GetID()] = unit;
	}
}

void AIOnlineDebugMgr::RemoveDebugUnit(Unit* unit)
{
	if (!unit->GetCurrScene())
		return;
	
	if (mMsgUnit.empty())
		return;

	if (mMsgUnit.find(unit->GetCurrScene()->GetSceneID()) == mMsgUnit.end())
		return;

	auto it = mMsgUnit[unit->GetCurrScene()->GetSceneID()].find(unit->GetID());
	if (it != mMsgUnit[unit->GetCurrScene()->GetSceneID()].end())
	{
		mMsgUnit[unit->GetCurrScene()->GetSceneID()].erase(it);

		if (mMsgUnit[unit->GetCurrScene()->GetSceneID()].empty())
		{
			auto unitit = mMsgUnit.find(unit->GetCurrScene()->GetSceneID());

			if (unitit != mMsgUnit.end())
				mMsgUnit.erase(unitit);
		}
	}
}

void AIOnlineDebugMgr::SendDebugMsg(UINT32 sceneid, string msg)
{
	if (mMsgUnit.find(sceneid) == mMsgUnit.end())
		return;

	std::unordered_map<UINT64, Unit*>& units = mMsgUnit[sceneid];

	if (units.empty())
	{
		mMsgUnit.erase(mMsgUnit.find(sceneid));
		return;
	}

	auto it = units.begin();

	PtcG2C_AIDebugInfo debuginfo;
	debuginfo.m_Data.set_level(1);
	debuginfo.m_Data.set_msg(msg);

	while(it != units.end())
	{
		Unit* unit = Unit::FindUnit(it->first);
		
		if (unit)
		{
			unit->Send(debuginfo);
			it ++;
		}
		else
		{
			it = units.erase(it);
		}

	}
}
void AIOnlineDebugMgr::SendMsg(Unit* pUnit)
{
	if (pUnit == NULL || pUnit->GetCurrScene() == NULL)
		return;

	auto it = mWatchUnits.find(pUnit->GetID());
	
	if (it == mWatchUnits.end())
	{
		return;
	}

	AIDebugInfo& debugInfo = it->second;
	if (debugInfo.infos.size() == 0)
		return;

	std::stringstream ss;
	ss << "------" << pUnit->GetID() << "---" << pUnit->GetTemplateID() << "------\n";
	for (auto it2 = debugInfo.infos.begin(); it2 != debugInfo.infos.end(); ++it2)
	{
		ss << *it2 << "\n";
	}

	debugInfo.infos.clear();

	if(debugInfo.observers.size() == 0 && mDebugAll.empty())
		return;

	PtcG2C_AIDebugInfo debuginfo;
	debuginfo.m_Data.set_level(1);
	debuginfo.m_Data.set_msg(ss.str());

	for (auto it2 = debugInfo.observers.begin(); it2 != debugInfo.observers.end();)
	{
		Unit* unit = Unit::FindUnit(*it2);

		if (unit && unit->GetCurrScene() && unit->GetCurrScene()->GetSceneID() == pUnit->GetCurrScene()->GetSceneID())
		{
			unit->Send(debuginfo);
			it2 ++;
		}
		else
		{
			it2 = debugInfo.observers.erase(it2);
		}
	}

	for (auto it3 = mDebugAll.begin(); it3 != mDebugAll.end();)
	{
		Unit* unit = Unit::FindUnit(*it3);

		if (unit)
		{
			if (unit->GetCurrScene() && unit->GetCurrScene()->GetSceneID() == pUnit->GetCurrScene()->GetSceneID())
				unit->Send(debuginfo);
			it3 ++;
		}
		else
		{
			it3 = mDebugAll.erase(it3);
		}
	}
}

void AIOnlineDebugMgr::AppendInfo(UINT64 uid, const string& info)
{
	if (IsDebugAll())
	{
		AddWatch(uid);
	}
	auto it = mWatchUnits.find(uid);
	if (it == mWatchUnits.end())
		return;

	it->second.AppendInfo(info);
}

bool AIOnlineDebugMgr::IsWatched(UINT64 uid)
{
	return mWatchUnits.find(uid) != mWatchUnits.end();
}

void AIOnlineDebugMgr::AddWatch(Unit* pTarget, Unit* pObserver)
{
	if (pTarget == NULL || pObserver == NULL)
		return;

	RemoveObserver(pObserver->GetID());

	mObservers[pObserver->GetID()] = pTarget->GetID();

	AIDebugInfo& info = mWatchUnits[pTarget->GetID()];
	info.AddObserver(pObserver->GetID());
	info.uid = pTarget->GetID();
}

void AIOnlineDebugMgr::AddWatch(Unit* pTarget)
{
	if (pTarget == NULL)
		return;

	AIDebugInfo& info = mWatchUnits[pTarget->GetID()];
	info.uid = pTarget->GetID();
}

void AIOnlineDebugMgr::AddWatch(UINT64 uid)
{
	AIDebugInfo& info = mWatchUnits[uid];
	info.uid = uid;
}

void AIOnlineDebugMgr::RemoveObserver(UINT64 uid)
{
	auto it = mObservers.find(uid);
	if (it != mObservers.end())
	{
		auto it2 = mWatchUnits.find(it->second);
		if (it2 != mWatchUnits.end())
		{
			if (it2->second.RemoveObserver(uid) == 0)
			{
				RemoveWatch(it2->first);
			}
		}

		mObservers.erase(it);
	}
}

void AIOnlineDebugMgr::RemoveWatch(UINT64 uid)
{
	mWatchUnits.erase(uid);
}

void AIOnlineDebugMgr::SetDebugAll(UINT64 uid, bool bDebugAll)
{
	if (bDebugAll)
		mDebugAll.insert(uid);
	else
		mDebugAll.erase(uid);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

AIDebugHelper::AIDebugHelper(AINodeBase* pAINode, Unit* pUnit)
{
	m_pAINode = pAINode;
	m_pUnit = pUnit;
#ifdef AIDEBUG
	if (m_pAINode)
	{
		m_pAINode->GetDebugInfo().clear();
		m_pAINode->GetDebugInfo().str("");
		m_pAINode->GetDebugInfo() << "{" << m_pAINode->GetFriendlyName() << "} ";
	}
	m_bShouldLog = m_pAINode && m_pUnit && m_pAINode->IsBreakPoint() && AIOnlineDebugMgr::Instance()->IsWatched(m_pUnit->GetID());
#else
	m_bShouldLog = false;
#endif
}

AIDebugHelper::~AIDebugHelper()
{
	Finish();
}

void AIDebugHelper::Finish()
{
#ifdef AIDEBUG
	if (!m_pAINode || !m_pUnit)
		return;

	if (!m_pAINode->IsBreakPoint())
		return;

	AIOnlineDebugMgr::Instance()->AppendInfo(m_pUnit->GetID(), m_pAINode->GetDebugInfo().str());
#endif
}

void AIDebugHelper::TryReport()
{
#ifdef AIDEBUG
	if (!m_pUnit)
		return;

	AIOnlineDebugMgr::Instance()->SendMsg(m_pUnit);
	///> 防止析构的时候调Finish函数
	m_pUnit = NULL;
#endif
}
