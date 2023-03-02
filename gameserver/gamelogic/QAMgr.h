#ifndef _QA_MANAGER_H_
#define _QA_MANAGER_H_

#include "table/QuestionLibraryTable.h"
#include "table/QAConditionTable.h"
#include "table/QAPointRankRewardTable.h"
#include "table/MultiActivityList.h"
#include "table/QALevelRewardTable.h"
#include "item.h"
#include "protocol.h"
#include "util/XCommon.h"
#include "define/qadef.h"
#include "timer.h"

class Role;
class QAMgr : public ITimer
{
	QAMgr();
	~QAMgr();
	DECLARE_SINGLETON(QAMgr);

public:
	bool Init();
	void Uninit();
	void Update();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();


	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void OnRoleLogin(Role* role);
	KKSG::ErrorCode CanRoleEnter(Role* role, UINT32 type);

	void SetGuildRoomId(UINT64 guildId, UINT32 roomId);
	void RemoveGuildRoom(UINT64 guildId);
	bool HasGuildRoom(UINT64 guildId);
	UINT32 GetGuildLastOpenTime(UINT64 guildId);

	void NotifyGuildRoomOpen(UINT64 guildId);
	void NotifyGuildRoomClose(UINT64 guildId);
	void PushQANotify(Role* role, UINT32 type, bool playing = false);
	void PushQAOverNotifyToAllNotEnter();

	bool IsGuildQAOpen();
	void TriggerQA(Role* pRole, UINT32 qaType);

	void GMOpenQA(Role* role, UINT32 type);
	void GMResetRole(Role* role);

	// ������
	bool IsTimeOk(UINT32 type, UINT32 now);
	bool IsLevelOk(QAConditionTable::RowData* row, UINT32 level);
	UINT32 GetPrepareTime(UINT32 type);

	void SetMultPrepareEndTime(UINT32 t);

private:
	HTIMER m_timerId;
	QAConditionTable		m_QAConditionTable;

	UINT32	m_lastUpdateTime;

	// GS崻������������ݶ�ʧ
	std::map<UINT64, UINT32> m_guildRooms;			// ���᷿��
	std::map<UINT64, UINT32> m_guildLastOpenTime;	// �����ϴο���ʱ��
};

#endif