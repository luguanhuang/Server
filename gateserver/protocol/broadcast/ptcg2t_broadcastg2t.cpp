#include "pch.h"
#include "broadcast/ptcg2t_broadcastg2t.h"
#include "session/sessionmanager.h"
#include "netproc.h"
#include "clientlink.h"
#include "network/protocolcoder.h"
#include "profile/gateprofile.h"

// generate by ProtoGen at date: 2017/2/28 15:32:00

void PtcG2T_BroadCastG2T::Process(UINT32 dwConnID)
{
	if(m_Data.sessionlist_size() == 0 || m_Data.data().empty())
	{
		return;
	}

	static char soTempBuffer[MAX_PACKAGE_SIZE];

	DataBlob newData;
	const char* pcData = m_Data.data().c_str();
	UINT32 dwDataLen = m_Data.data().size();
	ClientProtocolHead* poPtcHead = (ClientProtocolHead*)pcData;

	CProtocolCoder::Instance()->Encode(pcData + sizeof(ClientProtocolHead), dwDataLen - sizeof(ClientProtocolHead), newData, poPtcHead->dwType, poPtcHead->dwFlag);
	if(!newData.IsNull())
	{
		poPtcHead->dwLen = newData.m_size + sizeof(ClientProtocolHead) - sizeof(UINT32);
		memcpy(soTempBuffer, pcData, sizeof(ClientProtocolHead));
		memcpy(soTempBuffer + sizeof(ClientProtocolHead), newData.m_ptr, newData.m_size);

		pcData = soTempBuffer;
		dwDataLen = sizeof(ClientProtocolHead) + newData.m_size;
	}

	for(INT32 i = 0; i < m_Data.sessionlist_size(); ++i)
	{
		UINT64 qwSessionID = m_Data.sessionlist(i);
		UINT32 dwConnID = SessionManager::Instance()->GetSessionConnID(qwSessionID);
		if(dwConnID != INVALID_CONN_ID)
		{
			CNetProcessor::Instance()->Send(dwConnID, pcData, dwDataLen);
		}
	}

	CGateProfile::Instance()->AddBroadCastNum(1);
	CGateProfile::Instance()->AddBroadCastClient(m_Data.sessionlist_size());
}
