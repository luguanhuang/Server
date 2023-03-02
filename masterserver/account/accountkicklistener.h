#ifndef _ACCOUNT_KICK_LISTENER_H_
#define _ACCOUNT_KICK_LISTENER_H_

#include "accountkick.h"

class KickAccountDelRoleListener : public IAccountKickListener
{
public:
	virtual void OnKickReply(KKSG::ErrorCode nErrCode, const std::string& strAccount, UINT64 qwRoleID);

	static KickAccountDelRoleListener Listener;
};

#endif