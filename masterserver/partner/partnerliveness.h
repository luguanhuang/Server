#ifndef _PARTNER_LIVENESS_H_
#define _PARTNER_LIVENESS_H_

#include "partnerbase.h"

struct PartnerLivenessRecord
{
	PartnerLivenessRecord(): roleId(0), actId(0), value(0), time(0) {}
	UINT64 roleId;
	UINT32 actId;
	UINT32 value;
	UINT32 time;

	void FromKKSG(const KKSG::PartnerLivenessRecordData& data)
	{
		roleId = data.roleid();
		actId = data.actid();
		value = data.value();
		time = data.time();
	}

	void ToKKSG(KKSG::PartnerLivenessRecordData& data)
	{
		data.set_roleid(roleId);
		data.set_actid(actId);
		data.set_value(value);
		data.set_time(time);
	}
};

class PartnerLiveness : public PartnerBase
{
public:
	PartnerLiveness(Partner* partner);

	virtual bool LoadFromDB(const KKSG::PartnerSaveData& data);
	virtual void SaveToDB(KKSG::PartnerSaveData& data);
	virtual void OnDayPass();

	UINT32 GetLiveness() { return m_liveness; }

	void AddPartnerLiveness(UINT64 roleId, UINT32 actId, UINT32 value);

	void FillLivenessRecord(KKSG::GetPartnerLivenessRes& res);

private:
	UINT32 m_liveness;					// ªÓ‘æ∂»
	std::deque<PartnerLivenessRecord> m_livenessItem;
};

#endif
