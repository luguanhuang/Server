#ifndef _H_Account_Kick_H__
#define _H_Account_Kick_H__

#include "commondef.h"


class IAccountKickListener
{
public:
	virtual ~IAccountKickListener() {}
	virtual void OnKickReply(KKSG::ErrorCode nErrCode, const std::string& strAccount, UINT64 qwRoleID) = 0;
};


///> 1. idip
///> 2. change profession
///> 3. GM Del Role 
class CAccountKicker
{
	CAccountKicker();
	virtual ~CAccountKicker();
	DECLARE_SINGLETON(CAccountKicker)
public:
	bool Init();
	void Uninit();

	bool KickAccount(UINT64 qwRoleID, KKSG::KickType nKickType, IAccountKickListener* poListener, INT32 dwID=0);
	void OnKickReply(const std::string& strAccount, KKSG::ErrorCode nErrCode);
private:
	struct AccountKickInfo 
	{
		UINT64			m_qwRoleID;
		KKSG::KickType	m_nKickType;
		IAccountKickListener*	m_poListener;
		INT32 dwID;//idiphandler id;
		AccountKickInfo()
			:m_qwRoleID(INVALID_ROLE_ID)
			,m_nKickType(KKSG::KICK_NORMAL)
			,m_poListener(NULL)
			,dwID(0)
		{
		}
	};
private:
	typedef std::unordered_map<std::string, AccountKickInfo>	CKickInfoMap;
	CKickInfoMap	m_oMap;
};

#endif