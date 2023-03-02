#ifndef __EnhanceFxTable_H__
#define __EnhanceFxTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/7 16:19:23


class EnhanceFxTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EnhanceLevel;
		vector<string>	MainWeaponFx;
		uint	ProfID;
		string	MainWeaponMat;
		string	Tips;

		void CopyFrom(RowData *pRowData)
		{
			this->EnhanceLevel=pRowData->EnhanceLevel;
			this->MainWeaponFx=pRowData->MainWeaponFx;
			this->ProfID=pRowData->ProfID;
			this->MainWeaponMat=pRowData->MainWeaponMat;
			this->Tips=pRowData->Tips;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EnhanceLevel", "MainWeaponFx", "ProfID", "MainWeaponMat", "Tips",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EnhanceFxTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.EnhanceLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.MainWeaponFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.MainWeaponMat)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Tips)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EnhanceFxTable ()
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