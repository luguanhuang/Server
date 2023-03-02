#include "pch.h"
#include "idip/ptci2m_delbanaccountntf.h"
#include "idip/accountbanmgr.h"


// generate by ProtoGen at date: 2017/4/11 9:35:40

void PtcI2M_DelBanAccountNtf::Process(UINT32 dwConnID)
{
	AccountBanMsMgr::Instance()->DelAccount(m_Data.acc());
}
