#include "pch.h"
#include "config.h"
#include "gslink.h"
#include "mslink.h"
#include "clientlink.h"
#include "audiolink.h"
#include "util.h"
#include "rslink.h"
#include "controllink.h"

INSTANCE_SINGLETON(GateConfig)

GateConfig::GateConfig()
{
	SetServerTag("GateServer");

	GSLink::CreateInstance();
	MSLink::CreateInstance();
	CClientLink::CreateInstance();
	AudioLink::CreateInstance();
	RSLink::CreateInstance();
	ControlLink::CreateInstance();

	AddLinkHandler(GSLink::Instance());
	AddLinkHandler(MSLink::Instance());
	AddLinkHandler(CClientLink::Instance());
	AddLinkHandler(AudioLink::Instance());
	AddLinkHandler(RSLink::Instance());
	AddLinkHandler(ControlLink::Instance());

	m_nReconnectTimeout = 300;
	m_dwLine = 0;
	m_dwGsListenPort = 0;
	m_bCompress = false;
	m_bEncrypt = false;
	m_dwMinCompressSize = 500;

	m_nAppType = KKSG::GAME_APP_WECHAT;
	m_nPlatType = KKSG::PLAT_IOS;

	m_newConnPerSecond = 1000;
	m_tpsPerConn = 100;
	m_maxConnection = 20000;
}

GateConfig::~GateConfig()
{
	GSLink::DestroyInstance();
	MSLink::DestroyInstance();
	CClientLink::DestroyInstance();
	AudioLink::DestroyInstance();
	RSLink::DestroyInstance();
	ControlLink::DestroyInstance();
}

bool GateConfig::ReadSettings(tinyxml2::XMLElement *element)
{
	const char* pcLine = element->Attribute("line");
	if(pcLine == NULL)
	{
		LogError("can not find 'line' config");
		//return false;
	}
	m_dwLine = convert<UINT32>(pcLine);

	tinyxml2::XMLElement *elem = element->FirstChildElement("Reconnect");
	if(elem != NULL)
	{
		m_nReconnectTimeout = elem->IntAttribute("timeout");
	}

	elem = element->FirstChildElement("Listen");
	if(elem != NULL)
	{
		for (tinyxml2::XMLElement* node = elem->FirstChildElement("peer"); node != NULL; node = node->NextSiblingElement("peer"))
		{
			const char* pcIP = node->Attribute("ip");
			const char* pcPort = node->Attribute("port");
			const char* pcHandler = node->Attribute("handler");
			if(pcIP == NULL || pcPort == NULL || pcHandler == NULL) continue;
			if(strcmp(pcHandler, "gslink") != 0) continue;

			m_strGsListenIP.assign(pcIP);
			m_dwGsListenPort = convert<UINT32>(pcPort);
		}
	}

	elem = element->FirstChildElement("ConnectLimit");
	if (elem)
	{
		m_newConnPerSecond = elem->UnsignedAttribute("nps");
		m_tpsPerConn = elem->UnsignedAttribute("tps");
		m_maxConnection = elem->UnsignedAttribute("maxConnection");
		LogInfo("nps: %u, tps: %u, maxconn: %u", m_newConnPerSecond, m_tpsPerConn, m_maxConnection);
	}

	if(m_strGsListenIP.empty() || m_dwGsListenPort == 0)
	{
		LogError("gslink config error");
		return false;
	}

	elem = element->FirstChildElement("Compress");
	if(elem != NULL)
	{
		INT32 nEnabled = elem->IntAttribute("enabled");
		m_bCompress = (nEnabled == 1);

		INT32 nThreshold = elem->IntAttribute("threshold");
		if(nThreshold > 0)
		{
			m_dwMinCompressSize = (UINT32)nThreshold;
		}
	}

	elem = element->FirstChildElement("Encrypt");
	if(elem != NULL)
	{
		INT32 nEnabled = elem->IntAttribute("enabled");
		m_bEncrypt = (nEnabled == 1);
	}
	return true;
}

