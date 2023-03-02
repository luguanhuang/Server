#ifndef __TrophyInfo_H__
#define __TrophyInfo_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/7 17:48:54


class TrophyInfo : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		uint	SceneID;
		Sequence<uint, 3>	TrophyScore;
		uint	Third;
		uint	ThirdPara;
		uint	Second;
		uint	SecondPara;
		uint	First;
		uint	FirstPara;
		string	Name;
		string	ThirdDesc;
		string	SecondDesc;
		string	FirstDesc;
		string	Icon;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->SceneID=pRowData->SceneID;
			this->TrophyScore=pRowData->TrophyScore;
			this->Third=pRowData->Third;
			this->ThirdPara=pRowData->ThirdPara;
			this->Second=pRowData->Second;
			this->SecondPara=pRowData->SecondPara;
			this->First=pRowData->First;
			this->FirstPara=pRowData->FirstPara;
			this->Name=pRowData->Name;
			this->ThirdDesc=pRowData->ThirdDesc;
			this->SecondDesc=pRowData->SecondDesc;
			this->FirstDesc=pRowData->FirstDesc;
			this->Icon=pRowData->Icon;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "SceneID", "TrophyScore", "Third", "ThirdPara", "Second", "SecondPara", "First", "FirstPara", "Name", "ThirdDesc", "SecondDesc", "FirstDesc", "Icon",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TrophyInfo  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TrophyScore)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Third)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ThirdPara)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Second)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SecondPara)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.First)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.FirstPara)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ThirdDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.SecondDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.FirstDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.Icon)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~TrophyInfo ()
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