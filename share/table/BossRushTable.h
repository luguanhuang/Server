#ifndef __BossRushTable_H__
#define __BossRushTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class BossRushTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	bossid;
		Sequence<int, 2>	level;
		vector<int>	bossdifficult;
		int	rank;
		float	atkpercent;
		float	defpercent;
		float	lifepercent;
		string	bosstip;
		int	ragetime;
		Sequence<int, 2>	ragebuff;
		int	qniqueid;
		int	percent;
		vector<Sequence<uint, 2>>	reward;
		string	comment;
		string	bosstip2;
		string	bossTexture;

		void CopyFrom(RowData *pRowData)
		{
			this->bossid=pRowData->bossid;
			this->level=pRowData->level;
			this->bossdifficult=pRowData->bossdifficult;
			this->rank=pRowData->rank;
			this->atkpercent=pRowData->atkpercent;
			this->defpercent=pRowData->defpercent;
			this->lifepercent=pRowData->lifepercent;
			this->bosstip=pRowData->bosstip;
			this->ragetime=pRowData->ragetime;
			this->ragebuff=pRowData->ragebuff;
			this->qniqueid=pRowData->qniqueid;
			this->percent=pRowData->percent;
			this->reward=pRowData->reward;
			this->comment=pRowData->comment;
			this->bosstip2=pRowData->bosstip2;
			this->bossTexture=pRowData->bossTexture;

		}
	};

	std::unordered_map<int,int> qniqueid_Index;


    RowData *GetByqniqueid(const int &key)
    {
        auto i = qniqueid_Index.find(key);
        return (i == qniqueid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "bossid", "level", "bossdifficult", "rank", "atkpercent", "defpercent", "lifepercent", "bosstip", "ragetime", "ragebuff", "qniqueid", "percent", "reward", "comment", "bosstip2", "bossTexture",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BossRushTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByqniqueid(pSrcRow->qniqueid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                qniqueid_Index[pNewRow->qniqueid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.bossid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.bossdifficult)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.atkpercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.defpercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.lifepercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.bosstip)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ragetime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ragebuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.qniqueid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.percent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.comment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.bosstip2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.bossTexture)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		qniqueid_Index[row.qniqueid] = Table.size()-1;


		return true;
	}

	~BossRushTable ()
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

		qniqueid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif