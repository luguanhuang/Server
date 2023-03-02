#ifndef __IBShop_H__
#define __IBShop_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/21 16:03:16


class IBShop : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	type;
		string	name;
		uint	itemid;
		uint	discount;
		uint	viplevel;
		bool	bind;
		uint	levelshow;
		uint	levelbuy;
		uint	buycount;
		uint	refreshtype;
		uint	currencytype;
		uint	currencycount;
		vector<Sequence<string, 2>>	limittime;
		uint	opensystemtime;
		uint	newproduct;
		int	sortid;
		uint	ischeckpaymember;
		string	activitytime;
		vector<uint>	activitycount;
		Sequence<uint, 2>	level;
		uint	rmb;
		string	goodsid;
		uint	paydegree;
		bool	fashion;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->type=pRowData->type;
			this->name=pRowData->name;
			this->itemid=pRowData->itemid;
			this->discount=pRowData->discount;
			this->viplevel=pRowData->viplevel;
			this->bind=pRowData->bind;
			this->levelshow=pRowData->levelshow;
			this->levelbuy=pRowData->levelbuy;
			this->buycount=pRowData->buycount;
			this->refreshtype=pRowData->refreshtype;
			this->currencytype=pRowData->currencytype;
			this->currencycount=pRowData->currencycount;
			this->limittime=pRowData->limittime;
			this->opensystemtime=pRowData->opensystemtime;
			this->newproduct=pRowData->newproduct;
			this->sortid=pRowData->sortid;
			this->ischeckpaymember=pRowData->ischeckpaymember;
			this->activitytime=pRowData->activitytime;
			this->activitycount=pRowData->activitycount;
			this->level=pRowData->level;
			this->rmb=pRowData->rmb;
			this->goodsid=pRowData->goodsid;
			this->paydegree=pRowData->paydegree;
			this->fashion=pRowData->fashion;

		}
	};

	std::unordered_map<uint,int> id_Index;
	std::unordered_map<string,int> goodsid_Index;


    RowData *GetByid(const uint &key)
    {
        auto i = id_Index.find(key);
        return (i == id_Index.end()) ? NULL : Table[i->second];
    }
    
    RowData *GetBygoodsid(const string &key)
    {
        auto i = goodsid_Index.find(key);
        return (i == goodsid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "type", "name", "itemid", "discount", "viplevel", "bind", "levelshow", "levelbuy", "buycount", "refreshtype", "currencytype", "currencycount", "limittime", "opensystemtime", "newproduct", "sortid", "ischeckpaymember", "activitytime", "activitycount", "level", "rmb", "goodsid", "paydegree", "fashion",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(IBShop  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByid(pSrcRow->id);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                id_Index[pNewRow->id]=Table.size()-1;
                goodsid_Index[pNewRow->goodsid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.itemid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.discount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.viplevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.bind)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.levelshow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.levelbuy)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.buycount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.refreshtype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.currencytype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.currencycount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.limittime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.opensystemtime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.newproduct)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.sortid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.ischeckpaymember)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.activitytime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.activitycount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.rmb)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.goodsid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.paydegree)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.fashion)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;
		goodsid_Index[row.goodsid] = Table.size()-1;


		return true;
	}

	~IBShop ()
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

		id_Index.clear();
		goodsid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif