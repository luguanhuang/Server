#ifndef __SCENETRANSCONFIG_H__
#define __SCENETRANSCONFIG_H__

#include "table/SceneTrans.h"

struct TransConfig
{
	Sequence<UINT32, 2> timespan;
	std::map<UINT32, Sequence<UINT32, 3>> timecycle;
	UINT32 interval;
	std::vector<Sequence<UINT32, 3>> randombuff;

	TransConfig()
	{
		interval = (UINT32)(-1);
	}
};

class SceneTransConfig
{
	DECLARE_SINGLETON(SceneTransConfig);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	TransConfig* GetConfig(UINT32 sceneid);

private:
	SceneTrans m_table;
	std::map<UINT32, TransConfig> m_id2config;
};

#endif