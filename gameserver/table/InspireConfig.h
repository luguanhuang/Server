#ifndef __INSPIRECONFIG_H__
#define __INSPIRECONFIG_H__

#include "InspireList.h"

class InspireConfig
{
	InspireConfig();
	~InspireConfig();
	DECLARE_SINGLETON(InspireConfig);

public:
	bool Init();
	void Uninit();

	void GetAddAttr(UINT32 scenetype, std::vector<Sequence<int,2>>& addattr);
	bool HaveSceneType(UINT32 scenetype);

private:
	InspireList m_ITable;

};

#endif