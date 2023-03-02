#ifndef __PlayerLevelTable_H__
#define __PlayerLevelTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/18 15:01:10


class PlayerLevelTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Level;
		long long	Exp;
		double	InitialAttr;
		double	Trans1Attr;
		double	Trans2Attr;
		double	MaxMP;
		vector<int>	SuperArmorAttack;
		vector<int>	SuperArmorDefend;
		vector<int>	SuperArmorMax;
		vector<int>	SuperArmorRecovery;
		int	AddSkillPoint;
		double	ExpAddition;
		uint	MaxEnhanceLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->Exp=pRowData->Exp;
			this->InitialAttr=pRowData->InitialAttr;
			this->Trans1Attr=pRowData->Trans1Attr;
			this->Trans2Attr=pRowData->Trans2Attr;
			this->MaxMP=pRowData->MaxMP;
			this->SuperArmorAttack=pRowData->SuperArmorAttack;
			this->SuperArmorDefend=pRowData->SuperArmorDefend;
			this->SuperArmorMax=pRowData->SuperArmorMax;
			this->SuperArmorRecovery=pRowData->SuperArmorRecovery;
			this->AddSkillPoint=pRowData->AddSkillPoint;
			this->ExpAddition=pRowData->ExpAddition;
			this->MaxEnhanceLevel=pRowData->MaxEnhanceLevel;

		}
	};

	std::unordered_map<int,int> Level_Index;


    RowData *GetByLevel(const int &key)
    {
        auto i = Level_Index.find(key);
        return (i == Level_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "Exp", "InitialAttr", "Trans1Attr", "Trans2Attr", "MaxMP", "SuperArmorAttack", "SuperArmorDefend", "SuperArmorMax", "SuperArmorRecovery", "AddSkillPoint", "ExpAddition", "MaxEnhanceLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PlayerLevelTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByLevel(pSrcRow->Level);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Level_Index[pNewRow->Level]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Exp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.InitialAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Trans1Attr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Trans2Attr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MaxMP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SuperArmorAttack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SuperArmorDefend)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.SuperArmorMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.SuperArmorRecovery)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.AddSkillPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ExpAddition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.MaxEnhanceLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Level_Index[row.Level] = Table.size()-1;


		return true;
	}

	~PlayerLevelTable ()
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

		Level_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif