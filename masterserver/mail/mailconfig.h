#ifndef __MAILCONFIG_H__
#define __MAILCONFIG_H__

#include "table/MailContent.h"

struct MailConf
{
	std::string m_title;
	std::string m_content;
	std::vector<Sequence<uint, 2>> m_rewards;	// ½±Àø
};

class MailConfig
{
	MailConfig();
	~MailConfig();
	DECLARE_SINGLETON(MailConfig);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const MailConf* GetMailConf(UINT32 id);
	const std::string ReplaceString(const std::string& src, const std::string& dest, const std::string& content);
	void GetMailString(UINT32 id, std::string& strMailTitle, std::string& strMailContent);

private:
	std::map<UINT32, MailConf> m_id2conf;
	MailContent m_table;
};

#endif