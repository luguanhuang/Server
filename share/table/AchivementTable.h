#ifndef __AchivementTable_H__
#define __AchivementTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class AchivementTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	AchievementID;
		int	AchievementType;
		string	AchievementName;
		string	AchievementIcon;
		int	AchievementLevel;
		vector<Sequence<int, 2>>	AchievementItem;
		int	AchievementParam;
		string	AchievementDescription;
		int	AchievementCategory;
		int	AchievementActiveDays;
		int	AchievementFetchDays;
		int	ShowAchivementTip;

		void CopyFrom(RowData *pRowData)
		{
			this->AchievementID=pRowData->AchievementID;
			this->AchievementType=pRowData->AchievementType;
			this->AchievementName=pRowData->AchievementName;
			this->AchievementIcon=pRowData->AchievementIcon;
			this->AchievementLevel=pRowData->AchievementLevel;
			this->AchievementItem=pRowData->AchievementItem;
			this->AchievementParam=pRowData->AchievementParam;
			this->AchievementDescription=pRowData->AchievementDescription;
			this->AchievementCategory=pRowData->AchievementCategory;
			this->AchievementActiveDays=pRowData->AchievementActiveDays;
			this->AchievementFetchDays=pRowData->AchievementFetchDays;
			this->ShowAchivementTip=pRowData->ShowAchivementTip;

		}
	};

	std::unordered_map<int,int> AchievementID_Index;


    RowData *GetByAchievementID(const int &key)
    {
        auto i = AchievementID_Index.find(key);
        return (i == AchievementID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "AchievementID", "AchievementType", "AchievementName", "AchievementIcon", "AchievementLevel", "AchievementItem", "AchievementParam", "AchievementDescription", "AchievementCategory", "AchievementActiveDays", "AchievementFetchDays", "ShowAchivementTip",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AchivementTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByAchievementID(pSrcRow->AchievementID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                AchievementID_Index[pNewRow->AchievementID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.AchievementID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AchievementType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AchievementName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AchievementIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.AchievementLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.AchievementItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.AchievementParam)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.AchievementDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.AchievementCategory)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.AchievementActiveDays)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.AchievementFetchDays)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ShowAchivementTip)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		AchievementID_Index[row.AchievementID] = Table.size()-1;


		return true;
	}

	~AchivementTable ()
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

		AchievementID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif