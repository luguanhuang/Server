#include "pch.h"
#include "common/ptcd2n_mysqlconnectlost.h"
#include "account/accountsessionmgr.h"
#include "config.h"

// generate by ProtoGen at date: 2017/3/10 0:01:55

void PtcD2N_MysqlConnectLost::Process(UINT32 dwConnID)
{
	LogWarn("Mysql Connection Lost, open whitelist, kick all roles now");

	ControlConfig::Instance()->SetUseWhiteList(true);
	CAccountSessionMgr::Instance()->OnMysqlConnectLost();
}
