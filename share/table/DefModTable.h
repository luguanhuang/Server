#ifndef __DefModTable_H__
#define __DefModTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class DefModTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Level;
		float	P_10;
		float	P_9;
		float	P_8;
		float	P_7;
		float	P_6;
		float	P_5;
		float	P_4;
		float	P_3;
		float	P_2;
		float	P_1;
		float	P_0;
		float	P1;
		float	P2;
		float	P3;
		float	P4;
		float	P5;
		float	P6;
		float	P7;
		float	P8;
		float	P9;
		float	P10;
		float	P_11;
		float	P_12;
		float	P_13;
		float	P_14;
		float	P_15;
		float	P_16;
		float	P_17;
		float	P_18;
		float	P_19;
		float	P_20;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->P_10=pRowData->P_10;
			this->P_9=pRowData->P_9;
			this->P_8=pRowData->P_8;
			this->P_7=pRowData->P_7;
			this->P_6=pRowData->P_6;
			this->P_5=pRowData->P_5;
			this->P_4=pRowData->P_4;
			this->P_3=pRowData->P_3;
			this->P_2=pRowData->P_2;
			this->P_1=pRowData->P_1;
			this->P_0=pRowData->P_0;
			this->P1=pRowData->P1;
			this->P2=pRowData->P2;
			this->P3=pRowData->P3;
			this->P4=pRowData->P4;
			this->P5=pRowData->P5;
			this->P6=pRowData->P6;
			this->P7=pRowData->P7;
			this->P8=pRowData->P8;
			this->P9=pRowData->P9;
			this->P10=pRowData->P10;
			this->P_11=pRowData->P_11;
			this->P_12=pRowData->P_12;
			this->P_13=pRowData->P_13;
			this->P_14=pRowData->P_14;
			this->P_15=pRowData->P_15;
			this->P_16=pRowData->P_16;
			this->P_17=pRowData->P_17;
			this->P_18=pRowData->P_18;
			this->P_19=pRowData->P_19;
			this->P_20=pRowData->P_20;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "-10", "-9", "-8", "-7", "-6", "-5", "-4", "-3", "-2", "-1", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "-11", "-12", "-13", "-14", "-15", "-16", "-17", "-18", "-19", "-20",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DefModTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.P_10)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.P_9)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.P_8)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.P_7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.P_6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.P_5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.P_4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.P_3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.P_2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.P_1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.P_0)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.P1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.P2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.P3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.P4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.P5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.P6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.P7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.P8)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.P9)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.P10)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.P_11)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.P_12)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.P_13)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.P_14)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.P_15)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.P_16)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.P_17)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.P_18)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.P_19)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.P_20)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DefModTable ()
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