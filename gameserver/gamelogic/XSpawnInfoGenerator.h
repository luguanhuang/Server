#ifndef __XSPAWNINFOGENERATOR_H__
#define __XSPAWNINFOGENERATOR_H__

class Role;
class Scene;
class XLevelWave;

namespace KKSG
{
	class SceneCfg;
	class UnitAppearance;
}

class IWaveGenerateHander
{
public:
	virtual ~IWaveGenerateHander(){}
	virtual void FillWave(Role *pRole, Scene *pScene, XLevelWave *pWave, KKSG::UnitAppearance *pData) = 0;
};

class XSpawnInfoGenerator
{
	XSpawnInfoGenerator();
	~XSpawnInfoGenerator();
	DECLARE_SINGLETON(XSpawnInfoGenerator)

public:

	bool Init();
	void Uninit();

	void RegistHandler(int type, IWaveGenerateHander *handler);
	void GenerateWaveData(Role *pRole, Scene *pScene, KKSG::SceneCfg *pData);

private:

	std::unordered_map<int, IWaveGenerateHander*> m_handler;
};

#endif // __XSPAWNINFOGENERATOR_H__