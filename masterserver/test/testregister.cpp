#include "pch.h"
#include "test/testmgr.h"
#include "rank_test.h"


void RegisterTestCase()
{
	//CTestMgr::Instance()->Register("rank", new CRankTestCase());
	CTestMgr::Instance()->Register("garden", new garden());
}