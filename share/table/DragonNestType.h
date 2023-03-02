#ifndef __DragonNestType_H__
#define __DragonNestType_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class DragonNestType : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	DragonNestType;
		string	TypeName;
		string	TypeBg;
		string	TypeIcon;

		void CopyFrom(RowData *pRowData)
		{
			this->DragonNestType=pRowData->DragonNestType;
			this->TypeName=pRowData->TypeName;
			this->TypeBg=pRowData->TypeBg;
			this->TypeIcon=pRowData->TypeIcon;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DragonNestType", "TypeName", "TypeBg", "TypeIcon",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DragonNestType  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.DragonNestType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TypeName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TypeBg)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TypeIcon)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DragonNestType ()
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