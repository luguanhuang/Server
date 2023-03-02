/*
 * =====================================================================================
 *    Description:  网络包编码 解码器
 *    Author:  LiYang 
 *
 * =====================================================================================
 */

#ifndef _Packet_Coder_H__
#define _Packet_Coder_H__


class IPacketCoder
{
public:
	virtual ~IPacketCoder() {}


	// 返回值:	  表示是否要对数据编码 解码
	//			  返回true才会调用 Encode Decode 接口
	virtual bool IsCoded() const = 0;


	// pcIn:      待编码的缓冲区
	// dwInLen:   待编码的数据长度
	// pcOut:	  编码后到数据输出缓冲区
	// dwOutLen:  输入时为编码缓冲区长度，输出时为编码后的数据长度
	// 返回值:	  true表示编码成功，false表示编码失败(输出缓冲区空间不足)
	virtual bool Encode(const char* pcIn, UINT32 dwInLen, char* pcOut, UINT32& dwOutLen) = 0;


	// pcIn:      待解码的缓冲区
	// dwInLen:   待解码的数据长度，
	// pcOut:	  解码后到数据输出缓冲区
	// dwOutLen:  输入时为解码缓冲区长度，输出时为解码后的数据长度
	// dwUsedLen: 表示输入缓冲区中已被解码使用的数据长度
	// 返回值:	  true表示解码成功，false表示解码失败(输出缓冲区空间不足)
	virtual bool Decode(const char* pcIn, UINT32 dwInLen, char* pcOut, UINT32& dwOutLen, UINT32& dwUsedLen) = 0;


	// pcData:	  待解包缓冲区
	// nLen:	  待解包数据长度
	// nPacketLen:解包成功时为包数据长度
	// 返回值:	  true表示成功解出一个数据包，false表示没解出 (若返回true且nPacketLen == 0 则不处理)
	virtual bool BreakPacket(const char* pcData, UINT32 dwLen, UINT32& dwPacketLen) = 0;
};


#endif
