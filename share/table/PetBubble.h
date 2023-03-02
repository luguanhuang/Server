#ifndef __PetBubble_H__
#define __PetBubble_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PetBubble : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	ActionID;
		string	ActionFile;
		string	Condition;
		vector<string>	Bubble;
		float	BubbleTime;
		uint	Weights;
		string	SEFile;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->ActionID=pRowData->ActionID;
			this->ActionFile=pRowData->ActionFile;
			this->Condition=pRowData->Condition;
			this->Bubble=pRowData->Bubble;
			this->BubbleTime=pRowData->BubbleTime;
			this->Weights=pRowData->Weights;
			this->SEFile=pRowData->SEFile;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "ActionID", "ActionFile", "Condition", "Bubble", "BubbleTime", "Weights", "SEFile",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PetBubble  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.ActionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ActionFile)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Condition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Bubble)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BubbleTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Weights)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SEFile)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PetBubble ()
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