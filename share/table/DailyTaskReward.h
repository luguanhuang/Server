#ifndef __DailyTaskReward_H__
#define __DailyTaskReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/12 16:06:16


class DailyTaskReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	level;
		uint	tasktype;
		uint	taskquality;
		vector<Sequence<uint, 2>>	taskreward;
		uint	contribute;
		vector<Sequence<uint, 2>>	extrareward_s;
		uint	category;
		uint	score;
		vector<Sequence<uint, 2>>	reward_s;
		vector<Sequence<uint, 2>>	reward_a;
		vector<Sequence<uint, 2>>	reward_b;
		vector<Sequence<uint, 2>>	reward_c;
		vector<Sequence<uint, 2>>	reward_d;
		vector<Sequence<uint, 2>>	extrareward_a;
		vector<Sequence<uint, 2>>	extrareward_b;
		vector<Sequence<uint, 2>>	extrareward_c;
		vector<Sequence<uint, 2>>	extrareward_d;

		void CopyFrom(RowData *pRowData)
		{
			this->level=pRowData->level;
			this->tasktype=pRowData->tasktype;
			this->taskquality=pRowData->taskquality;
			this->taskreward=pRowData->taskreward;
			this->contribute=pRowData->contribute;
			this->extrareward_s=pRowData->extrareward_s;
			this->category=pRowData->category;
			this->score=pRowData->score;
			this->reward_s=pRowData->reward_s;
			this->reward_a=pRowData->reward_a;
			this->reward_b=pRowData->reward_b;
			this->reward_c=pRowData->reward_c;
			this->reward_d=pRowData->reward_d;
			this->extrareward_a=pRowData->extrareward_a;
			this->extrareward_b=pRowData->extrareward_b;
			this->extrareward_c=pRowData->extrareward_c;
			this->extrareward_d=pRowData->extrareward_d;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "level", "tasktype", "taskquality", "taskreward", "contribute", "extrareward_s", "category", "score", "reward_s", "reward_a", "reward_b", "reward_c", "reward_d", "extrareward_a", "extrareward_b", "extrareward_c", "extrareward_d",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DailyTaskReward  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.tasktype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.taskquality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.taskreward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.contribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.extrareward_s)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.category)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.score)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.reward_s)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.reward_a)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.reward_b)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.reward_c)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.reward_d)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.extrareward_a)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.extrareward_b)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.extrareward_c)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.extrareward_d)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DailyTaskReward ()
	{
		Clear();
	}

	void Clear()
	{
		for (UINT32 i = 0; i < Table.size(); ++i)
		{
			delete Table[i];
		}

		Table.clear();


	}

	std::vector<RowData*> Table;
};

#endif