#ifndef __QAConditionTable_H__
#define __QAConditionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class QAConditionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	QAType;
		int	StartLevel;
		int	EndLevel;
		int	Count;
		int	CoolDown;
		int	MinPlayerNum;
		int	MaxPlayerNum;
		vector<Sequence<uint, 2>>	Reward;
		vector<Sequence<uint, 2>>	ExtraReward;
		int	PrepareTime;
		vector<Sequence<uint, 2>>	LevelSection;

		void CopyFrom(RowData *pRowData)
		{
			this->QAType=pRowData->QAType;
			this->StartLevel=pRowData->StartLevel;
			this->EndLevel=pRowData->EndLevel;
			this->Count=pRowData->Count;
			this->CoolDown=pRowData->CoolDown;
			this->MinPlayerNum=pRowData->MinPlayerNum;
			this->MaxPlayerNum=pRowData->MaxPlayerNum;
			this->Reward=pRowData->Reward;
			this->ExtraReward=pRowData->ExtraReward;
			this->PrepareTime=pRowData->PrepareTime;
			this->LevelSection=pRowData->LevelSection;

		}
	};

	std::unordered_map<int,int> QAType_Index;


    RowData *GetByQAType(const int &key)
    {
        auto i = QAType_Index.find(key);
        return (i == QAType_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "QAType", "StartLevel", "EndLevel", "Count", "CoolDown", "MinPlayerNum", "MaxPlayerNum", "Reward", "ExtraReward", "PrepareTime", "LevelSection",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(QAConditionTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByQAType(pSrcRow->QAType);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                QAType_Index[pNewRow->QAType]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.QAType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.StartLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EndLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.CoolDown)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MinPlayerNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MaxPlayerNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ExtraReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.PrepareTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.LevelSection)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		QAType_Index[row.QAType] = Table.size()-1;


		return true;
	}

	~QAConditionTable ()
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

		QAType_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif