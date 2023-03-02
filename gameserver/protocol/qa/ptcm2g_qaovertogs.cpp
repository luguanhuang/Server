#include "pch.h"
#include "qa/ptcm2g_qaovertogs.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/QARecord.h"
#include "define/qadef.h"

// generate by ProtoGen at date: 2016/10/11 11:03:33

void PtcM2G_QAOverToGS::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindByRoleID(m_Data.role_id());
	if (!role)
	{
		SSWarn << "no role:" << m_Data.role_id() << END;
		return;
	}

	QARecord* record = role->Get<QARecord>();
	record->SetCurQA(0, 0);
	SSDebug << "role QA end" << END;
}
