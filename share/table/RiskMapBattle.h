#ifndef __RiskMapBattle_H__
#define __RiskMapBattle_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RiskMapBattle : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	MapID;
		int	SceneID;
		int	Probability;
		vector<int>	DropID;

		void CopyFrom(RowData *pRowData)
		{
			this->MapID=pRowData->MapID;
			this->SceneID=pRowData->SceneID;
			this->Probability=pRowData->Probability;
			this->DropID=pRowData->DropID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "MapID", "SceneID", "Probability", "DropID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RiskMapBattle  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.MapID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Probability)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DropID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RiskMapBattle ()
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