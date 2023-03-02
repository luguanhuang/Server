#include "pch.h"
#include "qa/ptcm2g_updateguildqaroom.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/bagtransition.h"
#include "define/qadef.h"
#include "gamelogic/QAMgr.h"

// generate by ProtoGen at date: 2016/10/9 14:44:18

void PtcM2G_UpdateGuildQARoom::Process(UINT32 dwConnID)
{
	if (m_Data.status() == RoomStatus_Prepare)
	{
		QAMgr::Instance()->SetGuildRoomId(m_Data.guild_id(), m_Data.room_id());
		QAMgr::Instance()->NotifyGuildRoomOpen(m_Data.guild_id());
	}
	else if (m_Data.status() == RoomStatus_Over || m_Data.status() == RoomStatus_Close)
	{
		QAMgr::Instance()->RemoveGuildRoom(m_Data.guild_id());
		QAMgr::Instance()->NotifyGuildRoomClose(m_Data.guild_id());
	}
}
