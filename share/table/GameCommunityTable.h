#ifndef __GameCommunityTable_H__
#define __GameCommunityTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GameCommunityTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		string	ButtonName;
		bool	QQ;
		bool	WX;
		int	OpenLevel;
		int	SysID;
		bool	YK;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->ButtonName=pRowData->ButtonName;
			this->QQ=pRowData->QQ;
			this->WX=pRowData->WX;
			this->OpenLevel=pRowData->OpenLevel;
			this->SysID=pRowData->SysID;
			this->YK=pRowData->YK;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "ButtonName", "QQ", "WX", "OpenLevel", "SysID", "YK",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GameCommunityTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ButtonName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.QQ)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.WX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.OpenLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SysID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.YK)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GameCommunityTable ()
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