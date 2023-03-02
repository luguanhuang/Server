#ifndef __ProfSkillTable_H__
#define __ProfSkillTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ProfSkillTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ProfID;
		string	ProfName;
		string	Skill1;
		string	Skill2;
		string	Skill3;
		string	Skill4;
		string	ProfIcon;
		string	ProfPic;
		string	ProfHeadIcon;
		string	ProfHeadIcon2;
		float	FixedEnmity;
		float	EnmityCoefficient;
		string	Description;
		uint	PromoteExperienceID;
		uint	OperateLevel;
		bool	PromoteLR;
		string	ProfNameIcon;
		string	ProfIntro;
		string	ProfTypeIntro;
		string	ProfWord1;
		string	ProfWord2;

		void CopyFrom(RowData *pRowData)
		{
			this->ProfID=pRowData->ProfID;
			this->ProfName=pRowData->ProfName;
			this->Skill1=pRowData->Skill1;
			this->Skill2=pRowData->Skill2;
			this->Skill3=pRowData->Skill3;
			this->Skill4=pRowData->Skill4;
			this->ProfIcon=pRowData->ProfIcon;
			this->ProfPic=pRowData->ProfPic;
			this->ProfHeadIcon=pRowData->ProfHeadIcon;
			this->ProfHeadIcon2=pRowData->ProfHeadIcon2;
			this->FixedEnmity=pRowData->FixedEnmity;
			this->EnmityCoefficient=pRowData->EnmityCoefficient;
			this->Description=pRowData->Description;
			this->PromoteExperienceID=pRowData->PromoteExperienceID;
			this->OperateLevel=pRowData->OperateLevel;
			this->PromoteLR=pRowData->PromoteLR;
			this->ProfNameIcon=pRowData->ProfNameIcon;
			this->ProfIntro=pRowData->ProfIntro;
			this->ProfTypeIntro=pRowData->ProfTypeIntro;
			this->ProfWord1=pRowData->ProfWord1;
			this->ProfWord2=pRowData->ProfWord2;

		}
	};

	std::unordered_map<int,int> ProfID_Index;


    RowData *GetByProfID(const int &key)
    {
        auto i = ProfID_Index.find(key);
        return (i == ProfID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfID", "ProfName", "Skill1", "Skill2", "Skill3", "Skill4", "ProfIcon", "ProfPic", "ProfHeadIcon", "ProfHeadIcon2", "FixedEnmity", "EnmityCoefficient", "Description", "PromoteExperienceID", "OperateLevel", "PromoteLR", "ProfNameIcon", "ProfIntro", "ProfTypeIntro", "ProfWord1", "ProfWord2",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ProfSkillTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByProfID(pSrcRow->ProfID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ProfID_Index[pNewRow->ProfID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ProfName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Skill1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Skill2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Skill3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Skill4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ProfIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ProfPic)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ProfHeadIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ProfHeadIcon2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.FixedEnmity)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.EnmityCoefficient)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.PromoteExperienceID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.OperateLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.PromoteLR)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.ProfNameIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.ProfIntro)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.ProfTypeIntro)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.ProfWord1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.ProfWord2)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ProfID_Index[row.ProfID] = Table.size()-1;


		return true;
	}

	~ProfSkillTable ()
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

		ProfID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif