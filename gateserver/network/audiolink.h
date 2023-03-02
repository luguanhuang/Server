#ifndef __audio_LINK_H__
#define __audio_LINK_H__

#include "baselink.h"

class AudioLink : public BaseLink, public ForwardInterface
{
	AudioLink();
	~AudioLink();
	DECLARE_SINGLETON(AudioLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID) {}
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head);
	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const RpcHead &head);

	virtual bool IsChangeHead() { return true; }
	virtual void GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);
	virtual void GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);

	//Send to ms
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(const CRpc &roRpc);

	UINT32	GetConnID() const { return m_dwConnID; }
	void	SetConnID(UINT32 dwConnID) { m_dwConnID = dwConnID; }
	bool	IsRegistered() const { return m_bRegistered; }
	void	SetRegistered(bool bVal) { m_bRegistered = bVal; }
private:
	UINT32	m_dwConnID;
	bool	m_bRegistered;

	ProtocolHead PtcChangedHead;
	RpcHead RpcChangedHead;
};

#endif