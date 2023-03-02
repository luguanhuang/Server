#ifndef __AUDIOCONFIG_H__
#define __AUDIOCONFIG_H__

#include "baseconfig.h"

class AudioConfig : public Config
{
	AudioConfig();
	~AudioConfig();
	DECLARE_SINGLETON(AudioConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	bool GetDebugEnv() { return  m_bDebug; }

private:
	bool  m_bDebug;
};

#endif