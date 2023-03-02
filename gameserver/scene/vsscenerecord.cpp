#include "pch.h"
#include "vsscenerecord.h"
#include "unit/unit.h"
#include "unit/role.h"
#include "unit/dummyrole.h"

VkOneRec::VkOneRec(const VsUnit& kill, const VsUnit& dead)
	:m_kill(kill),m_dead(dead)
{
	m_ktime = time(NULL);
}

VsKillRecord::VsKillRecord()
{

}

void VsKillRecord::AddOneRecord(Unit* kill, Unit* dead)
{
	if(NULL == kill || NULL == dead)
	{
		return ;
	}
	VsUnit killunit;
	VsUnit deadunit;
	GetVsUnit(kill, killunit);
	GetVsUnit(dead, deadunit);
	VkOneRec rec(killunit, deadunit);
	m_killlist.push_back(rec);
}

void VsKillRecord::Clear()
{
	m_killlist.clear();
}

void VsKillRecord::GetVsUnit(Unit* pUnit, VsUnit& vsunit)
{
	if(NULL == pUnit)
	{
		return ;
	}
	vsunit.uid = pUnit->GetID();
	if(pUnit->IsRole())
	{
		vsunit.name = ((Role*)(pUnit))->GetName();
	}
	else if(pUnit->IsDummyRole())
	{
		vsunit.name = ((DummyRole*)(pUnit))->GetName();
	}
	vsunit.ppt = pUnit->GetAttr(TOTAL_POWERPOINT);
}

void GmfKickRecord::AddRole(UINT64 roleid)
{
	m_kickrec[roleid] = (UINT32)time(NULL);
}

UINT32 GmfKickRecord::GetCoolDownTime(UINT64 roleid)
{
	auto it = m_kickrec.find(roleid);
	if(it == m_kickrec.end())
	{
		return 0;
	}

	time_t now = time(NULL);
	if(it->second + GetGlobalConfig().GMFKickCoolDown > now)
	{
		return (it->second + GetGlobalConfig().GMFKickCoolDown - now);
	}
	return 0;
}
