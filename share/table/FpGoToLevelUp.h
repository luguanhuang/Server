#ifndef __FpGoToLevelUp_H__
#define __FpGoToLevelUp_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FpGoToLevelUp : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Id;
		string	Name;
		uint	SystemId;
		string	Tips;
		string	IconName;
		uint	StarNum;
		uint	IsRecommond;

		void CopyFrom(RowData *pRowData)
		{
			this->Id=pRowData->Id;
			this->Name=pRowData->Name;
			this->SystemId=pRowData->SystemId;
			this->Tips=pRowData->Tips;
			this->IconName=pRowData->IconName;
			this->StarNum=pRowData->StarNum;
			this->IsRecommond=pRowData->IsRecommond;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Id", "Name", "SystemId", "Tips", "IconName", "StarNum", "IsRecommond",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FpGoToLevelUp  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SystemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Tips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.IconName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.StarNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.IsRecommond)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FpGoToLevelUp ()
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