#ifndef __FashionChallenge_H__
#define __FashionChallenge_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2015/6/27 10:26:13


class FashionChallenge : public CVSReader
{
public:

	struct RowData
	{
		int	ID;
		string	Name;
		int	ProfID;
		int	Quality;
		float	Chance;
		int	FashionID;
		float	AttributesRation;
		int	RewardDropID;
		uint	EnemyID;
		vector<int>	ViewDrop;

	};

	std::unordered_map<int,int> ID_Index;


        RowData *GetByID(const int &key)
        {
            auto i = ID_Index.find(key);
            return (i == ID_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Name", "ProfID", "Quality", "Chance", "FashionID", "AttributesRation", "RewardDropID", "EnemyID", "ViewDrop",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Chance)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.FashionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.AttributesRation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.RewardDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.EnemyID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ViewDrop)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~FashionChallenge ()
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