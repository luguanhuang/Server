#ifndef __BackFlowServerConfig_H__
#define __BackFlowServerConfig_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/23 17:25:09


class BackFlowServerConfig : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ServerLevel;
		uint	Profession;
		vector<uint>	Tutorial;
		uint	MainTaskProcess;
		vector<uint>	BranchTaskProcess;
		uint	MainStageProcess;
		uint	AbyssStageProcess;
		uint	NestStageProcess;
		vector<uint>	Atlas;
		uint	Title;
		vector<uint>	Designation;
		uint	FishLevel;
		uint	CookLevel;
		uint	WorldBoss;
		uint	GuildBoss;
		vector<uint>	MailIDs;
		vector<uint>	Achieve;

		void CopyFrom(RowData *pRowData)
		{
			this->ServerLevel=pRowData->ServerLevel;
			this->Profession=pRowData->Profession;
			this->Tutorial=pRowData->Tutorial;
			this->MainTaskProcess=pRowData->MainTaskProcess;
			this->BranchTaskProcess=pRowData->BranchTaskProcess;
			this->MainStageProcess=pRowData->MainStageProcess;
			this->AbyssStageProcess=pRowData->AbyssStageProcess;
			this->NestStageProcess=pRowData->NestStageProcess;
			this->Atlas=pRowData->Atlas;
			this->Title=pRowData->Title;
			this->Designation=pRowData->Designation;
			this->FishLevel=pRowData->FishLevel;
			this->CookLevel=pRowData->CookLevel;
			this->WorldBoss=pRowData->WorldBoss;
			this->GuildBoss=pRowData->GuildBoss;
			this->MailIDs=pRowData->MailIDs;
			this->Achieve=pRowData->Achieve;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ServerLevel", "Profession", "Tutorial", "MainTaskProcess", "BranchTaskProcess", "MainStageProcess", "AbyssStageProcess", "NestStageProcess", "Atlas", "Title", "Designation", "FishLevel", "CookLevel", "WorldBoss", "GuildBoss", "MailIDs", "Achieve",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BackFlowServerConfig  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[2]], row.Tutorial)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.MainTaskProcess)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.BranchTaskProcess)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MainStageProcess)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.AbyssStageProcess)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.NestStageProcess)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Atlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Designation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.FishLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.CookLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.WorldBoss)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.GuildBoss)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.MailIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.Achieve)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~BackFlowServerConfig ()
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