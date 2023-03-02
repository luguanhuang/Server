#ifndef __TITLECONFIG_H__
#define __TITLECONFIG_H__

#include "table/TitleTable.h"

typedef TitleTable::RowData TitleConf;

class TitleConfig
{
public:
	TitleConfig();
	~TitleConfig();
	DECLARE_SINGLETON(TitleConfig)

	bool Init();
	void Uninit();

	const TitleConf* GetTitleTableData(UINT32 titleID, UINT32 prof);
	inline UINT32 GetMaxTitle() const { return m_maxTitle; }

private:
	TitleTable m_oTitleTable;

	UINT32 m_maxTitle;
	std::unordered_map<UINT32, std::vector<TitleConf*>> m_title;
};

#endif