#ifndef __DragonExpList_H__
#define __DragonExpList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class DragonExpList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneID;
		string	Description;
		vector<Sequence<uint, 2>>	ChallengeReward;
		vector<Sequence<uint, 2>>	WinReward;
		string	ResName;
		int	During;
		string	BuffIcon;
		string	BuffDes;
		uint	BossID;
		uint	SealLevel;
		Sequence<uint, 2>	SealBuff;
		Sequence<uint, 2>	ChapterID;
		string	WinHit;
		float	LimitPos;
		vector<float>	SnapPos;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneID=pRowData->SceneID;
			this->Description=pRowData->Description;
			this->ChallengeReward=pRowData->ChallengeReward;
			this->WinReward=pRowData->WinReward;
			this->ResName=pRowData->ResName;
			this->During=pRowData->During;
			this->BuffIcon=pRowData->BuffIcon;
			this->BuffDes=pRowData->BuffDes;
			this->BossID=pRowData->BossID;
			this->SealLevel=pRowData->SealLevel;
			this->SealBuff=pRowData->SealBuff;
			this->ChapterID=pRowData->ChapterID;
			this->WinHit=pRowData->WinHit;
			this->LimitPos=pRowData->LimitPos;
			this->SnapPos=pRowData->SnapPos;

		}
	};

	std::unordered_map<uint,int> SceneID_Index;


    RowData *GetBySceneID(const uint &key)
    {
        auto i = SceneID_Index.find(key);
        return (i == SceneID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneID", "Description", "ChallengeReward", "WinReward", "ResName", "During", "BuffIcon", "BuffDes", "BossID", "SealLevel", "SealBuff", "ChapterID", "WinHit", "LimitPos", "SnapPos",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DragonExpList  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySceneID(pSrcRow->SceneID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SceneID_Index[pNewRow->SceneID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ChallengeReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.WinReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ResName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.During)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.BuffIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BuffDes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.BossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.SealLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.SealBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ChapterID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.WinHit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.LimitPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.SnapPos)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneID_Index[row.SceneID] = Table.size()-1;


		return true;
	}

	~DragonExpList ()
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

		SceneID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif