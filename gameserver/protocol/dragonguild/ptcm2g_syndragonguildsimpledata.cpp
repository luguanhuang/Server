#include "pch.h"
#include "dragonguild/ptcm2g_syndragonguildsimpledata.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "foreach.h"

// generate by ProtoGen at date: 2017/9/6 15:14:43

void PtcM2G_SynDragonGuildSimpleData::Process(UINT32 dwConnID)
{
	foreach (i in m_Data.data())
	{
		if (i->has_isdel() && i->isdel())
		{
			DragonGuildSimpleMgr::Instance()->DeleteDragonGuildSimple(i->dragonguildid());
			continue;
		}

		CDragonGuildSimple& dragonguildSimple = DragonGuildSimpleMgr::Instance()->AddDragonGuildSimple(i->dragonguildid());
		dragonguildSimple.SetDragonGuildId(i->dragonguildid());

		if (i->has_dragonguildlv())
		{
			dragonguildSimple.SetDragonGuildLvl(i->dragonguildlv());
		}

		if (i->has_dragonguildname())
		{
			dragonguildSimple.SetDragonGuildName(i->dragonguildname());
		}

		for (UINT32 cnt = 0; cnt < i->addmembers_size(); ++ cnt)
		{
			dragonguildSimple.AddDragonGuildMember(i->addmembers(cnt).roleid(), i->addmembers(cnt).position());
		}

		for (UINT32 cnt = 0; cnt < i->delmembers_size(); ++ cnt)
		{
			dragonguildSimple.DeleteMember(i->delmembers(cnt));
		}
	}
}
