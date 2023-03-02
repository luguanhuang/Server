#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{

}