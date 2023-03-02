#include "pch.h"
#include "rank_test.h"
#include "ranklist/ranklistmgr.h"
#include "role/rolesummarymgr.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/rpcc2m_startplant.h"
void CRankTestCase::Run(const CArgList& roArgs)
{
	std::unordered_map<UINT64, CRoleSummary*>::iterator it = CRoleSummaryMgr::Instance()->Begin();
	std::unordered_map<UINT64, CRoleSummary*>::iterator itEnd = CRoleSummaryMgr::Instance()->End();
	for(; it != itEnd; ++it)
	{
		CRoleSummary* poSummary = it->second;
		CRankListMgr::Instance()->OnChangeFashionPower(poSummary->GetID(), 1000, poSummary->GetLevel() + 10, -1);
	}
}

void garden::Run(const CArgList& roArgs)
{
	auto it = CRoleManager::Instance()->Begin();
	auto it_end = CRoleManager::Instance()->End();
	for (; it != it_end; ++it)
	{
		/*auto garden = GardenManager::Instance()->GetGarden((*it)->GetID(),(*it)->GetName());
		garden->TryPlantSeed(0,509);*/
		/*RpcC2M_StartPlant* rpc = RpcC2M_StartPlant::CreateRpc();
		rpc->m_oArg.set_farmland_id(0);
		rpc->m_oArg.set_seed_id(509);	
		MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), rpc);*/

	}
}