#ifndef _PHOTO_MANAGER_
#define _PHOTO_MANAGER_

#include "table/PhotographEffectCfg.h"

class PhotoManager
{
	PhotoManager();
	~PhotoManager();
	DECLARE_SINGLETON(PhotoManager);
public:
	bool Init();
	void Uninit();
	const PhotographEffectCfg& GetEffectCfg()
	{
		return photograph_effect_;
	}
private:
	PhotographEffectCfg photograph_effect_;
};

#endif // __GUILD_H__