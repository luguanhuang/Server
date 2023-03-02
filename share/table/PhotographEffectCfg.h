#ifndef __PhotographEffectCfg_H__
#define __PhotographEffectCfg_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PhotographEffectCfg : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EffectID;
		vector<Sequence<uint, 2>>	Condition;
		string	EffectName;
		string	EffectRoute;
		string	ConditionDesc;
		string	desc;
		uint	SystemID;

		void CopyFrom(RowData *pRowData)
		{
			this->EffectID=pRowData->EffectID;
			this->Condition=pRowData->Condition;
			this->EffectName=pRowData->EffectName;
			this->EffectRoute=pRowData->EffectRoute;
			this->ConditionDesc=pRowData->ConditionDesc;
			this->desc=pRowData->desc;
			this->SystemID=pRowData->SystemID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EffectID", "Condition", "EffectName", "EffectRoute", "ConditionDesc", "desc", "SystemID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PhotographEffectCfg  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.EffectID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Condition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EffectName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.EffectRoute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ConditionDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SystemID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PhotographEffectCfg ()
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