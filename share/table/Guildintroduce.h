#ifndef __Guildintroduce_H__
#define __Guildintroduce_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class Guildintroduce : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	HelpName;
		string	Logo;
		string	Title;
		string	Desc;

		void CopyFrom(RowData *pRowData)
		{
			this->HelpName=pRowData->HelpName;
			this->Logo=pRowData->Logo;
			this->Title=pRowData->Title;
			this->Desc=pRowData->Desc;

		}
	};

	std::unordered_map<string,int> HelpName_Index;


    RowData *GetByHelpName(const string &key)
    {
        auto i = HelpName_Index.find(key);
        return (i == HelpName_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "HelpName", "Logo", "Title", "Desc",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(Guildintroduce  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByHelpName(pSrcRow->HelpName);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                HelpName_Index[pNewRow->HelpName]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.HelpName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Logo)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Desc)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		HelpName_Index[row.HelpName] = Table.size()-1;


		return true;
	}

	~Guildintroduce ()
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

		HelpName_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif