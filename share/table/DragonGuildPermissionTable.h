#ifndef __DragonGuilePermissionTable_H__
#define __DragonGuilePermissionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/10 12:05:26


class DragonGuildPermissionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	DragonGuildID;
		int	DGPOS_LEADER;
		int	DGPOS_VIVELEADER;
		int	DGPOS_MEMBER;

		void CopyFrom(RowData *pRowData)
		{
			this->DragonGuildID=pRowData->DragonGuildID;
			this->DGPOS_LEADER=pRowData->DGPOS_LEADER;
			this->DGPOS_VIVELEADER=pRowData->DGPOS_VIVELEADER;
			this->DGPOS_MEMBER=pRowData->DGPOS_MEMBER;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DragonGuildID", "DGPOS_LEADER", "DGPOS_VIVELEADER", "DGPOS_MEMBER",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DragonGuildPermissionTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.DragonGuildID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.DGPOS_LEADER)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.DGPOS_VIVELEADER)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DGPOS_MEMBER)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DragonGuildPermissionTable ()
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