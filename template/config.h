#ifndef __GSCONFIG_H__
#define __GSCONFIG_H__

#include "baseconfig.h"

class GMConfig : public Config
{
	GMConfig();
	~GMConfig();
	DECLARE_SINGLETON(GMConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);

private:
};

#endif