#ifndef __RegionSkillList_H__
#define __RegionSkillList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RegionSkillList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SceneID;
		string	SkillScript;
		int	RegionID;
		int	RegionEffect;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneID=pRowData->SceneID;
			this->SkillScript=pRowData->SkillScript;
			this->RegionID=pRowData->RegionID;
			this->RegionEffect=pRowData->RegionEffect;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneID", "SkillScript", "RegionID", "RegionEffect",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RegionSkillList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SkillScript)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.RegionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RegionEffect)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RegionSkillList ()
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