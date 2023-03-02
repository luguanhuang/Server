#include "pch.h"
#include "clientlink.h"
#include "crpc.h"
#include "protocol.h"
#include "connection/connectionmgr.h"
#include "version/version.h"
#include "config.h"


INSTANCE_SINGLETON(CClientLink)

CClientLink::CClientLink()
{ 
}

CClientLink::~CClientLink()
{ 
}

void CClientLink::OnPassiveConnect(UINT32 dwConnID)
{
	ConnectionMgr::Instance()->AddConn(dwConnID);

	UINT32 dwIP;
	UINT16 wPort;
	CNetProcessor::Instance()->GetRemoteAddr(dwConnID, dwIP, wPort);
	char IPBuf[64] = {0};
	unsigned char *p = (unsigned char *)&dwIP;
	sprintf(IPBuf, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);

	Send(dwConnID);

	LogInfo("a client %u from %s is connected", dwConnID, IPBuf);
}

void CClientLink::OnClose(UINT32 dwConnID, int err)
{
	ConnectionMgr::Instance()->RemoveConn(dwConnID);
	LogInfo("a client %u is closed", dwConnID);
}

const std::string & CClientLink::GetName()
{
	static const std::string LinkName = "clientlink";
	return LinkName;
}

void CClientLink::Close(UINT32 dwConnID)
{
	CNetProcessor::Instance()->Close(dwConnID);
}

bool CClientLink::Send(UINT32 dwConnID)
{
	MemoryStream *stream = MemoryStream::Get();
	const int HeadSize = sizeof(ClientProtocolHead);

	int offset = HeadSize;

	const Version& version = VersionConfig::Instance()->GetVersion();
	if (version.ios.empty() || version.android.empty())
	{
		LogError("get version failed.");
		return false;
	}

	SuffixWrite(stream, *version.ios.begin(), offset);

	SuffixWrite(stream, "|", offset);

	SuffixWrite(stream, *version.android.begin(), offset);

	SuffixWrite(stream, "|", offset);

	if (VersionConfig::Instance()->ClientUpdate())
	{
		SuffixWrite(stream, "1", offset);
	}
	else
	{
		SuffixWrite(stream, "0", offset);
	}

	SuffixWrite(stream, "|", offset);

	SuffixWrite(stream, VersionConfig::Instance()->GetExtendInfo(), offset);

	ClientProtocolHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}


void CClientLink::SuffixWrite(MemoryStream* stream, const std::string& info, int& offset)
{
	int size = info.length();
	stream->Seek(offset);
	stream->Write(info.c_str(), size);
	offset += size;
}

UINT32 CClientLink::MinSize()
{
	return sizeof(ClientProtocolHead);
}
