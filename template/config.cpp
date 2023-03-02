#include "pch.h"
#include "config.h"
#include "dblink.h"

INSTANCE_SINGLETON(GMConfig)

GMConfig::GMConfig()
{
	SetServerTag("GMServer");
	DBLink::CreateInstance();
	AddLinkHandler(DBLink::Instance());
}

GMConfig::~GMConfig()
{
	DBLink::DestroyInstance();
}

bool GMConfig::ReadSettings(tinyxml2::XMLElement *element)
{
	return true;
}

