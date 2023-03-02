#include "pch.h"
#include "pvpbasedata.h"
#include "teammgr.h"
#include "team.h"
#include "teammember.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "util/XRandom.h"
#include "pvpmgr.h"
#include "unit/dummyrolemanager.h"
#include "pvprecord.h"

void PvpBattleData::TestPrint()
{
	SSDebug << "Pvp Battle Data : " << END;
	SSDebug << "roleid = " << roleid << END;
	SSDebug << "role state = " << rolestate << END;
	SSDebug << "role camp = " << camp << END;
	SSDebug << "is leader = " << (int)(isleader) << END;
	SSDebug << "last die time = " << lastDieTime << END;
}
