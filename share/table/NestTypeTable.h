#ifndef __NestTypeTable_H__
#define __NestTypeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/25 15:35:17


class NestTypeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	TypeID;
		string	TypeName;
		string	TypeBg;
		string	TypeIcon;
		vector<float>	TypeBgTransform;

		void CopyFrom(RowData *pRowData)
		{
			this->TypeID=pRowData->TypeID;
			this->TypeName=pRowData->TypeName;
			this->TypeBg=pRowData->TypeBg;
			this->TypeIcon=pRowData->TypeIcon;
			this->TypeBgTransform=pRowData->TypeBgTransform;

		}
	};

	std::unordered_map<int,int> TypeID_Index;


        RowData *GetByTypeID(const int &key)
        {
            auto i = TypeID_Index.find(key);
            return (i == TypeID_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TypeID", "TypeName", "TypeBg", "TypeIcon", "TypeBgTransform",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NestTypeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
           RowData *pDstRow = GetByTypeID(pSrcRow->TypeID);
           if(pDstRow != NULL)
           {
               pDstRow->CopyFrom(pSrcRow);
           }
           else
           {
               RowData* pNewRow = new RowData();
               pNewRow->CopyFrom(pSrcRow);
               Table.push_back(pNewRow);
               TypeID_Index[pNewRow->TypeID]=Table.size()-1;
           }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.TypeID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TypeName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TypeBg)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TypeIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TypeBgTransform)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TypeID_Index[row.TypeID] = Table.size()-1;


		return true;
	}

	~NestTypeTable ()
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