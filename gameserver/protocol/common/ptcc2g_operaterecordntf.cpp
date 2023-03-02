#include "pch.h"
#include "common/ptcc2g_operaterecordntf.h"
#include "utility/loghelper.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "util.h"
#include "utility/tlogger.h"
#include "utility/tlogmgr.h"
#include "gamelogic/payv2Record.h"
#include "config.h"
#include "appid.h"

// generate by ProtoGen at date: 2015/9/21 10:27:50

void PtcC2G_OperateRecordNtf::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		TUIGuideFlowLog oLog(pRole);
		if (m_Data.has_arg())
		{
			oLog.arg = m_Data.arg();
		}
		oLog.m_nPosition = m_Data.position();
		oLog.Do();
	}
}
