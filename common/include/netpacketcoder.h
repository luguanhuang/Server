#ifndef _Net_Packet_Coder_H__
#define _Net_Packet_Coder_H__

#include "packetcoder.h"
#include <string.h>


class CNetPacketCoder : public IPacketCoder
{
public: 
	virtual bool IsCoded() const
	{
		return false;
	}

	virtual bool Encode(const char* pcIn, UINT32 dwInLen, char* pcOut, UINT32& dwOutLen)
	{
		UINT32 dwOutBufferLen = dwOutLen - 4;

		if(dwOutBufferLen < dwInLen) return false;

		memcpy(pcOut + 4, pcIn, dwInLen);
		for(UINT32 i = 0; i < dwInLen; ++i)
		{ 
			*(pcOut + 4 + i) ^= 'x';
		}

		*(UINT32*)pcOut = dwInLen;
		dwOutLen = dwInLen + 4;
		return true;
	}


	virtual bool Decode(const char* pcIn, UINT32 dwInLen, char* pcOut, UINT32& dwOutLen, UINT32& dwUsedLen)
	{
		//UINT32 dwOutBufferLen = dwOutLen;

		dwUsedLen = 0;
		dwOutLen = 0;

		UINT32 dwInPos = 0;
		UINT32 dwLeft = dwInLen;
		while(dwLeft > 0)
		{
			UINT32 dwPackLen = *(UINT32*)(pcIn + dwInPos);
			if(dwPackLen == 0) return false;

			if(dwPackLen + 4 > dwLeft) break;

			memcpy(pcOut + dwOutLen, pcIn + dwInPos + 4, dwPackLen);
			for(UINT32 i = 0; i < dwPackLen; ++i)
			{ 
				*(pcOut + dwOutLen + i) ^= 'x';
			} 

			dwOutLen += dwPackLen;
			dwUsedLen += (4 + dwPackLen); 
			dwInPos = dwUsedLen; 
			dwLeft -= (4 + dwPackLen);
		}

		return true;
	}

	// pcData:	  待解包缓冲区
	// nLen:	  待解包数据长度
	// nPacketLen:解包成功时为包数据长度
	// 返回值:	  true表示成功解出一个数据包，false表示没解出 
	bool BreakPacket(const char* pcData, UINT32 dwLen, UINT32& dwPacketLen)
	{
		if(dwLen < sizeof(UINT32))
		{
			return false;
		}

		dwPacketLen = *(UINT32*)pcData;
		//printf("package length = %d, recv data length = %d\n", dwPacketLen, dwLen);
		if(dwLen < sizeof(UINT32) + dwPacketLen)
		{
			return false;
		}

		dwPacketLen += sizeof(UINT32);
		return true;
	}
};


#endif
