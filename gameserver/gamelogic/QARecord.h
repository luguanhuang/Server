#ifndef __QA_RECORD_H__
#define __QA_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include <map>
#include "unit/role.h"
#include "define/qadef.h"

class QARecord : public RoleSerializeInterface
{
public:
	QARecord(Role* role);
	virtual ~QARecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	void AddQACount( UINT32 qaType );
	UINT32 GetQACount(UINT32 type);
	void SetTrigger(UINT32 type);
	UINT32 GetTriggerTime(UINT32 type);

	bool IsPlayingQA();
	UINT32 GetCurType();
	void SetCurQA(UINT32 type, UINT32 endTime);

	void OnDayPass(bool force = false);

public:
	Role* m_pRole;
	Switch m_ismodify;

	UINT32 m_curQAType;
	UINT32 m_lastEndTime;
	UINT32 m_lastResetTime;
	std::map<UINT32, UINT32> m_triggerTime;
	std::map<UINT32, UINT32> m_usedCount;
};

#endif // __LOTTERYRECORD_H__