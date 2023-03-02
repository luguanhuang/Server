#ifndef __GMMGR_H__
#define __GMMGR_H__

#include "cmdline.h"
#include "idip/idip.h"

class GMMgr
{
	DECLARE_SINGLETON(GMMgr);
public:
	bool Init();
	void Uninit();

	bool Load();
	bool Reload();

	bool IsGM(const std::string& account);
	bool AddGM(const std::string& account);
	bool DelGM(const std::string& account);
	void DumpGM();

	bool HttpQuery(std::map<std::string, std::string>& keyvalues, std::string& result);
	bool HttpAdd(std::map<std::string, std::string>& keyvalues, std::string& result);
	bool HttpDel(std::map<std::string, std::string>& keyvalues, std::string& result);
	bool IdipAdd(std::map<std::string, std::string>& keyvalues, INT32& errcode);

	static bool CmdAddGM(const CmdLineArgs &args, std::string &outputMessage);
	static bool CmdDelGM(const CmdLineArgs &args, std::string &outputMessage);
	static bool CmdDumpGM(const CmdLineArgs &args, std::string &outputMessage);
private:
	void _Add(const std::string& account);
	void _Del(const std::string& account);

private:
	//std::set<std::string> m_internals;
	//std::set<std::string> m_externals;
	//std::set<std::string> m_phones;
	std::set<std::string> m_gmAccounts;
};

#endif