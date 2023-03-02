#ifndef __JadeEquipTable_H__
#define __JadeEquipTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class JadeEquipTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		vector<uint>	Jade1Type;
		vector<uint>	Jade2Type;
		vector<uint>	Jade3Type;
		vector<uint>	Jade4Type;

		void CopyFrom(RowData *pRowData)
		{
			this->Jade1Type=pRowData->Jade1Type;
			this->Jade2Type=pRowData->Jade2Type;
			this->Jade3Type=pRowData->Jade3Type;
			this->Jade4Type=pRowData->Jade4Type;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Jade1Type", "Jade2Type", "Jade3Type", "Jade4Type",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(JadeEquipTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Jade1Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Jade2Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Jade3Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Jade4Type)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~JadeEquipTable ()
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