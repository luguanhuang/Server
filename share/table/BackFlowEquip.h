#ifndef __BackFlowEquip_H__
#define __BackFlowEquip_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/20 1:19:25


class BackFlowEquip : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ServerLevel;
		uint	Profession;
		uint	EquipID;
		uint	EnhanceLevel;
		uint	ForgeAttrID;
		Sequence<uint, 3>	EnchantAttr;
		uint	SmeltRate;
		vector<uint>	Jade;

		void CopyFrom(RowData *pRowData)
		{
			this->ServerLevel=pRowData->ServerLevel;
			this->Profession=pRowData->Profession;
			this->EquipID=pRowData->EquipID;
			this->EnhanceLevel=pRowData->EnhanceLevel;
			this->ForgeAttrID=pRowData->ForgeAttrID;
			this->EnchantAttr=pRowData->EnchantAttr;
			this->SmeltRate=pRowData->SmeltRate;
			this->Jade=pRowData->Jade;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ServerLevel", "Profession", "EquipID", "EnhanceLevel", "ForgeAttrID", "EnchantAttr", "SmeltRate", "Jade",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BackFlowEquip  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ServerLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EquipID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.EnhanceLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ForgeAttrID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.EnchantAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SmeltRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Jade)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~BackFlowEquip ()
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