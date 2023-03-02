#ifndef __CustomSystemRewardTable_H__
#define __CustomSystemRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/9 10:19:43


class CustomSystemRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	wincounts;
		vector<Sequence<uint, 2>>	rewardshow;
		uint	rewarddrop;
		uint	boxcooldown;
		Sequence<uint, 2>	boxquickopen;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->wincounts=pRowData->wincounts;
			this->rewardshow=pRowData->rewardshow;
			this->rewarddrop=pRowData->rewarddrop;
			this->boxcooldown=pRowData->boxcooldown;
			this->boxquickopen=pRowData->boxquickopen;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "wincounts", "rewardshow", "rewarddrop", "boxcooldown", "boxquickopen",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CustomSystemRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.wincounts)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.rewardshow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.rewarddrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.boxcooldown)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.boxquickopen)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CustomSystemRewardTable ()
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