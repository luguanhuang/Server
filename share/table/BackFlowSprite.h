#ifndef __BackFlowSprite_H__
#define __BackFlowSprite_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/19 17:35:52


class BackFlowSprite : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ServerLevel;
		uint	Profession;
		vector<Sequence<uint, 2>>	Sprite;

		void CopyFrom(RowData *pRowData)
		{
			this->ServerLevel=pRowData->ServerLevel;
			this->Profession=pRowData->Profession;
			this->Sprite=pRowData->Sprite;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ServerLevel", "Profession", "Sprite",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BackFlowSprite  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ServerLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Sprite)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~BackFlowSprite ()
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