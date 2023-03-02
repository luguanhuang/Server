#ifndef __DragonNestTable_H__
#define __DragonNestTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/31 11:31:34


class DragonNestTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	DragonNestID;
		uint	DragonNestType;
		uint	DragonNestDifficulty;
		uint	DragonNestWave;
		int	DragonNestPosX;
		int	DragonNestPosY;
		string	DragonNestIcon;
		string	SuggestAttr;
		vector<Sequence<uint, 3>>	WeakInfo;
		vector<string>	WeakTip1;
		vector<string>	WeakTip2;
		string	WeakNotPassTip1;
		string	WeakNotPassTip2;
		vector<int>	WeakPercent;
		vector<uint>	WeakCombat;
		vector<Sequence<uint, 3>>	WeakInfoEx;
		vector<string>	WeakTip1EX;
		vector<string>	WeakTip2EX;
		vector<int>	WeakPercentEX;
		vector<uint>	WeakCombatEX;
		string	DragonNestAtlas;
		int	MaxDragonDropLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->DragonNestID=pRowData->DragonNestID;
			this->DragonNestType=pRowData->DragonNestType;
			this->DragonNestDifficulty=pRowData->DragonNestDifficulty;
			this->DragonNestWave=pRowData->DragonNestWave;
			this->DragonNestPosX=pRowData->DragonNestPosX;
			this->DragonNestPosY=pRowData->DragonNestPosY;
			this->DragonNestIcon=pRowData->DragonNestIcon;
			this->SuggestAttr=pRowData->SuggestAttr;
			this->WeakInfo=pRowData->WeakInfo;
			this->WeakTip1=pRowData->WeakTip1;
			this->WeakTip2=pRowData->WeakTip2;
			this->WeakNotPassTip1=pRowData->WeakNotPassTip1;
			this->WeakNotPassTip2=pRowData->WeakNotPassTip2;
			this->WeakPercent=pRowData->WeakPercent;
			this->WeakCombat=pRowData->WeakCombat;
			this->WeakInfoEx=pRowData->WeakInfoEx;
			this->WeakTip1EX=pRowData->WeakTip1EX;
			this->WeakTip2EX=pRowData->WeakTip2EX;
			this->WeakPercentEX=pRowData->WeakPercentEX;
			this->WeakCombatEX=pRowData->WeakCombatEX;
			this->DragonNestAtlas=pRowData->DragonNestAtlas;
			this->MaxDragonDropLevel=pRowData->MaxDragonDropLevel;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DragonNestID", "DragonNestType", "DragonNestDifficulty", "DragonNestWave", "DragonNestPosX", "DragonNestPosY", "DragonNestIcon", "SuggestAttr", "WeakInfo", "WeakTip1", "WeakTip2", "WeakNotPassTip1", "WeakNotPassTip2", "WeakPercent", "WeakCombat", "WeakInfoEx", "WeakTip1EX", "WeakTip2EX", "WeakPercentEX", "WeakCombatEX", "DragonNestAtlas", "MaxDragonDropLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DragonNestTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.DragonNestID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.DragonNestType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.DragonNestDifficulty)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DragonNestWave)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DragonNestPosX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.DragonNestPosY)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.DragonNestIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SuggestAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.WeakInfo)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.WeakTip1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.WeakTip2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.WeakNotPassTip1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.WeakNotPassTip2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.WeakPercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.WeakCombat)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.WeakInfoEx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.WeakTip1EX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.WeakTip2EX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.WeakPercentEX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.WeakCombatEX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.DragonNestAtlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.MaxDragonDropLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DragonNestTable ()
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