#ifndef __SkillEmblem_H__
#define __SkillEmblem_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/11/28 16:49:05


class SkillEmblem : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EmblemID;
		string	SkillScript;
		uint	SkillType;
		uint	SkillPercent;
		string	SkillName;
		uint	SkillPPT;
		vector<Sequence<uint, 2>>	ChangeProfReplaceItem;
		string	ExSkillScript;
		vector<string>	OtherSkillScripts;

		void CopyFrom(RowData *pRowData)
		{
			this->EmblemID=pRowData->EmblemID;
			this->SkillScript=pRowData->SkillScript;
			this->SkillType=pRowData->SkillType;
			this->SkillPercent=pRowData->SkillPercent;
			this->SkillName=pRowData->SkillName;
			this->SkillPPT=pRowData->SkillPPT;
			this->ChangeProfReplaceItem=pRowData->ChangeProfReplaceItem;
			this->ExSkillScript=pRowData->ExSkillScript;
			this->OtherSkillScripts=pRowData->OtherSkillScripts;

		}
	};

	std::unordered_map<uint,int> EmblemID_Index;


    RowData *GetByEmblemID(const uint &key)
    {
        auto i = EmblemID_Index.find(key);
        return (i == EmblemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EmblemID", "SkillScript", "SkillType", "SkillPercent", "SkillName", "SkillPPT", "ChangeProfReplaceItem", "ExSkillScript", "OtherSkillScripts",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SkillEmblem  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEmblemID(pSrcRow->EmblemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                EmblemID_Index[pNewRow->EmblemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EmblemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SkillScript)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SkillType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SkillPercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SkillName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SkillPPT)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ChangeProfReplaceItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ExSkillScript)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.OtherSkillScripts)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		EmblemID_Index[row.EmblemID] = Table.size()-1;


		return true;
	}

	~SkillEmblem ()
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

		EmblemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif