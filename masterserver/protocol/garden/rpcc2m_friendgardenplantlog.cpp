#include "pch.h"
#include "garden/rpcc2m_friendgardenplantlog.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "role/rolesummarymgr.h"
// generate by ProtoGen at date: 2016/10/9 11:03:11


RPC_SERVER_IMPLEMETION(RpcC2M_FriendGardenPlantLog, FriendGardenPlantLogArg, FriendGardenPlantLogRes)

	void RpcC2M_FriendGardenPlantLog::OnCall(const FriendGardenPlantLogArg &roArg, FriendGardenPlantLogRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	FriendOp op(role->GetID());
	const std::map<UINT64, FriendInfo>* friends = op.GetFriendList();

	auto it = friends->begin();
	auto it_end = friends->end();
	for (; it != it_end; ++it )
	{
		KKSG::FriendPlantLog*  plant_log = roRes.add_frinend_plant_log();
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(it->first);
		if (NULL == summary)
		{
			//roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			SSWarn<<"summary is NULL, role  id:"<<it->first<<END;
			continue;
		}
		plant_log->set_role_id(summary->GetID());
		plant_log->set_role_name(summary->GetName());	
		plant_log->set_profession_id(summary->GetProfession());

		Garden* garden = GardenManager::Instance()->GetGarden(summary->GetID(),summary->GetName());
		if (NULL == garden)
		{
			continue;
		}
		const PlantInfoStore& all_record = garden->GetPlantInfoStore();
		auto rcit = all_record.plantinfo_record.begin();
		auto rcit_end = all_record.plantinfo_record.end();
		for (; rcit != rcit_end; ++rcit)
		{
			if (rcit->second->plant_grow_state == KKSG::growMature)
			{
				auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(rcit->second->seed_id);
				if (rowdata == NULL)
				{
					roRes.set_result(KKSG::ERR_GARDEN_NOTEXIST_SEEDID);
					return ;
				}
				// 还有偷的次数，且自己没有偷过				
				if (rcit->second->stealed_times < rowdata->CanStealMaxTimes)
				{
					bool steal = false;
					auto logit = rcit->second->record_event_log.begin();
					auto logit_end = rcit->second->record_event_log.end();
					for (; logit != logit_end; ++logit)
					{
						if (logit->event_type() == friendSteal)
						{
							if ( role->GetID() == logit->role_id() )
							{
								steal = true;
								break;
							}
						}
					}

					if (!steal)
					{
						plant_log->set_mature(true);
					}
				}
			}
			else if (rcit->second->plant_grow_state != KKSG::growCD)
			{
				plant_log->set_abnormal_state(true);
			}		
		}

		if (all_record.sprite_id>0 && all_record.sprite_durations >0 )
		{
			plant_log->set_exist_sprite(true);
		}
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_FriendGardenPlantLog::OnDelayReplyRpc(const FriendGardenPlantLogArg &roArg, FriendGardenPlantLogRes &roRes, const CUserData &roUserData)
{
}
