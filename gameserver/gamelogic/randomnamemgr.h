#ifndef __RANDOMNAMEMGR_H__
#define __RANDOMNAMEMGR_H__

#include "table/RandomName.h"
#include "table/RenameTable.h"

class RandomNameMgr
{
	RandomNameMgr();
	~RandomNameMgr();
	DECLARE_SINGLETON(RandomNameMgr);
public:
	bool Init();
	void Uninit();

	std::string& Random(const std::string& name);

	UINT32 GetRenameConsume(UINT32 count);

private:
	RandomName m_table;
	RenameTable m_renametable;
	std::vector<std::string> m_names;
};

#endif