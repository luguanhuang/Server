#ifndef _QA_ROOM_H_
#define _QA_ROOM_H_

#include "table/QuestionLibraryTable.h"
#include "table/QAConditionTable.h"
#include "table/QAPointRankRewardTable.h"
#include "table/MultiActivityList.h"
#include "table/QALevelRewardTable.h"
#include "protocol.h"
#include "pb/project.pb.h"

class CRole;
class QAMgr;
class QARoom
{
public:
	friend class QAMgr;
	QARoom(UINT32 id, QAConditionTable::RowData* data);
	~QARoom();

	UINT32 Update();

	// �������
	bool Enter(CRole* role);
	// �ύ��
	KKSG::ErrorCode Answer(UINT64 roleId, const KKSG::CommitAnswerNtf& req, KKSG::GiveAnserQARewardNtf& res);
	// ����(���˴���)
	void GiveUp();

	// ��ȡ��ǰ����״̬
	void GetQADataReq(CRole* role, KKSG::GetQADataRes &roRes);

	void GetReward(UINT64 roleId, int rank, int times,  KKSG::GiveAnserQARewardNtf &roRes);
	bool IsFull();
	void SetRoomOver(bool reward = true);
	void RemovePlayer();

	void DoPrepare(UINT32 now);
	void DoRunning(UINT32 now);

	UINT32 GetID() {return m_id;}
	UINT32 GetEndTime();

	void UpdateRank(UINT64 roleId);

	// Э�����
	void PushEnterRoomNtf(UINT64 roleId);
	void PushIdName(CRole* role);
	void PushRankNtf(CRole* role);
	void PushRankNtfToAll();
	void PushNextQuestion();
	void PushAnswerAck(UINT64 roleId, UINT64 audioUID, const std::string& answer, bool correct, 
				    	INT32 rank, INT32 times, UINT32 titleId, UINT32 audioTime);
	void NotifyGSOver();

	void BroadCast(const CProtocol &ptc);

	static UINT32 GetAwardTimes(const QuestionLibraryTable::RowData *pConfig);

	struct QARole
	{
		QARole():roleId(0), score(0), correctNum(0), updateTime(0) {}

		UINT64 roleId;
		UINT32 score;
		UINT32 correctNum;
		UINT32 updateTime;
		std::string name;
		std::vector<KKSG::ItemBrief> items;		// ����

		bool operator > (const QARole& ths) const
		{
			if (score > ths.score)
			{
				return true;
			}
			if (score == ths.score && updateTime < ths.updateTime)
			{
				return true;
			}
			return false;
		}

		void AddReward(UINT32 itemId, UINT32 itemCnt)
		{
			for (size_t i = 0; i < items.size(); ++i)
			{
				KKSG::ItemBrief& item = items[i];
				if (item.itemid() == itemId)
				{
					item.set_itemcount(item.itemcount() + itemCnt);
					return;
				}
			}
			KKSG::ItemBrief item;
			item.set_itemid(itemId);
			item.set_itemcount(itemCnt);
			items.push_back(item);
		}
	};

private:
	QARole* GetQARole(UINT64 roleId);

private:
	UINT32 m_id;
	UINT32 m_type;
	UINT32 m_status;
	UINT32 m_beginPrepareTime;
	UINT32 m_closeTime;					// ����ʱ��
	UINT32 m_totalRunningTime;			// ����ʱ��(������ʱ�������)
	UINT32 m_lastUpateTime;				// �ϴθ���ʱ��
	QAConditionTable::RowData* m_data;

	std::vector<UINT32> m_questions;					// �����б�
	INT32 m_curIndex;									// ��ǰ����
	UINT32 m_curBeginTime;								// ��ǰ���⿪ʼʱ��

	UINT64 m_guild;										// ����id

	std::map<UINT64, QARole*> m_roles;				// ��Ҵ����¼
	std::vector<QARole*> m_rank;					// �������

	std::map<UINT32, std::vector<UINT64> > m_correctList;	//key:qid����ID  value����ԵĽ�ɫID�б�;
};

#endif