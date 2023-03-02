#ifndef __FlowerRain_H__
#define __FlowerRain_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FlowerRain : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	FlowerID;
		int	Count;
		string	EffectPath;
		int	PlayTime;

		void CopyFrom(RowData *pRowData)
		{
			this->FlowerID=pRowData->FlowerID;
			this->Count=pRowData->Count;
			this->EffectPath=pRowData->EffectPath;
			this->PlayTime=pRowData->PlayTime;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "FlowerID", "Count", "EffectPath", "PlayTime",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FlowerRain  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.FlowerID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EffectPath)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PlayTime)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FlowerRain ()
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