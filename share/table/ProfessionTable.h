#ifndef __ProfessionTable_H__
#define __ProfessionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/27 16:00:33


class ProfessionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		string	Name;
		int	Sex;
		uint	PresentID;
		vector<Sequence<string, 2>>	InitSkill;
		vector<Sequence<string, 2>>	InitBindSkill;
		uint	AttackType;
		vector<uint>	WeddingFashion;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Name=pRowData->Name;
			this->Sex=pRowData->Sex;
			this->PresentID=pRowData->PresentID;
			this->InitSkill=pRowData->InitSkill;
			this->InitBindSkill=pRowData->InitBindSkill;
			this->AttackType=pRowData->AttackType;
			this->WeddingFashion=pRowData->WeddingFashion;

		}
	};

	std::unordered_map<uint,int> ID_Index;


    RowData *GetByProfID(const uint &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfID", "ProfName", "ProfSex", "PresentID", "InitSkill", "InitBindSkill", "AttackType", "WeddingFashion",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ProfessionTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByProfID(pSrcRow->ID);
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
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Sex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PresentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.InitSkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.InitBindSkill)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.AttackType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.WeddingFashion)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~ProfessionTable ()
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