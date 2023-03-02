#ifndef _QA_MGR_H_
#define _QA_MGR_H_

#include "table/QuestionLibraryTable.h"
#include "table/QAConditionTable.h"
#include "table/QAPointRankRewardTable.h"
#include "table/MultiActivityList.h"
#include "table/QALevelRewardTable.h"
#include "protocol.h"
#include "util/XCommon.h"
#include "QARoom.h"
#include "timer.h"



class CRole;
class QAMgr : public ITimer
{
	QAMgr();
	~QAMgr();
	DECLARE_SINGLETON(QAMgr);

public:
	bool Init();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	void Uninit();
	void Update();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	// ��Ҳ���
	QARoom* EnterQARoom(CRole* role, UINT32 type, KKSG::ErrorCode& code);
	QARoom* CreateGuildQARoom(CRole* role, KKSG::ErrorCode& code);

	UINT32 GetCount(UINT32 nType);
	void GetGuildQAStatus(CRole* role, UINT32& status, UINT32& time, KKSG::ErrorCode& code);

	void RemoveRole(UINT64 roleId);

	void GMOpenQA(CRole* role, UINT32 type);
	void GMResetRole(CRole* role);

	// ��ѯ���
	bool IsRolePlaying(UINT64 roleId);
	QARoom* GetAvailableMultiRoom();
	QARoom* GetGuildRoom(CRole* role);
	QARoom* GetRoom(UINT32 roomId);
	UINT32 GetRoleRoomId(UINT64 roleId);
	QARoom* GetRoomByRoleId(UINT64 roleId);

	// ����
	void PushQANotify(CRole* role, UINT32 type);

	// ������
	void RandQuestion(UINT32 type, std::vector<UINT32>& questions, UINT32 maxNum);
	QuestionLibraryTable::RowData* GetQuestion(UINT32 id);
	UINT32 GetLevelSeal(UINT32 levelseal,UINT32 type);
	QAPointRankRewardTable::RowData* GetPointRankReward(UINT32 type, UINT32 rank,UINT32 levelseal);
	void GetExtraReward(UINT32 qaType, UINT32 level, UINT32 rank, UINT32& itemId, UINT32& itemCount);
	void GetStandardReward(UINT32 qaType, UINT32 level, std::vector<KKSG::ItemBrief>& itemList);
	static bool IsAnswerCorrect(std::vector<std::string>& config, const std::string& answer);
	UINT32 GetPrepareTime(UINT32 type);

	void GMOpenQA(UINT32 type);

private:
	QARoom* CreateRoom(UINT32 type);

private:
	UINT32 m_nWeek;
	QuestionLibraryTable	m_QuestionLibraryTable;
	QAConditionTable		m_QAConditionTable;
	QAPointRankRewardTable	m_QAPointRankRewardTable;
	QALevelRewardTable		m_levelRewardTable;
	std::map<UINT64, std::vector<UINT32> > m_mapQuestionLib;	//��Ŀ�б� key��qatype   value���ô��������µ�������Ŀ���;
	HTIMER m_timerId;

	UINT32	m_curRoomId;
	UINT32	m_lastUpdateTime;
	QARoom* m_curRoom;				// ��ǰ�ɽ����˷���

	std::unordered_map<UINT64, UINT32>	m_roleId2RoomId;
	std::map<UINT32, QARoom*>	m_rooms;			// ���еĴ��ⷿ��
	std::map<UINT64, QARoom*>	m_guildRooms;		// ������ⷿ��

///ͳ�ƴ����������
	FILE* m_pLogFile;
	UINT32 m_nTotalCount;
	UINT32 m_nLogTime;

};

#endif