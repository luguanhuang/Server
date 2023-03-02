#ifndef __SpritePassiveSkillBank_H__
#define __SpritePassiveSkillBank_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SpritePassiveSkillBank : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	PassiveSkillsID;
		uint	SkillEffectID;
		uint	SkillID;
		uint	Weight;

		void CopyFrom(RowData *pRowData)
		{
			this->PassiveSkillsID=pRowData->PassiveSkillsID;
			this->SkillEffectID=pRowData->SkillEffectID;
			this->SkillID=pRowData->SkillID;
			this->Weight=pRowData->Weight;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "PassiveSkillsID", "SkillEffectID", "SkillID", "Weight",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SpritePassiveSkillBank  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.PassiveSkillsID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SkillEffectID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SkillID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Weight)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SpritePassiveSkillBank ()
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


	}

	std::vector<RowData*> Table;
};

#endif