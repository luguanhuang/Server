#ifndef __DanceConfig_H__
#define __DanceConfig_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class DanceConfig : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	MotionID;
		string	MotionName;
		int	PresentID;
		int	SysID;
		string	SysIDName;
		string	Motion;
		string	Music;
		int	LoopCount;
		string	EffectPath;
		float	EffectTime;
		int	Type;
		string	HallBtnIcon;

		void CopyFrom(RowData *pRowData)
		{
			this->MotionID=pRowData->MotionID;
			this->MotionName=pRowData->MotionName;
			this->PresentID=pRowData->PresentID;
			this->SysID=pRowData->SysID;
			this->SysIDName=pRowData->SysIDName;
			this->Motion=pRowData->Motion;
			this->Music=pRowData->Music;
			this->LoopCount=pRowData->LoopCount;
			this->EffectPath=pRowData->EffectPath;
			this->EffectTime=pRowData->EffectTime;
			this->Type=pRowData->Type;
			this->HallBtnIcon=pRowData->HallBtnIcon;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "MotionID", "MotionName", "PresentID", "SysID", "SysIDName", "Motion", "Music", "LoopCount", "EffectPath", "EffectTime", "Type", "HallBtnIcon",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DanceConfig  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.MotionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.MotionName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.PresentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SysID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SysIDName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Motion)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Music)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.LoopCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.EffectPath)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.EffectTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.HallBtnIcon)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DanceConfig ()
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