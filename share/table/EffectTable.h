#ifndef __EffectTable_H__
#define __EffectTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/8 17:14:53


class EffectTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EffectID;
		uint	BuffID;
		uint	TemplateBuffID;
		string	SkillScript;
		vector<Sequence<int, 3>>	EffectParams;
		vector<Sequence<string, 2>>	ConstantParams;
		uint	SortID;
		uint	CompareSortID;
		vector<Sequence<uint, 2>>	CompareParams;

		void CopyFrom(RowData *pRowData)
		{
			this->EffectID=pRowData->EffectID;
			this->BuffID=pRowData->BuffID;
			this->TemplateBuffID=pRowData->TemplateBuffID;
			this->SkillScript=pRowData->SkillScript;
			this->EffectParams=pRowData->EffectParams;
			this->ConstantParams=pRowData->ConstantParams;
			this->SortID=pRowData->SortID;
			this->CompareSortID=pRowData->CompareSortID;
			this->CompareParams=pRowData->CompareParams;

		}
	};

	std::unordered_map<string,int> SkillScript_Index;


    RowData *GetBySkillScript(const string &key)
    {
        auto i = SkillScript_Index.find(key);
        return (i == SkillScript_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EffectID", "BuffID", "TemplateBuffID", "SkillScript", "EffectParams", "ConstantParams", "SortID", "CompareSortID", "CompareParams",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EffectTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySkillScript(pSrcRow->SkillScript);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SkillScript_Index[pNewRow->SkillScript]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EffectID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.BuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TemplateBuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SkillScript)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.EffectParams)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ConstantParams)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SortID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.CompareSortID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.CompareParams)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SkillScript_Index[row.SkillScript] = Table.size()-1;


		return true;
	}

	~EffectTable ()
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

		SkillScript_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif