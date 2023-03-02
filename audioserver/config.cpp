#include "pch.h"
#include "config.h"
#include "network/gatelink.h"
#include "network/centerlink.h"

INSTANCE_SINGLETON(AudioConfig)


AudioConfig::AudioConfig()
{
	SetServerTag("AudioServer");

	GateLink::CreateInstance();
	AddLinkHandler(GateLink::Instance());
	CenterLink::CreateInstance();
	AddLinkHandler(CenterLink::Instance());
	m_bDebug = false;
}

AudioConfig::~AudioConfig()
{
	GateLink::DestroyInstance();
	CenterLink::DestroyInstance();
}

bool AudioConfig::ReadSettings(tinyxml2::XMLElement *element)
{
	tinyxml2::XMLElement* elem = element->FirstChildElement("TestApolloEnv");
	if(elem != NULL)
	{
		m_bDebug = true;
	}
	return true;
}

