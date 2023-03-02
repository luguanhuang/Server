#ifndef __STRINGTABLEMGR_H__
#define __STRINGTABLEMGR_H__

#include "table/StringTable.h"


class StringTableMgr
{
	StringTableMgr();
	~StringTableMgr();
	DECLARE_SINGLETON(StringTableMgr)
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	std::string GetText(std::string Enum);

private:
	StringTable m_oTable;
};

#endif