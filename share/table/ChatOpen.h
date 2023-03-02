#ifndef __ChatOpen_H__
#define __ChatOpen_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/13 19:46:54


class ChatOpen : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		string	name;
		vector<int>	opens;
		uint	friends;
		int	posX;
		int	posY;
		float	alpha;
		int	pivot;
		float	scale;
		int	fade;
		int	real;
		int	radioX;
		int	radioY;
		int	battle;
		uint	sceneid;
		int	max;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->name=pRowData->name;
			this->opens=pRowData->opens;
			this->friends=pRowData->friends;
			this->posX=pRowData->posX;
			this->posY=pRowData->posY;
			this->alpha=pRowData->alpha;
			this->pivot=pRowData->pivot;
			this->scale=pRowData->scale;
			this->fade=pRowData->fade;
			this->real=pRowData->real;
			this->radioX=pRowData->radioX;
			this->radioY=pRowData->radioY;
			this->battle=pRowData->battle;
			this->sceneid=pRowData->sceneid;
			this->max=pRowData->max;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "name", "opens", "friends", "posX", "posY", "alpha", "pivot", "scale", "fade", "real", "radioX", "radioY", "battle", "sceneid", "max",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ChatOpen  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.opens)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.friends)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.posX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.posY)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.alpha)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.pivot)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.scale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.fade)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.real)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.radioX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.radioY)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.battle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.sceneid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.max)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ChatOpen ()
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