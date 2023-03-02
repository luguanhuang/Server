#ifndef __SpriteSkill_H__
#define __SpriteSkill_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/8 11:01:15


class SpriteSkill : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SkillID;
		string	SkillName;
		uint	SkillQuality;
		uint	PassiveSkillType;
		string	Tips;
		string	Icon;
		string	Detail;
		vector<Sequence<uint, 2>>	Attrs;
		vector<Sequence<uint, 2>>	Buffs;
		int	Duration;
		int	CD;
		vector<Sequence<uint, 2>>	BaseAttrs;
		string	NoticeDetail;
		Sequence<int, 2>	ShowNotice;
		string	Audio;
		string	Atlas;

		void CopyFrom(RowData *pRowData)
		{
			this->SkillID=pRowData->SkillID;
			this->SkillName=pRowData->SkillName;
			this->SkillQuality=pRowData->SkillQuality;
			this->PassiveSkillType=pRowData->PassiveSkillType;
			this->Tips=pRowData->Tips;
			this->Icon=pRowData->Icon;
			this->Detail=pRowData->Detail;
			this->Attrs=pRowData->Attrs;
			this->Buffs=pRowData->Buffs;
			this->Duration=pRowData->Duration;
			this->CD=pRowData->CD;
			this->BaseAttrs=pRowData->BaseAttrs;
			this->NoticeDetail=pRowData->NoticeDetail;
			this->ShowNotice=pRowData->ShowNotice;
			this->Audio=pRowData->Audio;
			this->Atlas=pRowData->Atlas;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SkillID", "SkillName", "SkillQuality", "PassiveSkillType", "Tips", "Icon", "Detail", "Attrs", "Buffs", "Duration", "CD", "BaseAttrs", "NoticeDetail", "ShowNotice", "Audio", "Atlas",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SpriteSkill  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.SkillID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SkillName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SkillQuality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PassiveSkillType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Tips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Detail)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Attrs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Buffs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.CD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.BaseAttrs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.NoticeDetail)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.ShowNotice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.Audio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.Atlas)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SpriteSkill ()
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