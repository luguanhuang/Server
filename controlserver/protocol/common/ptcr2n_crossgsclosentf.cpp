#include "pch.h"
#include "common/ptcr2n_crossgsclosentf.h"
#include "account/accountsessionmgr.h"

// generate by ProtoGen at date: 2016/11/18 14:22:32

void PtcR2N_CrossGsCloseNtf::Process(UINT32 dwConnID)
{
	SSInfo << "cross gs close, gsline: " << m_Data.gsline() << END;
	CAccountSessionMgr::Instance()->KickoutPlayerOnGsLine(m_Data.gsline(), true);
}
