#ifndef __ARTIFACTMGR_H__
#define __ARTIFACTMGR_H__

#include "util/utilsingleton.h"


class CombatAttribute;
class ArtifactSuitDetector
{
public:
	ArtifactSuitDetector() {};
	~ArtifactSuitDetector() {};

	void AddArtifact(UINT32 itemID);
	void ApplySuitEffect(CombatAttribute* combatAttr);	

private:
	std::map<UINT32, UINT32> m_suitCounts;
};


struct XItem;
class Role;

class ArtifactMgr : public Singleton<ArtifactMgr>
{
public:
	ArtifactMgr();
	~ArtifactMgr();

	bool Init();
	void Uninit();

	void InitArtifactData(XItem* pItem, bool isRecast = false);

	KKSG::ErrorCode ComposeArtifact(Role* pRole, const std::set<UINT64>& uids, XItem& giveItem);

	void MultiCompose(Role* pRole, UINT32 level, UINT32 quality, KKSG::ArtifactComposeRes& roRes);

	KKSG::ErrorCode RecastArtifact(Role* pRole, XItem* pItem);
	KKSG::ErrorCode FuseArtifact(Role* pRole, XItem* pItem1, XItem* pItem2, bool isUsedStone);
	KKSG::ErrorCode InscriptionArtifact(Role* pRole, XItem* pItem1, XItem* pItem2);

	UINT32 GetBelongLevel(UINT32 level);

private:
	void _GetRandomList(UINT32 count, UINT32 needCount, std::vector<UINT32>& vec);
};


#endif
