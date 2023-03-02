#ifndef _MAIL_TABLE_MGR_H_
#define _MAIL_TABLE_MGR_H_

#include "util/utilsingleton.h"
#include "table/MailContent.h"

struct MailConf
{
	std::string m_title;
	std::string m_content;
	std::vector<Sequence<uint, 2>> m_rewards;	// ½±Àø
};

class MailTableMgr : public  Singleton<MailTableMgr>
{
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const MailConf* GetMailConf(UINT32 id);
	const std::string ReplaceString(const std::string& src, const std::string& dest, const std::string& content);

private:
	std::map<UINT32, MailConf> m_id2conf;
	MailContent m_table;
};

#endif