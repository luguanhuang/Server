#include "pch.h"
#include "securitymobstatistics.h"
#include "gamelogic/XCombat.h"
#include "securitystatistics.h"
#include "entity/XEntity.h"
#include "timeutil.h"
#include "unit/role.h"
#include "securitydamagestatistics.h"


void XSecurityMobStatistics::XMobInfo::Reset()
{
	_TemplateID = 0;
	_CastCount = 0;
	_AttackTotal = 0;
}

void XSecurityMobStatistics::XMobInfo::Merge(const XMobInfo& other)
{
	_AttackTotal += other._AttackTotal;

	_CastCount += other._CastCount;
}

XSecurityMobStatistics::XMobInfo* XSecurityMobStatistics::_TryGetMobInfo(UINT32 templateID)
{
	auto it = _MobInfos.find(templateID);
	if (it != _MobInfos.end())
		return it->second;

	XMobInfo* info = new XMobInfo();
	info->Reset();
	info->_TemplateID = templateID;
	_MobInfos[templateID] = info;
	
	return info;
}

void XSecurityMobStatistics::Reset()
{
	for(auto it = _MobInfos.begin(); it != _MobInfos.end(); ++it)
	{
		delete (XMobInfo*)(it->second);
	}
	_MobInfos.clear();
}

XSecurityMobStatistics::~XSecurityMobStatistics()
{
	Reset();
}

XSecurityMobStatistics* XSecurityMobStatistics::TryGetStatistics(Unit *pUnit)
{
	if (pUnit == NULL || pUnit->GetSecurityStatistics() == NULL)
		return NULL;

	return pUnit->GetSecurityStatistics()->_MobInfo;
}

void XSecurityMobStatistics::Merge(const XSecurityMobStatistics& other)
{
	for(auto it = other._MobInfos.begin(); it != other._MobInfos.end(); ++it)
	{
		XMobInfo* pMobInfo = _TryGetMobInfo(it->second->_TemplateID);
		pMobInfo->Merge(*it->second);
	}
}

void XSecurityMobStatistics::Append(Unit* pUnit)
{
	if (pUnit == NULL)
		return;
	XSecurityStatistics* pSS = pUnit->GetSecurityStatistics();
	if(pSS == NULL || !pSS->IsValid())
	{
		return;
	}

	XMobInfo* pMobInfo = _TryGetMobInfo(pUnit->GetTemplateID());
	++(pMobInfo->_CastCount);
	pMobInfo->_AttackTotal += pSS->_DamageInfo->_AttackTotal;
}
