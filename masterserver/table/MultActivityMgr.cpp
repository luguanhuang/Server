#include "pch.h"
#include "MultActivityMgr.h"
#include "foreach.h"
#include <time.h>
#include "../share/util/XCommon.h"
#include "../skycity/skycitymgr.h"
#include "../reswar/reswarmgr.h"
#include "notice/noticemgr.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "config.h"
#include "mayhem/mayhemmgr.h"
#include "battlefield/battlefieldmgr.h"


INSTANCE_SINGLETON(MultActivityMgr);


MultActivityMgr::MultActivityMgr()
{

}

MultActivityMgr::~MultActivityMgr()
{

}

bool MultActivityMgr::Init()
{
	if (!MultActivityMgrBase::Init())
	{
		return false;
	}
	m_nOpenTime = MSConfig::Instance()->GetGameServerOpenTimeStamp();
	return true;
}

void MultActivityMgr::Uninit()
{
	MultActivityMgrBase::Uninit();
}

void MultActivityMgr::OnActivityStateChange(UINT32 id, KKSG::MulActivityTimeState lastState, KKSG::MulActivityTimeState curState)
{
	if (KKSG::MULACTIVITY_RUNNING == curState)
	{
		switch(id)
		{
		case MULT_ACTIVITY_SKYCITY:
			{
				SSInfo << "Sky City Start" << END;
				SkyCityMgr::Instance()->SetState(true);
				break;
			}
		case MULT_ACTIVITY_GUILDRESWAR:
			{	
				ResWarMgr::Instance()->SetState(true);
				SSInfo << "reswar Start" << END;
				break;
			}
		case MULT_ACTIVITY_CARDMATCH:
			{
				NoticeMgr::Instance()->NoticeGuildCardBeginSoon();
				break;
			}
		case MULT_ACTIVITY_POKERTOUNAMENT:
			{
				NoticeMgr::Instance()->NoticePokerTournamentBeginSoon();
				break;			
			}
		case MULT_ACTIVITY_BIGMELEE:
			{
				MayhemMsMgr::Instance()->OnStart();
				break; 
			}
		case MULT_ACTIVITY_BATTLEFIELD:
			{
				SSInfo << "BattleFiled Start...." << END;
				BattleFieldMgr::Instance()->OnStart();
				break;
			}
		default:
			break;
		}
	}else if (KKSG::MULACTIVITY_RUNNING == lastState &&KKSG::MULACTIVITY_RUNNING != curState)
	{
		switch(id)
		{
		case MULT_ACTIVITY_BATTLEFIELD:
			{

				break;
			}
		default:
				break;

		}
	}
}

void MultActivityMgr::GetGuildTask(std::vector<MultiActivityList::RowData* >& vCfg)
{
	for (auto i = m_tableList.Table.begin(); i != m_tableList.Table.end(); i ++)
	{
		if ((*i) && (*i)->WxGuildTask && GetOpenTimeState((*i)->ID) != KKSG::MULACTIVITY_UNOPEN_TODAY)
		{
			vCfg.push_back(*i);
		}
	}
}

INT32 MultActivityMgr::GetWeekCount(UINT32 id)
{
	MultActivityConf* pConf = GetActivityConfig(id);
	if (pConf == NULL)
	{
		return 0;
	}
	if (pConf->openServerWeek && GameTime::GetThisWeekBeginTime() < (GameTime::GetWeekBeginTime(m_nOpenTime) + 7*pConf->openServerWeek*XCommon::GetOneDayTime()))
	{
		return 0;
	}
	for (UINT32 i = 0; i < pConf->openDays.size(); ++i)
	{
		if (pConf->openDays[i] == -1)
		{
			return 0;
		}
		if (pConf->openDays[i] == 0)
		{
			return 7;
		}
	}
	return pConf->openDays.size();
}
