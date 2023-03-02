#ifndef _H_Mysql_Test_H_
#define _H_Mysql_Test_H_

#include "cmdline.h"

class CMysqlTest
{
	CMysqlTest();
	~CMysqlTest();
	DECLARE_SINGLETON(CMysqlTest);
public:
	bool Init();
	void Uninit();

	void ReadTest(UINT32 dwCount);
	void UpdateTest(UINT32 dwCount);
	void InsertTest(UINT32 dwCount);

	static bool OnCmdLineRead(const CmdLineArgs &args, std::string &outputMessage);
	static bool OnCmdLineUpdate(const CmdLineArgs &args, std::string &outputMessage);
	static bool OnCmdLineInsert(const CmdLineArgs &args, std::string &outputMessage);
private:
	std::vector<std::string>	m_oUsers;
};


#endif