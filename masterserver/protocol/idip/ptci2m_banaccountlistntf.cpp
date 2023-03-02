#include "pch.h"
#include "idip/ptci2m_banaccountlistntf.h"
#include "idip/accountbanmgr.h"

// generate by ProtoGen at date: 2017/4/10 22:25:54

void PtcI2M_BanAccountListNtf::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.acclist_size(); i++)
	{
		AccountBanMsMgr::Instance()->AddAccount(m_Data.acclist(i));
	}
}
