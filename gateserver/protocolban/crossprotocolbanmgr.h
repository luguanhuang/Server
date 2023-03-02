#ifndef _CROSS_PROTOCOLBAN_MGR_H_
#define _CROSS_PROTOCOLBAN_MGR_H_

#include "util/utilsingleton.h"

class CrossProtocolBanMgr : public Singleton<CrossProtocolBanMgr>
{
public:

	bool Init();
	void Uninit();

	bool IsBan(UINT32 protocolId);
	void AddBan(UINT32 protocolId);

private:
	std::unordered_set<UINT32> m_banProtocolIds;
};




#endif