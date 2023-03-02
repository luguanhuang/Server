#ifndef _INET_WORK_H__
#define _INET_WORK_H__

class IPacketCoder;
class INetWorkProcess;


#define SOCKET_SEND_BUFF_SIZE		1024 * 256
#define SOCKET_RECV_BUFF_SIZE		1024 * 256
#define GLOBAL_NET_REQ_BUFF_SIZE	1024 * 1024 * 32 
#define GLOBAL_NET_EVT_BUFF_SIZE	1024 * 1024 * 32 

#define MAX_SOCKET_COUNT			10000
#define MAX_PACKAGE_SIZE			1024 * 1024 * 4

#define INVALID_CONN_ID	 			(UINT32)-1
#define PACK_LENGTH					sizeof(UINT32)


enum ENetErrCode
{
	NET_NO_ERROR	= 0,
	NET_SYS_ERROR,
	NET_INVALID_PACKET_SIZE,
	NET_CONNECT_FAILED,
	NET_LISTEN_FAILED,
	NET_ACCEPT_FAILED,
	NET_SEND_FAILED,
	NET_RECV_FAILED,
	NET_ALLOC_FAILED,
	NET_SEND_BUFF_OVERFLOW,
	NET_RECV_BUFF_OVERFLOW,
	NET_ENCODE_BUFF_OVERFLOW,
	NET_DECODE_BUFF_OVERFLOW,

	NET_SEND_PIPE_OVERFLOW,
	NET_RECV_PIPE_OVERFLOW,
};

enum EConnectType
{
	INVALID_CONN,
	INITIATIVE_CONN,
	PASSIVE_CONNE,
};

enum ENetReqType
{
	INVALID_NET_REQ,
	LISTEN_NET_REQ,
	CONNECT_NET_REQ,
	DISCONNECT_NET_REQ,
	SEND_NET_REQ,
};

enum ENetEvtType
{
	INVALID_NET_EVT,
	CONNECT_SUCCEED_NET_EVT,
	CONNECT_FAILED_NET_EVT,
	PASSIVE_CONNECT_NET_EVT,
	RECEIVE_NET_EVT,
	ERROR_NET_EVT,
	CLOSE_NET_EVT,
};


/* 注:  所有传入的和传出的IP Port都是网络字节序 
 *
 */


class INetWork
{
public:
	virtual ~INetWork() {}


	//
	//poCoder:		对数据包进行加密解密, 传入NULL表示不加密解密
	//poBreaker:	断包接口, 接收时会根据该格式解包
	//
	virtual bool Listen(UINT32 dwIP, UINT16 wPort, void* pUserData, IPacketCoder* poCoder, UINT32 dwSendBuffSize, UINT32 dwRecvBuffSize) = 0;

	virtual bool Connect(UINT32 dwIP, UINT16 wPort, void* pUserData, IPacketCoder* poCoder, UINT32 dwSendBuffSize, UINT32 dwRecvBuffSize) = 0;
	virtual bool DisConnect(UINT32 dwConnID) = 0;

	virtual bool Send(UINT32 dwConnID, const void* pData, UINT32 dwLen) = 0;

	virtual void ProcessMsg(UINT32 dwCount) = 0;

	virtual bool GetLocalAddr(UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort) = 0;
	virtual bool GetRemoteAddr(UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort) = 0;

	virtual bool SetConnUserData(UINT32 dwConnID, const CUserData& roUserData) = 0;
	virtual bool GetConnUserData(UINT32 dwConnID, CUserData& roUserData) = 0;

	virtual void Release() = 0;
};


INetWork* CreateNetWork(INetWorkProcess* poProcessor);

#endif
