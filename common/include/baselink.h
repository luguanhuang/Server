#ifndef __BASELINK_H__
#define __BASELINK_H__

#include <assert.h>
#include "coder.h"
#include "baseconfig.h"
#include <unordered_set>

class CProtocol;
class CRpc;

struct RecvParament
{
	UINT32        m_dwConnID;
	UINT32        m_dwLen;
	const char   *m_pcData;
	ProtocolHead *m_pHead;
	RpcHead      *m_pRpcHead;
};

struct HeadStruct
{
	bool          m_IsPtcHead;
	ProtocolHead  m_oPtcHead;
	RpcHead       m_oRpcHead;
	UINT32        m_dwHeadSize;
};

struct HeadInfo
{
	HeadInfo() { m_ptr = NULL, m_size = 0; }

	void SetNull()		{ m_ptr = NULL; m_size = 0; }
	bool IsNull() const { return m_ptr == NULL; }

	char  *m_ptr;
	UINT32 m_size;
};

struct DataBlob
{
	DataBlob() { m_ptr = NULL; m_size = 0; }

	void SetNull()		{ m_ptr = NULL; m_size = 0; }
	bool IsNull() const { return m_ptr == NULL; }

	const char *m_ptr;
	UINT32 m_size;
};

class ForwardInterface
{
public:
	virtual ~ForwardInterface() {}

	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head) = 0;
	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const RpcHead &head) = 0;

	virtual bool IsChangeHead() { return false; }

	//srcHead:	旧包头
	//pcData:   数据
	//dwDataLen:数据长度
	//newHead:	新包头，出错时置Null，不转发
	//newData:	新数据，需要改写数据时设置
	virtual void GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData) {}
	virtual void GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData) {}
};


class BaseLink
{
public:
	BaseLink();
	virtual ~BaseLink();

	virtual void OnConnectSucceed(UINT32 dwConnID) = 0;
	virtual void OnConnectFailed() = 0;
	virtual void OnPassiveConnect(UINT32 dwConnID) = 0;
	virtual void OnClose(UINT32 dwConnID, int err) = 0;
	virtual const std::string &GetName() = 0;

	virtual bool Send(UINT32 dwConnID, const CProtocol& roPtc);
	virtual bool Send(UINT32 dwConnID, CRpc &roRpc);
	virtual HeadStruct DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen);
	virtual UINT32 MinSize();
	void OnRecv(UINT32 dwConnID, const char* pcData, UINT32 dwLen);

	void SetForwardInterface(ForwardInterface *i);
	ForwardInterface *GetForwardInterface();

	void SetPrintProtocol(bool flag);
	void AddPrintIgnoreProtocol(UINT32 proto);

	virtual bool UseKeepAlive() const { return true; }
private:
	enum PtcResult
	{
		PTCRES_SUCCESS,
		PTCRES_ERROR,
		PTCRES_FORWARD,
	};

	PtcResult OnProtocol(RecvParament &parament);
	PtcResult OnRpc(RecvParament &parament);
	void ForwardProtocol(UINT32 dwConnID, DataBlob *rawPacket, DataBlob *protoBody, ProtocolHead &head);
	void ForwardRpc(UINT32 dwConnID, DataBlob *rawPacket, DataBlob *protoBody, RpcHead &head);

	bool m_printProtocol;
	std::unordered_set<UINT32> m_printIgnoreProtocol;

	ForwardInterface *m_pForwardInterface;
};

class ListenLink : public BaseLink
{
public:
	virtual void OnConnectSucceed(UINT32 dwConnID) { assert(false); }
	virtual void OnConnectFailed() { assert(false); }
};


#endif