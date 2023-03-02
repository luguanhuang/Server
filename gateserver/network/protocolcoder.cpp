#include "pch.h"
#include "protocolcoder.h"
#include "timeutil.h"
#include "coder.h"
#include "config.h"
#include "protocol.h"


INSTANCE_SINGLETON(CProtocolCoder)


CProtocolCoder::CProtocolCoder()
{
}

CProtocolCoder::~CProtocolCoder()
{
}

bool CProtocolCoder::Init()
{
	return true;
}

void CProtocolCoder::Uninit()
{
}

void CProtocolCoder::Encode(const char* pcData, UINT32 dwDataLen, DataBlob& roOutData, UINT32 dwPtcType, UINT32& rdwPtcFlag)
{
	if(GateConfig::Instance()->IsCompress() && dwDataLen >= GateConfig::Instance()->GetMinCompressSize())
	{
		if((rdwPtcFlag & PTC_TYPE_COMPRESS) != 0)
		{
			LogWarn("Protocol [%s] has been compressed, ignored", CProtocol::GetProtoName(dwPtcType));
			return;
		}

		ZlibTool *pZlib = ZlibTool::Get();
		pZlib->Clear();
		if(!pZlib->Compress(pcData, dwDataLen))
		{
			LogWarn("Compress protocol [%s] failed, size [%u]", CProtocol::GetProtoName(dwPtcType), dwDataLen);
			return;
		}

		rdwPtcFlag |= PTC_TYPE_COMPRESS;
		roOutData.m_ptr = pZlib->GetBuffer();
		roOutData.m_size = (UINT32)pZlib->GetLength();
		LogDebug("Compress protocol %u [%u -> %u] bytes", dwPtcType, dwDataLen, roOutData.m_size);
	}
}

bool CProtocolCoder::Decode(const char* pcData, UINT32 dwDataLen, DataBlob& roOutData, UINT32 dwPtcType, UINT32& rdwPtcFlag)
{
	if((rdwPtcFlag & PTC_TYPE_COMPRESS) != 0)
	{
		ZlibTool *pZlib = ZlibTool::Get();
		pZlib->Clear();
		if(!pZlib->DeCompress(pcData, dwDataLen))
		{
			LogWarn("Decompress protocol [%s] failed!", CProtocol::GetProtoName(dwPtcType));
			return false;
		}

		roOutData.m_ptr = pZlib->GetBuffer();
		roOutData.m_size = pZlib->GetLength();
		rdwPtcFlag &= ~PTC_TYPE_COMPRESS;
		LogDebug("Decompress protocol %u [%u -> %u] bytes", dwPtcType, dwDataLen, roOutData.m_size);
	}

	return true;
}