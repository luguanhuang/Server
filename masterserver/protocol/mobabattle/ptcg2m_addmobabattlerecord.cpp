#include "pch.h"
#include "mobabattle/ptcg2m_addmobabattlerecord.h"
#include "mobabattle/mobarecordmgr.h"

// generate by ProtoGen at date: 2017/6/16 20:16:16

void PtcG2M_AddMobaBattleRecord::Process(UINT32 dwConnID)
{
	MobaRecordMgr::Instance()->AddMobaBattleRecord(m_Data);
}
