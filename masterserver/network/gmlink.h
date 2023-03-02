#ifndef __GMLINK_H__
#define __GMLINK_H__

#include "baselink.h"


class GMLink : public BaseLink
{
	GMLink();
	~GMLink();
	DECLARE_SINGLETON(GMLink)
public:

	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	void Close();

private:

	//UINT32 m_dwConnID;
};

#endif