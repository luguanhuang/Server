#include "pch.h"
#include "partnerliveness.h"
#include "partner.h"
#include "util/gametime.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "tablemgr/partnertablemgr.h"
#include "partner/ptcm2g_updatepartnertogs.h"
#include "partner/ptcm2c_updatepartnertoclient.h"
#include "table/globalconfig.h"

PartnerLiveness::PartnerLiveness(Partner* partner)
:PartnerBase(partner)
,m_liveness(0)
{

}

bool PartnerLiveness::LoadFromDB(const KKSG::PartnerSaveData& data)
{
	KKSG::PartnerLivenessData ln;
	if (!ln.ParseFromString(data.liveness()))
	{
		LogError("parse livenes failed");
		return false;
	}

	m_liveness = ln.liveness();

	m_livenessItem.clear();
	m_livenessItem.resize(ln.record_size());
	for (int i = 0; i < ln.record_size(); ++i)
	{
		m_livenessItem[i].FromKKSG(ln.record(i));
	}

	return true;
}

void PartnerLiveness::SaveToDB(KKSG::PartnerSaveData& data)
{
	KKSG::PartnerLivenessData ln;

	ln.set_liveness(m_liveness);

	ln.clear_record();
	for (auto it = m_livenessItem.begin(); it != m_livenessItem.end(); ++it)
	{
		KKSG::PartnerLivenessRecordData* record = ln.add_record();
		it->ToKKSG(*record);
	}

	ln.SerializeToString(data.mutable_liveness());
}

void PartnerLiveness::OnDayPass()
{
	m_liveness = 0;	
	m_livenessItem.clear();
	SetDirty();
}

void PartnerLiveness::AddPartnerLiveness(UINT64 roleId, UINT32 actId, UINT32 value)
{
	PartnerLivenessRecord item;
	item.roleId = roleId;
	item.actId = actId;
	item.value = value;
	item.time = GameTime::GetTime();
	
	UINT32 oldValue = m_liveness;
	m_liveness += value;
	m_livenessItem.push_front(item);

	if (m_livenessItem.size() > GetGlobalConfig().PartnerLivenessRecordNum)
	{
		m_livenessItem.pop_back();
	}

	// 有箱子可以领取的时候, 推送给GS活跃度，用于推送给客户端红点
	bool hasChest = PartnerTableMgr::Instance()->HasNewCanTakChest(m_owner->GetLevel(), oldValue, m_liveness);
	if (hasChest)
	{
		PtcM2G_UpdatePartnerToGS ntf;
		ntf.m_Data.set_type(KKSG::POT_Liveness);
		ntf.m_Data.set_liveness(m_liveness);
		m_owner->BroadCastToGS(ntf);
	}

	SetDirty();
}

void PartnerLiveness::FillLivenessRecord(KKSG::GetPartnerLivenessRes& res)
{
	UINT32 now = GameTime::GetTime();
	for (auto it = m_livenessItem.begin(); it != m_livenessItem.end(); ++it)
	{
		PartnerLivenessRecord& item = *it;
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(item.roleId);
		if (su == NULL)
		{
			continue;
		}
		KKSG::PartnerLivenessItem* record = res.add_record();
		record->set_roleid(item.roleId);
		record->set_name(su->GetName());
		record->set_actid(item.actId);
		record->set_value(item.value);
		UINT32 passTime = now > item.time ? (now - item.time) : 0;
		record->set_time(passTime);
	}
}
