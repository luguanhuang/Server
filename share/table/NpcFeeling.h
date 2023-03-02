#ifndef __NpcFeeling_H__
#define __NpcFeeling_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/20 18:24:38


class NpcFeeling : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	npcId;
		string	name;
		string	introduction;
		uint	openLevel;
		Sequence<uint, 2>	feelingLevel;
		vector<Sequence<uint, 3>>	likeItem;
		vector<Sequence<uint, 3>>	randomItem;
		vector<Sequence<uint, 3>>	favorItem;
		vector<Sequence<uint, 2>>	clientItem;
		vector<Sequence<uint, 2>>	exchangeGive;
		vector<Sequence<uint, 3>>	exchangeGet;
		vector<Sequence<uint, 2>>	npcReturn;
		vector<string>	giveWords;
		vector<string>	giveSuccessWords;
		vector<string>	exchangeWords;
		vector<string>	exchangeSuccessWords;
		uint	npcReturnMailConf;
		string	icon;
		uint	unionId;
		string	relicsName;
		string	relicsIcon;
		string	relicsDesc;
		uint	xnpclistid;

		void CopyFrom(RowData *pRowData)
		{
			this->npcId=pRowData->npcId;
			this->name=pRowData->name;
			this->introduction=pRowData->introduction;
			this->openLevel=pRowData->openLevel;
			this->feelingLevel=pRowData->feelingLevel;
			this->likeItem=pRowData->likeItem;
			this->randomItem=pRowData->randomItem;
			this->favorItem=pRowData->favorItem;
			this->clientItem=pRowData->clientItem;
			this->exchangeGive=pRowData->exchangeGive;
			this->exchangeGet=pRowData->exchangeGet;
			this->npcReturn=pRowData->npcReturn;
			this->giveWords=pRowData->giveWords;
			this->giveSuccessWords=pRowData->giveSuccessWords;
			this->exchangeWords=pRowData->exchangeWords;
			this->exchangeSuccessWords=pRowData->exchangeSuccessWords;
			this->npcReturnMailConf=pRowData->npcReturnMailConf;
			this->icon=pRowData->icon;
			this->unionId=pRowData->unionId;
			this->relicsName=pRowData->relicsName;
			this->relicsIcon=pRowData->relicsIcon;
			this->relicsDesc=pRowData->relicsDesc;
			this->xnpclistid=pRowData->xnpclistid;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "npcId", "name", "introduction", "openLevel", "feelingLevel", "likeItem", "randomItem", "favorItem", "clientItem", "exchangeGive", "exchangeGet", "npcReturn", "giveWords", "giveSuccessWords", "exchangeWords", "exchangeSuccessWords", "npcReturnMailConf", "icon", "unionId", "relicsName", "relicsIcon", "relicsDesc", "xnpclistid",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NpcFeeling  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.npcId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.introduction)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.openLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.feelingLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.likeItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.randomItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.favorItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.clientItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.exchangeGive)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.exchangeGet)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.npcReturn)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.giveWords)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.giveSuccessWords)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.exchangeWords)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.exchangeSuccessWords)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.npcReturnMailConf)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.unionId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.relicsName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.relicsIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.relicsDesc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.xnpclistid)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~NpcFeeling ()
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