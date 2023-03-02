#include "pch.h"
#include "loginprofile.h"
#include "verify/tokenverifymgr.h"
#include "verify/logincontrol.h"

INSTANCE_SINGLETON(CLoginProfiler)

CLoginProfiler::CLoginProfiler()
{
}

CLoginProfiler::~CLoginProfiler()
{
}

bool CLoginProfiler::Init()
{
	StartTimer();
	SetFileName("loginserver");
	return true;
}

void CLoginProfiler::Uninit()
{
	StopTimer();
}

void CLoginProfiler::DoProfile(FILE* fp)
{
	fprintf(fp, "Logining Num: %u\n", CLoginControl::Instance()->GetLoginNum());
	fprintf(fp, "Http TaskNum: %u\n", TokenVerifyMgr::Instance()->GetHttpTaskNum());
	fprintf(fp, "DB TaskNum: %u\n", TokenVerifyMgr::Instance()->GetDbTaskNum());

	CLoginControl::Instance()->ClearLoginNum();
	TokenVerifyMgr::Instance()->ClearHttpTaskNum();
	TokenVerifyMgr::Instance()->ClearDbTaskNum();
}
