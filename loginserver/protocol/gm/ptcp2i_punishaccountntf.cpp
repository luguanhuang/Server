#include "pch.h"
#include "gm/ptcp2i_punishaccountntf.h"
#include "iplist/accountbanmgr.h"
#include "idip/ptci2m_delbanaccountntf.h"
#include "idip/ptci2m_banaccountlistntf.h"
#include "network/mslink.h"


// generate by ProtoGen at date: 2017/4/1 17:06:20

void PtcP2I_PunishAccountNtf::Process(UINT32 dwConnID)
{
	UINT32 nType = m_Data.type();
	if (nType==1)
	{
		KKSG::BanAccount acc;
		acc.set_openid(m_Data.account());
		acc.set_endtime(m_Data.endtime());
		acc.set_reason(m_Data.reason());
		AccountBanMgr::Instance()->AddAccount(acc);
		PtcI2M_BanAccountListNtf ntf;
		KKSG::BanAccount *pAcc = ntf.m_Data.add_acclist();
		pAcc->set_openid(m_Data.account());
		pAcc->set_endtime(m_Data.endtime());
		MsLink::Instance()->Brocast(ntf);	
	}else if (nType==2)
	{
		AccountBanMgr::Instance()->DelAccount(m_Data.account());
		PtcI2M_DelBanAccountNtf ptc;
		ptc.m_Data.set_acc(m_Data.account());
		MsLink::Instance()->Brocast(ptc);	
	}
}
