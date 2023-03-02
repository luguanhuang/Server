#ifndef __ShareBgTexture_H__
#define __ShareBgTexture_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/19 23:35:41


class ShareBgTexture : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ShareBgType;
		vector<uint>	SubBgIDList;
		vector<string>	TexturePathList;
		vector<string>	Text;

		void CopyFrom(RowData *pRowData)
		{
			this->ShareBgType=pRowData->ShareBgType;
			this->SubBgIDList=pRowData->SubBgIDList;
			this->TexturePathList=pRowData->TexturePathList;
			this->Text=pRowData->Text;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ShareBgType", "SubBgIDList", "TexturePathList", "Text",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ShareBgTexture  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ShareBgType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SubBgIDList)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TexturePathList)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Text)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ShareBgTexture ()
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