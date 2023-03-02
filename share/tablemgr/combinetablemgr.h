#ifndef _COMBINE_TABLE_MGR_H_
#define _COMBINE_TABLE_MGR_H_

#include "util/utilsingleton.h"
#include "table/MailContent.h"
#include "table/PlayerLevelTable.h"
#include "define/itemdef.h"

struct MailConf
{
	std::string m_title;
	std::string m_content;
	std::vector<Sequence<uint, 2>> m_rewards;	// ½±Àø
};

#define MAX_LEVEL 1000
class CombineTableMgr : public  Singleton<CombineTableMgr>
{
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const MailConf* GetMailConf(UINT32 id);
	const std::string ReplaceString(const std::string& src, const std::string& dest, const std::string& content);
	PlayerLevelTable::RowData* GetLevelInfo(int level);
	UINT32 GetNeedExp(UINT32 startLevel, UINT32 endLevel);

	std::vector<ItemDesc>* GetRewardByDeltaDay(UINT32 day);

private:
	std::map<UINT32, MailConf> m_id2conf;
	MailContent m_table;
	PlayerLevelTable m_LevelTable;
	PlayerLevelTable::RowData *m_LevelIndex[MAX_LEVEL+1];
};

#endif