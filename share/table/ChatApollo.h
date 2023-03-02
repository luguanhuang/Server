#ifndef __ChatApollo_H__
#define __ChatApollo_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ChatApollo : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		int	speak;
		int	music;
		int	click;
		string	note;
		int	opens;
		int	openm;
		string	note2;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->speak=pRowData->speak;
			this->music=pRowData->music;
			this->click=pRowData->click;
			this->note=pRowData->note;
			this->opens=pRowData->opens;
			this->openm=pRowData->openm;
			this->note2=pRowData->note2;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "speak", "music", "click", "note", "opens", "openm", "note2",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ChatApollo  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.speak)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.music)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.click)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.note)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.opens)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.openm)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.note2)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ChatApollo ()
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