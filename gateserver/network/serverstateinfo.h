#ifndef __SERVERSTATEINFO_H__
#define __SERVERSTATEINFO_H__

class ServerStateInfo
{
	ServerStateInfo();
	~ServerStateInfo();
	DECLARE_SINGLETON(ServerStateInfo)

public:
	bool Init();
	void Uninit();

};

#endif // __SERVERSTATEINFO_H__

