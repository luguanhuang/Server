#ifndef __CHANNELCLOSESYS_H__
#define __CHANNELCLOSESYS_H__
#include "OpenSystemTable.h"
class Role;


class ChannelCloseSysMgr
{
	ChannelCloseSysMgr();
	~ChannelCloseSysMgr();
	DECLARE_SINGLETON(ChannelCloseSysMgr)
public:
	struct stChannelCloseSys
	{
		std::vector<UINT32> qq;
		std::vector<UINT32> weixin;
	};
	bool Init();
	void Uninit();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	bool IsSysOpen(Role *pRole,UINT32 nSysID,const std::string &channel);
private:
	void _LoadCloseSystem();
	std::unordered_map<std::string,stChannelCloseSys> m_mapCloseSys;
};

#endif // __OPENSYSTEMMGR_H__

