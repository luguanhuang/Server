#ifndef __RegionList_H__
#define __RegionList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RegionList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	RegionID;
		int	RegionType;
		vector<float>	RegionPos;
		vector<float>	RegionSize;

		void CopyFrom(RowData *pRowData)
		{
			this->RegionID=pRowData->RegionID;
			this->RegionType=pRowData->RegionType;
			this->RegionPos=pRowData->RegionPos;
			this->RegionSize=pRowData->RegionSize;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "RegionID", "RegionType", "RegionPos", "RegionSize",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RegionList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.RegionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.RegionType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RegionPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RegionSize)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RegionList ()
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