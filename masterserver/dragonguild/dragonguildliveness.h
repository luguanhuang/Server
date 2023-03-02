#ifndef _DRAGONGUILD_LIVENESS_H_
#define _DRAGONGUILD_LIVENESS_H_

#include "dragonguildbase.h"

struct DragonGuildLivenessRecord
{
	DragonGuildLivenessRecord(): roleId(0), actId(0), value(0), time(0) {}
	UINT64 roleId;
	UINT32 actId;
	UINT32 value;
	UINT32 time;

	void FromKKSG(const KKSG::DragonGuildLivenessRecordData& data)
	{
		roleId = data.roleid();
		actId = data.actid();
		value = data.value();
		time = data.time();
	}

	void ToKKSG(KKSG::DragonGuildLivenessRecordData& data)
	{
		data.set_roleid(roleId);
		data.set_actid(actId);
		data.set_value(value);
		data.set_time(time);
	}
};


class DragonGuildLiveness : public DragonGuildBase
{
public:
	DragonGuildLiveness(DragonGuild* dragonguild);

	virtual bool LoadFromDB(const KKSG::DragonGuildSaveData& data);
	virtual void SaveToDB(KKSG::DragonGuildSaveData& data);
	virtual void OnDayPass();

	UINT32 GetLiveness() { return m_liveness; }

	void AddPartnerLiveness(UINT64 roleId, UINT32 actId, UINT32 value);

	void FillLivenessRecord(KKSG::GetPartnerLivenessRes& res);

	UINT64 _FindNextLeader();
	void _CheckLeaderActivity();

	void DoRoleLeave(UINT64 roleId);
	UINT32 GetRoleActivity(UINT64 roleId);

private:
	static const int DAY_SECONDS = 24 * 60 * 60;

private:
	UINT32 m_liveness;					// ªÓ‘æ∂»
	std::map<UINT64, UINT32> m_roleActivityMap;
	std::deque<DragonGuildLivenessRecord> m_livenessItem;
};

#endif
