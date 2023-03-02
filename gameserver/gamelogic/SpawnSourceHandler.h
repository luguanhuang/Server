#ifndef __SPAWNSOURCEMONSTERHANDLER_H__
#define __SPAWNSOURCEMONSTERHANDLER_H__

#include "XSpawnInfoGenerator.h"

class SpawnSourceMonsterHandler : public IWaveGenerateHander
{
public:
	virtual void FillWave(Role *pRole, Scene *pScene, XLevelWave *pWave, KKSG::UnitAppearance *pData);
};

class SpawnSourcePlayerHandler : public IWaveGenerateHander
{
public:
	virtual void FillWave(Role *pRole, Scene *pScene, XLevelWave *pWave, KKSG::UnitAppearance *pData);
};

class SpawnSourceRandomHandler : public IWaveGenerateHander
{
public:
	virtual void FillWave(Role *pRole, Scene *pScene, XLevelWave *pWave, KKSG::UnitAppearance *pData);
};

#endif // __SPAWNSOURCEMONSTERHANDLER_H__
