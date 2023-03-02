#ifndef __PandoraHeart_H__
#define __PandoraHeart_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/16 22:46:19


class PandoraHeart : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	PandoraID;
		uint	ProfID;
		uint	FireID;
		vector<Sequence<uint, 2>>	Reward;
		vector<uint>	DisplaySlot0;
		vector<uint>	DisplayAngle0;
		vector<uint>	DisplaySlot1;
		vector<uint>	DisplayAngle1;
		vector<uint>	DisplaySlot2;
		vector<uint>	DisplayAngle2;
		string	DisplayName0;
		string	DisplayName1;
		string	DisplayName2;
		uint	PandoraDropID;

		void CopyFrom(RowData *pRowData)
		{
			this->PandoraID=pRowData->PandoraID;
			this->ProfID=pRowData->ProfID;
			this->FireID=pRowData->FireID;
			this->Reward=pRowData->Reward;
			this->DisplaySlot0=pRowData->DisplaySlot0;
			this->DisplayAngle0=pRowData->DisplayAngle0;
			this->DisplaySlot1=pRowData->DisplaySlot1;
			this->DisplayAngle1=pRowData->DisplayAngle1;
			this->DisplaySlot2=pRowData->DisplaySlot2;
			this->DisplayAngle2=pRowData->DisplayAngle2;
			this->DisplayName0=pRowData->DisplayName0;
			this->DisplayName1=pRowData->DisplayName1;
			this->DisplayName2=pRowData->DisplayName2;
			this->PandoraDropID=pRowData->PandoraDropID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "PandoraID", "ProfID", "FireID", "Reward", "DisplaySlot0", "DisplayAngle0", "DisplaySlot1", "DisplayAngle1", "DisplaySlot2", "DisplayAngle2", "DisplayName0", "DisplayName1", "DisplayName2", "PandoraDropID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PandoraHeart  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.PandoraID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.FireID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DisplaySlot0)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.DisplayAngle0)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.DisplaySlot1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.DisplayAngle1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.DisplaySlot2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.DisplayAngle2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.DisplayName0)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.DisplayName1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.DisplayName2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.PandoraDropID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PandoraHeart ()
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