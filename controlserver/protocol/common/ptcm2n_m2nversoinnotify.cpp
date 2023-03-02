#include "pch.h"
#include "common/ptcm2n_m2nversoinnotify.h"
#include "config.h"
#include "version/version.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2017/6/26 21:43:15

void PtcM2N_M2NVersoinNotify::Process(UINT32 dwConnID)
{
	Version& roVersion = ControlConfig::Instance()->GetVersion();
	roVersion.Clear();
	roVersion.type = m_Data.type();

	std::stringstream ss;
	ss <<"set version by ms, type:"<<roVersion.type;

	for (int i = 0; i < m_Data.ios_size(); ++i)
	{
		roVersion.ios.insert(m_Data.ios(i));
		ss <<" ios:"<< m_Data.ios(i);
	}
	for (int i = 0; i < m_Data.android_size(); ++i)
	{
		roVersion.android.insert(m_Data.android(i));
		ss <<" android:"<<m_Data.android(i);
	}
	
	// by huangds. 2018/6/27
	roVersion.pc.insert("0.0.0");

	SSInfo<< ss.str().c_str() <<END;

	MSLink::Instance()->SetVersioned(true);
}
