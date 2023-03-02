#ifndef __XChapter_H__
#define __XChapter_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class XChapter : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ChapterID;
		string	Comment;
		int	Reward1;
		int	Reward2;
		int	Reward3;
		int	Type;
		string	Pic;
		int	PreChapter;
		vector<Sequence<int, 2>>	Drop;
		Sequence<int, 2>	Difficult;
		int	BossID;

		void CopyFrom(RowData *pRowData)
		{
			this->ChapterID=pRowData->ChapterID;
			this->Comment=pRowData->Comment;
			this->Reward1=pRowData->Reward1;
			this->Reward2=pRowData->Reward2;
			this->Reward3=pRowData->Reward3;
			this->Type=pRowData->Type;
			this->Pic=pRowData->Pic;
			this->PreChapter=pRowData->PreChapter;
			this->Drop=pRowData->Drop;
			this->Difficult=pRowData->Difficult;
			this->BossID=pRowData->BossID;

		}
	};

	std::unordered_map<int,int> ChapterID_Index;


    RowData *GetByChapterID(const int &key)
    {
        auto i = ChapterID_Index.find(key);
        return (i == ChapterID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ChapterID", "Comment", "Reward1", "Reward2", "Reward3", "Type", "Pic", "PreChapter", "Drop", "Difficult", "BossID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(XChapter  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByChapterID(pSrcRow->ChapterID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ChapterID_Index[pNewRow->ChapterID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ChapterID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Comment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Reward1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Reward2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Reward3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Pic)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.PreChapter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Drop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Difficult)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.BossID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ChapterID_Index[row.ChapterID] = Table.size()-1;


		return true;
	}

	~XChapter ()
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

		ChapterID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif