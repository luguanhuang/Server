#ifndef _H_ClientConf_Info_H__
#define _H_ClientConf_Info_H__


struct ClientConfInfo
{
	INT32		m_platID;			//ios 0 /android 1

	std::string	m_clientVersion;
	std::string m_systemSoftware;
	std::string m_systemHardware;
	std::string	m_telecomOper;
	std::string	m_network;

	INT32		m_screenWidth;
	INT32		m_screenHeight;
	float		m_density;
	INT32		m_loginChannel;

	std::string	m_cpuHardware;
	INT32		m_memory;

	std::string m_glRender;
	std::string m_glVersion;
	std::string m_deviceId;

	ClientConfInfo()
		:m_platID(-1)
		,m_screenWidth(0)
		,m_screenHeight(0)
		,m_density(0.0f)
		,m_loginChannel(0)
		,m_memory(0)
	{
	}

	~ClientConfInfo()
	{
	}
};

#endif