#include "pch.h"
#include "teamsynextradata.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "dragonguild/dragonguildmgr.h"

HTeamSynExtraData::HTeamSynExtraData()
{

}

HTeamSynExtraData::~HTeamSynExtraData()
{

}

void HTeamSynExtraData::FillExtraData(CRole* pRole, KKSG::TeamSynExtraData& data)
{
	if(NULL == pRole)
	{
		return;
	}
	data.set_roleid(pRole->GetID());
	data.set_sceneid(pRole->GetMapID());
	data.set_guildid(CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID()));
	data.set_dragonguildid(DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID()));
}

