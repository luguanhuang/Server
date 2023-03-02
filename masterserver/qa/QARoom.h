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

	// 加入答题
	bool Enter(CRole* role);
	// 提交答案
	KKSG::ErrorCode Answer(UINT64 roleId, const KKSG::CommitAnswerNtf& req, KKSG::GiveAnserQARewardNtf& res);
	// 放弃(个人答题)
	void GiveUp();

	// 拉取当前答题状态
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

	// 协议相关
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
		std::vector<KKSG::ItemBrief> items;		// 奖励

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
	UINT32 m_closeTime;					// 结束时间
	UINT32 m_totalRunningTime;			// 答题时间(所有题时间加起来)
	UINT32 m_lastUpateTime;				// 上次更新时间
	QAConditionTable::RowData* m_data;

	std::vector<UINT32> m_questions;					// 问题列表
	INT32 m_curIndex;									// 当前问题
	UINT32 m_curBeginTime;								// 当前问题开始时间

	UINT64 m_guild;										// 公会id

	std::map<UINT64, QARole*> m_roles;				// 玩家答题记录
	std::vector<QARole*> m_rank;					// 玩家排行

	std::map<UINT32, std::vector<UINT64> > m_correctList;	//key:qid问题ID  value：答对的角色ID列表;
};

#endif