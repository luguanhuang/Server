#ifndef __PROTOCOL_FORWARD_H__
#define __PROTOCOL_FORWARD_H__

class BaseLink;

class ProtocolForward
{
public:
	static void RegistForward(UINT32 dwType, BaseLink *link);
	static BaseLink *GetProtocolForwardLink(UINT32 dwType);
};

class RpcForward
{
public:
	static void RegistForward(UINT32 dwType, BaseLink *reqForwardLink, BaseLink *rplForwardLink);
	static BaseLink *GetRpcForwardLink(UINT32 dwType, bool isRequest);
};

#endif