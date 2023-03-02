#ifndef __QuestionLibraryTable_H__
#define __QuestionLibraryTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/9 12:12:55


class QuestionLibraryTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		int	QAType;
		string	Question;
		vector<string>	Answer;
		int	TimeLimit;
		vector<uint>	Point;
		vector<Sequence<int, 2>>	Luck;
		uint	Week;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->QAType=pRowData->QAType;
			this->Question=pRowData->Question;
			this->Answer=pRowData->Answer;
			this->TimeLimit=pRowData->TimeLimit;
			this->Point=pRowData->Point;
			this->Luck=pRowData->Luck;
			this->Week=pRowData->Week;

		}
	};

	std::unordered_map<int,int> ID_Index;


    RowData *GetByID(const int &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "QAType", "Question", "Answer", "TimeLimit", "Point", "Luck", "Week",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(QuestionLibraryTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByID(pSrcRow->ID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ID_Index[pNewRow->ID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.QAType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Question)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Answer)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TimeLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Point)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Luck)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Week)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~QuestionLibraryTable ()
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

		ID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif