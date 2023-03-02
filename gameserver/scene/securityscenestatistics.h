#ifndef __SECURITYSCENESTATISTICS_H__
#define __SECURITYSCENESTATISTICS_H__

#include "unit/combatattrdef.h"
#include "cvsreader.h"

class XSecurityDamageStatistics;
class XSecurityHPStatistics;
class XSecurityBuffStatistics;
class XSecuritySkillStatistics;
class XSecurityAIStatistics;
struct HurtInfo;
struct ProjectDamageResult;
class XEntity;
class Scene;
struct RoleBattleRecord;

class XSecuritySceneStatistics
{
public:
	XSecurityHPStatistics* _BossHPInfo;
	XSecurityHPStatistics* _MonsterHPInfo;
	XSecurityDamageStatistics* _BossDamageInfo;
	XSecurityDamageStatistics* _MonsterDamageInfo;
	//public static XSecurityBuffInfo _EnemyBuffInfo = new XSecurityBuffInfo();
	XSecurityAIStatistics* _BossAIInfo;
	XSecurityAIStatistics* _MonsterAIInfo;

	float _BossMoveDistance;
	float _MonsterMoveDistance;

	bool _Inited;

public:
	XSecuritySceneStatistics();
	~XSecuritySceneStatistics();

	void Reset();

	void Merge(const XSecuritySceneStatistics& other);

	void SendCurrentData(Scene* pScene, RoleBattleRecord* pBattleRecord);
	void SendData(RoleBattleRecord* pBattleRecord);

private:
	static XSecuritySceneStatistics* s_Statistics;
};

#endif