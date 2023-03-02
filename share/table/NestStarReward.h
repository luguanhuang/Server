#ifndef __NestStarReward_H__
#define __NestStarReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class NestStarReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		uint	Stars;
		uint	IsHadTittle;
		string	Tittle;
		vector<Sequence<uint, 2>>	Reward;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->Stars=pRowData->Stars;
			this->IsHadTittle=pRowData->IsHadTittle;
			this->Tittle=pRowData->Tittle;
			this->Reward=pRowData->Reward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "Stars", "IsHadTittle", "Tittle", "Reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NestStarReward  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Stars)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.IsHadTittle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Tittle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~NestStarReward ()
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