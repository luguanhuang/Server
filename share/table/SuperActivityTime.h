#ifndef __SuperActivityTime_H__
#define __SuperActivityTime_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/7 17:48:37


class SuperActivityTime : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	actid;
		uint	systemid;
		uint	minlevel;
		uint	starttime;
		uint	duration;
		uint	rewardtime;
		uint	pointid;
		uint	needpoint;
		vector<Sequence<uint, 2>>	bigprize;
		float	rate;
		uint	starthour;
		uint	durationhour;
		uint	datetype;
		vector<Sequence<uint, 2>>	timestage;

		void CopyFrom(RowData *pRowData)
		{
			this->actid=pRowData->actid;
			this->systemid=pRowData->systemid;
			this->minlevel=pRowData->minlevel;
			this->starttime=pRowData->starttime;
			this->duration=pRowData->duration;
			this->rewardtime=pRowData->rewardtime;
			this->pointid=pRowData->pointid;
			this->needpoint=pRowData->needpoint;
			this->bigprize=pRowData->bigprize;
			this->rate=pRowData->rate;
			this->starthour=pRowData->starthour;
			this->durationhour=pRowData->durationhour;
			this->datetype=pRowData->datetype;
			this->timestage=pRowData->timestage;

		}
	};

	std::unordered_map<uint,int> actid_Index;


    RowData *GetByactid(const uint &key)
    {
        auto i = actid_Index.find(key);
        return (i == actid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "actid", "systemid", "minlevel", "starttime", "duration", "rewardtime", "pointid", "needpoint", "bigprize", "rate", "starthour", "durationhour", "datetype", "timestage",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SuperActivityTime  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByactid(pSrcRow->actid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                actid_Index[pNewRow->actid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.actid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.systemid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.minlevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.starttime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.rewardtime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.pointid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.needpoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.bigprize)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.rate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.starthour)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.durationhour)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.datetype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.timestage)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		actid_Index[row.actid] = Table.size()-1;


		return true;
	}

	~SuperActivityTime ()
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

		actid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif