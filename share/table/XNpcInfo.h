#ifndef __XNpcInfo_H__
#define __XNpcInfo_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/26 10:18:53


class XNpcInfo : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		string	Name;
		uint	PresentID;
		string	Icon;
		string	Portrait;
		uint	SceneID;
		vector<float>	Position;
		vector<float>	Rotation;
		uint	RequiredTaskID;
		vector<string>	Content;
		vector<int>	FunctionList;
		uint	Gazing;
		vector<string>	Voice;
		vector<string>	ShowUp;
		bool	OnlyHead;
		int	LinkSystem;
		uint	NPCType;
		uint	DisappearTask;
		string	SpecialAnim;
		string	SpecialChat;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Name=pRowData->Name;
			this->PresentID=pRowData->PresentID;
			this->Icon=pRowData->Icon;
			this->Portrait=pRowData->Portrait;
			this->SceneID=pRowData->SceneID;
			this->Position=pRowData->Position;
			this->Rotation=pRowData->Rotation;
			this->RequiredTaskID=pRowData->RequiredTaskID;
			this->Content=pRowData->Content;
			this->FunctionList=pRowData->FunctionList;
			this->Gazing=pRowData->Gazing;
			this->Voice=pRowData->Voice;
			this->ShowUp=pRowData->ShowUp;
			this->OnlyHead=pRowData->OnlyHead;
			this->LinkSystem=pRowData->LinkSystem;
			this->NPCType=pRowData->NPCType;
			this->DisappearTask=pRowData->DisappearTask;
			this->SpecialAnim=pRowData->SpecialAnim;
			this->SpecialChat=pRowData->SpecialChat;

		}
	};

	std::unordered_map<uint,int> ID_Index;


    RowData *GetByNPCID(const uint &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "NPCID", "NPCName", "PresentID", "NPCIcon", "NPCPortrait", "NPCScene", "NPCPosition", "NPCRotation", "RequiredTaskID", "Content", "FunctionList", "Gazing", "Voice", "ShowUp", "OnlyHead", "LinkSystem", "NPCType", "DisappearTask", "SpecialAnim", "SpecialChat",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(XNpcInfo  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByNPCID(pSrcRow->ID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ID_Index[pNewRow->ID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.PresentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Portrait)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Position)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Rotation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.RequiredTaskID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Content)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.FunctionList)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Gazing)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Voice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.ShowUp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.OnlyHead)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.LinkSystem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.NPCType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.DisappearTask)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.SpecialAnim)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.SpecialChat)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~XNpcInfo ()
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

		ID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif