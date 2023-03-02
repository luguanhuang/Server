#ifndef __OpenSystemTable_H__
#define __OpenSystemTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/2 19:14:48


class OpenSystemTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	TaskID;
		int	PlayerLevel;
		int	SystemID;
		int	NewbieGuideID;
		string	SystemDescription;
		string	Icon;
		int	Priority;
		vector<int>	TitanItems;
		vector<uint>	NoRedPointLevel;
		vector<Sequence<uint, 2>>	Reward;
		int	FatherID;
		uint	OpenDay;
		bool	IsOpen;
		vector<Sequence<uint, 2>>	BackServerOpenDay;
		bool	InNotice;
		vector<Sequence<string, 2>>	NoticeText;
		vector<Sequence<string, 2>>	NoticeIcon;
		vector<string>	NoticeEffect;

		void CopyFrom(RowData *pRowData)
		{
			this->TaskID=pRowData->TaskID;
			this->PlayerLevel=pRowData->PlayerLevel;
			this->SystemID=pRowData->SystemID;
			this->NewbieGuideID=pRowData->NewbieGuideID;
			this->SystemDescription=pRowData->SystemDescription;
			this->Icon=pRowData->Icon;
			this->Priority=pRowData->Priority;
			this->TitanItems=pRowData->TitanItems;
			this->NoRedPointLevel=pRowData->NoRedPointLevel;
			this->Reward=pRowData->Reward;
			this->FatherID=pRowData->FatherID;
			this->OpenDay=pRowData->OpenDay;
			this->IsOpen=pRowData->IsOpen;
			this->BackServerOpenDay=pRowData->BackServerOpenDay;
			this->InNotice=pRowData->InNotice;
			this->NoticeText=pRowData->NoticeText;
			this->NoticeIcon=pRowData->NoticeIcon;
			this->NoticeEffect=pRowData->NoticeEffect;

		}
	};

	std::unordered_map<int,int> SystemID_Index;


    RowData *GetBySystemID(const int &key)
    {
        auto i = SystemID_Index.find(key);
        return (i == SystemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TaskID", "PlayerLevel", "SystemID", "NewbieGuideID", "SystemDescription", "Icon", "Priority", "TitanItems", "NoRedPointLevel", "Reward", "FatherID", "OpenDay", "IsOpen", "BackServerOpenDay", "InNotice", "NoticeText", "NoticeIcon", "NoticeEffect",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(OpenSystemTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySystemID(pSrcRow->SystemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SystemID_Index[pNewRow->SystemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.TaskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.PlayerLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SystemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.NewbieGuideID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SystemDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Priority)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.TitanItems)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.NoRedPointLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.FatherID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.OpenDay)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.IsOpen)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.BackServerOpenDay)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.InNotice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.NoticeText)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.NoticeIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.NoticeEffect)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SystemID_Index[row.SystemID] = Table.size()-1;


		return true;
	}

	~OpenSystemTable ()
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

		SystemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif