#include "pch.h"
#include "testmgr.h"

INSTANCE_SINGLETON(CTestMgr)


CTestMgr::CTestMgr()
{
}

CTestMgr::~CTestMgr()
{
}

bool CTestMgr::Init()
{
	RegisterTestCase();
	CmdLine::RegistCmd("runtest", CTestMgr::OnCmdLineRunTest);
	return true;
}

void CTestMgr::Uninit()
{
	for(CTestCastMap::iterator it = m_oTestMap.begin(); it != m_oTestMap.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_oTestMap.clear();
}

void CTestMgr::Register(const std::string& strName, CTestCase* poTest)
{
	m_oTestMap[strName] = poTest;
}

CTestCase* CTestMgr::GetTest(const std::string& strName)
{
	CTestCastMap::iterator it = m_oTestMap.find(strName);
	return it == m_oTestMap.end() ? NULL : it->second;
}

bool CTestMgr::OnCmdLineRunTest(const CmdLineArgs &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		LogError("no test case specified");
		return false;
	}

	const std::string& strName = args[1];
	CTestCase* poTest = CTestMgr::Instance()->GetTest(strName);
	if(poTest == NULL)
	{
		LogError("test case [%s] doesn't exist", strName.c_str());
		return false;
	}

	CArgList oArgList;
	oArgList.assign(args.begin() + 2, args.end());

	CTestCase* poNew = poTest->New();
	poNew->Init();
	poNew->Run(oArgList);
	poNew->Uninit();
	delete poNew;
	return true;
}