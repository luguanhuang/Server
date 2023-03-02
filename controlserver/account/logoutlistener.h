#ifndef _H_Logout_Listener_H__
#define _H_Logout_Listener_H__


class ILogoutListener
{
public:
	virtual ~ILogoutListener() {}

	//on logout finished
	virtual void OnLogoutFinished(const std::string& strAccount) = 0;
};


#endif