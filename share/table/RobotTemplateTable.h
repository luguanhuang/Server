#ifndef __RobotTemplateTable_H__
#define __RobotTemplateTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RobotTemplateTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	RobotTemplateID;
		string	RobotName;
		int	RobotSex;
		int	RobotLevel;
		int	RobotProfession;
		int	RobotCamp;
		vector<int>	Equip;
		vector<Sequence<int, 3>>	Smelting;
		vector<int>	Enhance;
		vector<int>	Reinforce;
		int	Skill;
		int	SkillCount;
		vector<Sequence<int, 2>>	Item;
		vector<int>	FashionShow;

		void CopyFrom(RowData *pRowData)
		{
			this->RobotTemplateID=pRowData->RobotTemplateID;
			this->RobotName=pRowData->RobotName;
			this->RobotSex=pRowData->RobotSex;
			this->RobotLevel=pRowData->RobotLevel;
			this->RobotProfession=pRowData->RobotProfession;
			this->RobotCamp=pRowData->RobotCamp;
			this->Equip=pRowData->Equip;
			this->Smelting=pRowData->Smelting;
			this->Enhance=pRowData->Enhance;
			this->Reinforce=pRowData->Reinforce;
			this->Skill=pRowData->Skill;
			this->SkillCount=pRowData->SkillCount;
			this->Item=pRowData->Item;
			this->FashionShow=pRowData->FashionShow;

		}
	};

	std::unordered_map<int,int> RobotTemplateID_Index;


    RowData *GetByRobotTemplateID(const int &key)
    {
        auto i = RobotTemplateID_Index.find(key);
        return (i == RobotTemplateID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "RobotTemplateID", "RobotName", "RobotSex", "RobotLevel", "RobotProfession", "RobotCamp", "Equip", "Smelting", "Enhance", "Reinforce", "Skill", "SkillCount", "Item", "FashionShow",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RobotTemplateTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByRobotTemplateID(pSrcRow->RobotTemplateID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                RobotTemplateID_Index[pNewRow->RobotTemplateID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.RobotTemplateID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.RobotName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RobotSex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RobotLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.RobotProfession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.RobotCamp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Equip)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Smelting)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Enhance)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Reinforce)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Skill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.SkillCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Item)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.FashionShow)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		RobotTemplateID_Index[row.RobotTemplateID] = Table.size()-1;


		return true;
	}

	~RobotTemplateTable ()
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

		RobotTemplateID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif