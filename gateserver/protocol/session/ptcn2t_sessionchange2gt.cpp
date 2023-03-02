#include "pch.h"
#include "session/ptcn2t_sessionchange2gt.h"
#include "session/sessionmanager.h"

// generate by ProtoGen at date: 2017/6/20 19:47:52

void PtcN2T_SessionChange2Gt::Process(UINT32 dwConnID)
{
	LogInfo("roleid[%llu] oldsession[%llu] newsession[%llu]", m_Data.roleid(), m_Data.oldsession(), m_Data.newsession());
	SessionManager::Instance()->DestroySessionByNS(m_Data.oldsession());
}
