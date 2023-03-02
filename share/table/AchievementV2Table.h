#ifndef __AchievementV2Table_H__
#define __AchievementV2Table_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class AchievementV2Table : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		string	Achievement;
		int	Type;
		string	Explanation;
		int	CompleteType;
		vector<int>	CompleteValue;
		vector<Sequence<int, 2>>	Reward;
		string	ICON;
		string	DesignationName;
		int	GainShowIcon;
		int	SortID;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Achievement=pRowData->Achievement;
			this->Type=pRowData->Type;
			this->Explanation=pRowData->Explanation;
			this->CompleteType=pRowData->CompleteType;
			this->CompleteValue=pRowData->CompleteValue;
			this->Reward=pRowData->Reward;
			this->ICON=pRowData->ICON;
			this->DesignationName=pRowData->DesignationName;
			this->GainShowIcon=pRowData->GainShowIcon;
			this->SortID=pRowData->SortID;

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
		const char *Headers[] = { "ID", "Achievement", "Type", "Explanation", "CompleteType", "CompleteValue", "Reward", "ICON", "DesignationName", "GainShowIcon", "SortID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AchievementV2Table  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Achievement)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Explanation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.CompleteType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.CompleteValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ICON)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.DesignationName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.GainShowIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.SortID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~AchievementV2Table ()
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