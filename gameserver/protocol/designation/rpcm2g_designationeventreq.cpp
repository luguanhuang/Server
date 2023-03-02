#include "pch.h"
#include "designation/rpcm2g_designationeventreq.h"
#include "gamelogic/designationMgr.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/ibshoprecord.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/taskrecord.h"
#include "util/gametime.h"
// generate by ProtoGen at date: 2016/9/21 15:58:52

RPC_SERVER_IMPLEMETION(RpcM2G_DesignationEventReq, DesignationEventReq, DesignationEventRes)

void RpcM2G_DesignationEventReq::OnCall(const DesignationEventReq &roArg, DesignationEventRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole)
	{
		for (int i =  0; i < roArg.event_size(); i++)
		{
			const KKSG::DesignationEvent *pEvent = &roArg.event(i);	
			if (pEvent->neventid() < ACH_EVENT_OFFSET)
			{
				DesignationMgr::Instance()->DoEvent(pRole,pEvent);
			}
			else if (pEvent->neventid() >= ACH_EVENT_OFFSET&&pEvent->neventid() < BONUS_EVENT_OFFSET)
			{
				AchieveV2Mgr::Instance()->DoEvent(pRole,pEvent);
			}else if (pEvent->neventid() >= BONUS_EVENT_OFFSET&&pEvent->neventid() < MENTOR_EVENT_OFFSET){}
			else if (pEvent->neventid() >= MIX_EVENT_OFFSET && pEvent->neventid() < TASK_EVENT_OFFSET)
			{
				UINT32 nMix = pEvent->neventid() - MIX_EVENT_OFFSET;
				if (nMix == EVENT_MIDAS_PAY_DEGREE)
				{
					IBShopRecord* pRecord = pRole->Get<IBShopRecord>();
					pRecord->AddPayDegree(pEvent->nparam1());

				}else if(nMix == EVENT_MIDAS_PAY_COUNT)
				{
					time_t dayBegin = XCommon::GetDayBeginTime(GameTime::GetTime());
					time_t dayEvent = XCommon::GetDayBeginTime(pEvent->ntime());
					if (dayBegin == dayEvent)
					{
						RoleMisc* record = pRole->Get<RoleMisc>();
						UINT32 nCount = record->GetDayGiftItem()+1;
						record->SetDayGiftItem(nCount);
					}

				}

			}
			else if (pEvent->neventid() >= TASK_EVENT_OFFSET)
			{
				KKSG::TaskOper oper;
				UINT32 nEventId = pEvent->neventid() - TASK_EVENT_OFFSET;	
				switch (nEventId)
				{
				case TASK_EVENT_SET: 
					pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Set, pEvent->nparam1(), pEvent->nparam2(),  pEvent->nparam3());
					break;
				case TASK_EVENT_ADD:
					pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, pEvent->nparam1(), pEvent->nparam2(),  pEvent->nparam3());
					break;
				case TASK_EVENT_DEL:
					pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Del, pEvent->nparam1(), pEvent->nparam2(),  pEvent->nparam3());
					break;
				case TASK_EVENT_ADD_TASKID_STEP:
					{
						WeeklyTaskRecord& wRecord = pRole->Get<TaskRecord>()->GetWeeklyRecord();
						WeeklyTaskOne* pTask = wRecord.GetSubTask<WeeklyTaskOne>(pEvent->nparam1());
						if (pTask) 
						{
							pTask->AddTaskStep(pEvent->nparam2());
						}
						break;
					}
				case TASK_EVENT_HELP_RECORD:
					{
						WeeklyTaskRecord& wRecord = pRole->Get<TaskRecord>()->GetWeeklyRecord();
						KKSG::TaskHelpInfo info;
						info.set_roleid(pEvent->lparam1());
						info.set_itemid(pEvent->nparam1());
						info.set_itemcount(pEvent->nparam2());
						info.set_time(pEvent->nparam3());
						info.set_rolename(pEvent->name());
						wRecord.AddHelperInfo(info);
						break;
					}
				case TASK_EVENT_DAILY_TASK_REFRESH:
					{
						DailyTaskRecord& dRecord = pRole->Get<TaskRecord>()->GetDailyRecord();
						dRecord.RefreshWithCheck(pEvent->nparam1());
						break;
					}
				case TASK_EVENT_CHARGE_BACK:
					{
						pRole->Get<RoleMisc>()->StartChargeBack(pEvent->nparam1());
						break;
					}
				}
			}
		}	
		roRes.set_errorcode(ERR_SUCCESS);
	}
	else
		roRes.set_errorcode(ERR_FAILED);
}

void RpcM2G_DesignationEventReq::OnDelayReplyRpc(const DesignationEventReq &roArg, DesignationEventRes &roRes, const CUserData &roUserData)
{
}
