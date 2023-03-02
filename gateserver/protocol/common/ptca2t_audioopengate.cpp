#include "pch.h"
#include "common/ptca2t_audioopengate.h"
#include "network/audiolink.h"

// generate by ProtoGen at date: 2016/6/21 10:36:33

void PtcA2T_AudioOpenGate::Process(UINT32 dwConnID)
{
	/*
	SSInfo << "Begin Handle PtcAudioOpenGate" << END;
	AudioLink::Instance()->SetGSOpenFlag(dwConnID, m_Data.flag());

	AudioServerInfo* pInfo = AudioLink::Instance()->GetAudioServerInfoByConn(dwConnID);
	if (NULL == pInfo)
	{
		SSError << "Get Audio Server Info Failed" << END;
		return;
	}

	if (m_Data.flag())
	{
		LogWarn("open gate");
		SSInfo << "Audio Open Gate Success" << END;
		return;
	}
	LogWarn("close gate");
	SSInfo << "Audio Open Gate Failed" << END;
	*/
}
