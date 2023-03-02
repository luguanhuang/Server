#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/2/6 11:01:29

#include "common/rpco2v_gmtoolcommand2version.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CRpc::RegistRpc(RpcO2V_GMToolCommand2Version::CreateRpc());


	CProtocol::RegistProtoName(RPCO2V_GMTOOLCOMMAND2VERSION_TYPE, "RPCO2V_GMTOOLCOMMAND2VERSION");

}