#ifndef _H_TestMgr_H__
#define _H_TestMgr_H__

#include "cmdline.h"
#include "testcase.h"


class CTestMgr
{
	CTestMgr();
	~CTestMgr();
	DECLARE_SINGLETON(CTestMgr);
public:
	bool Init();
	void Uninit();

	void Register(const std::string& strName, CTestCase* poTest);
	CTestCase* GetTest(const std::string& strName);

	static bool OnCmdLineRunTest(const CmdLineArgs &args, std::string &outputMessage);
private:
	typedef std::map<std::string, CTestCase*>	CTestCastMap;
	CTestCastMap	m_oTestMap;
};


//每个server单独实现
void RegisterTestCase();

#endif
