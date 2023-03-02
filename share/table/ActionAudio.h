#ifndef __ActionAudio_H__
#define __ActionAudio_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ActionAudio : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	Prefab;
		vector<string>	Idle;
		vector<string>	Move;
		vector<string>	Jump;
		vector<string>	Fall;
		vector<string>	Dash;
		vector<string>	Charge;
		vector<string>	Freeze;
		vector<string>	Behit;
		vector<string>	Death;
		vector<string>	BehitFly;
		vector<string>	BehitRoll;
		vector<string>	BehitSuperArmor;

		void CopyFrom(RowData *pRowData)
		{
			this->Prefab=pRowData->Prefab;
			this->Idle=pRowData->Idle;
			this->Move=pRowData->Move;
			this->Jump=pRowData->Jump;
			this->Fall=pRowData->Fall;
			this->Dash=pRowData->Dash;
			this->Charge=pRowData->Charge;
			this->Freeze=pRowData->Freeze;
			this->Behit=pRowData->Behit;
			this->Death=pRowData->Death;
			this->BehitFly=pRowData->BehitFly;
			this->BehitRoll=pRowData->BehitRoll;
			this->BehitSuperArmor=pRowData->BehitSuperArmor;

		}
	};

	std::unordered_map<string,int> Prefab_Index;


    RowData *GetByPrefab(const string &key)
    {
        auto i = Prefab_Index.find(key);
        return (i == Prefab_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Prefab", "Idle", "Move", "Jump", "Fall", "Dash", "Charge", "Freeze", "Behit", "Death", "BehitFly", "BehitRoll", "BehitSuperArmor",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ActionAudio  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByPrefab(pSrcRow->Prefab);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Prefab_Index[pNewRow->Prefab]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Prefab)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Idle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Move)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Jump)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Fall)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Dash)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Charge)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Freeze)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Behit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Death)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.BehitFly)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.BehitRoll)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.BehitSuperArmor)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Prefab_Index[row.Prefab] = Table.size()-1;


		return true;
	}

	~ActionAudio ()
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

		Prefab_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif