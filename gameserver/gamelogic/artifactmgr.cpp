#include "pch.h"
#include "artifactmgr.h"
#include "util/XCommon.h"
#include "artifactconfig.h"
#include "item.h"
#include "effectconfig.h"
#include "unit/combatattribute.h"
#include "bag.h"
#include "config/itemdropconfig.h"
#include "bagtransition.h"
#include "utility/uidhelper.h"
#include "utility/tlogmgr.h"


void ArtifactSuitDetector::AddArtifact(UINT32 itemID)
{
	const ArtifactListConf* conf = ArtifactConfig::Instance()->GetArtifactListConf(itemID);
	if (conf == NULL)
	{
		return;
	}
	if (m_suitCounts.find(conf->ArtifactSuit) == m_suitCounts.end())
	{
		m_suitCounts[conf->ArtifactSuit] = 1;
	}
	else
	{
		++m_suitCounts[conf->ArtifactSuit];
	}
}

void ArtifactSuitDetector::ApplySuitEffect(CombatAttribute* combatAttr)
{
	for (auto i = m_suitCounts.begin(); i != m_suitCounts.end(); ++i)
	{
		const ArtifactSuitConf* conf = ArtifactConfig::Instance()->GetArtifactSuitConf(i->first);
		if (conf == NULL)
		{
			continue;
		}
		std::vector<std::vector<Sequence<UINT32, 2>>> addAttr;
#define AddEffect(x) if (i->second >= x) addAttr.push_back(conf->Effect##x)
		AddEffect(2);
		AddEffect(3);
		AddEffect(4);
		AddEffect(5);
		AddEffect(6);
#undef  AddEffect

		for (auto j = addAttr.begin(); j != addAttr.end(); ++j)
			for (auto k = j->begin(); k != j->end(); ++k)
			{
				combatAttr->AddAttr((CombatAttrDef)(*k)[0], (*k)[1]);
			}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ArtifactMgr::ArtifactMgr()
{
}

ArtifactMgr::~ArtifactMgr()
{

}

bool ArtifactMgr::Init()
{
	return true;
}

void ArtifactMgr::Uninit()
{

}

void ArtifactMgr::InitArtifactData(XItem* pItem, bool isRecast)
{
	if (pItem == NULL)
	{
		return;
	}
	const ArtifactListConf* listConf = ArtifactConfig::Instance()->GetArtifactListConf(pItem->itemID);
	if (listConf == NULL)
	{
		return;
	}

	ArtifactConfig::Instance()->InitArtifactAttribute(pItem, isRecast);

	if (!listConf->ProfessionIDs.empty())
	{
		UINT32 effectNum = listConf->EffectNum;
		if (isRecast)
		{
			effectNum = pItem->effects.size();
		}
		pItem->effects.clear();
		UINT32 random = XRandom::randInt(0, listConf->ProfessionIDs.size());
		std::vector<UINT32> effectIDs;
		EffectConfig::Instance()->GetRandomEffectIDs(listConf->EffectRandomIDs, listConf->ProfessionIDs[random], effectNum, effectIDs);
		for (auto i = effectIDs.begin(); i != effectIDs.end(); ++i)
		{
			KKSG::EffectData effectData;
			if (EffectConfig::Instance()->GetEffectData(*i, effectData))
			{
				pItem->effects.push_back(effectData);
			}
		}
		EffectConfig::Instance()->UpdateEffectData(pItem->effects);
	}
}

KKSG::ErrorCode ArtifactMgr::ComposeArtifact(Role* pRole, const std::set<UINT64>& uids, XItem& giveItem)
{
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	UINT32 level = -1, quality = -1;
	for (auto i = uids.begin(); i != uids.end(); ++i)
	{
		XItem* pItem = pRole->Get<Bag>()->FindUniqueItemInBag(*i);
		if (pItem == NULL)
		{
			return KKSG::ERR_ITEM_NOTEXIST;
		}
		if (pItem->type != ARTIFACT)
		{
			LogWarn("ArtifactCompose, item [%llu] 's type [%u] is not ARTIFACT", *i, pItem->type);
			return KKSG::ERR_FAILED;
		}
		UINT32 thisLevel = XItem::GetLevel(pItem->itemID), thisQuality = XItem::GetQuality(pItem->itemID);
		if (thisLevel == -1 || thisQuality == -1)
		{
			return KKSG::ERR_UNKNOWN;
		}
		thisLevel = GetBelongLevel(thisLevel);
		if (i == uids.begin())
		{
			level = thisLevel;
			quality = thisQuality;
		}
		else if (level != thisLevel)
		{
			return KKSG::ERR_ARTIFACT_COMPOSE_LEVELNOTEQUAL;
		}
		else if (quality != thisQuality)
		{
			return KKSG::ERR_ARTIFACT_COMPOSE_QUALITYNOTEQUAL;
		}
	}
	const ArtifactComposeConf* conf = ArtifactConfig::Instance()->GetArtifactComposeConf(level, quality);
	if (conf == NULL)
	{
		return KKSG::ERR_ARTIFACT_COMPOSE_CANTCOMPOSE;
	}
	UINT32 dropID = 0;
	for (auto i = conf->ArtifactNum2DropID.begin(); i != conf->ArtifactNum2DropID.end(); ++i)
	{
		if ((*i)[0] == uids.size())
		{
			dropID = (*i)[1];
			break;
		}
	}
	if (dropID == 0)
	{
		return KKSG::ERR_ARTIFACT_COMPOSE_NUMBER;
	}
	const ItemDesc* pItemDesc = ItemDropConfig::Instance()->GetRandomDrop(dropID, pRole->GetLevel());
	if (pItemDesc == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	std::string strTrans = TTag::Instance()->GetTagString();//根据流水号确认是否为同一次操作;
	for (auto i = uids.begin(); i != uids.end(); ++i)
	{
		XItem* poItem = pRole->Get<Bag>()->FindUniqueItemInBag(*i);
		if (NULL != poItem)
		{
			XItem::PrintArtifactTLog(pRole, poItem, TX_Artifact_ComposeTake, strTrans);
		}
	}

	UINT64 uid = UIDHelper::CreateUID(UID_Item);
	giveItem = XItem(uid, pItemDesc->itemID, pItemDesc->itemCount, ARTIFACT);
	giveItem.isbind = pItemDesc->isbind;

	XItem::PrintArtifactTLog(pRole, &giveItem, TX_Artifact_ComposeGet, strTrans);
	return KKSG::ERR_SUCCESS;
}

void ArtifactMgr::MultiCompose(Role* pRole, UINT32 level, UINT32 quality, KKSG::ArtifactComposeRes& roRes)
{
	if (pRole == NULL) return;
	const ArtifactComposeConf* conf = ArtifactConfig::Instance()->GetArtifactComposeConf(level, quality);
	if (conf == NULL || conf->ArtifactNum2DropID.empty())
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	Sequence<UINT32, 2> num2DropID = *conf->ArtifactNum2DropID.begin();
	if (num2DropID[0] == 0)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	const std::list<XItem*>& artifactList = pRole->Get<Bag>()->GetArtifactItemList();
	std::vector<UINT64> artifactID;
	for (auto i = artifactList.begin(); i != artifactList.end(); ++i)
	{
		XItem* pItem = *i;
		UINT32 thisLevel = XItem::GetLevel(pItem->itemID), thisQuality = XItem::GetQuality(pItem->itemID);
		if (thisLevel == -1 || thisQuality == -1)
		{
			continue;
		}
		thisLevel = GetBelongLevel(thisLevel);
		if (thisLevel == level && thisQuality == quality)
		{
			artifactID.push_back(pItem->uid);
		}
	}
	if (artifactID.size() < num2DropID[0])
	{
		roRes.set_errorcode(KKSG::ERR_ARTIFACT_COMPOSE_NUMBER);
		return;
	}
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactComposeMulti_Take);
	std::vector<XItem> giveItems;
	for (auto i = artifactID.begin(); i != artifactID.end(); )
	{
		std::set<UINT64> tmpuids;
		for (UINT32 j = 0; j < num2DropID[0] && i != artifactID.end(); ++j, ++i)
		{
			tmpuids.insert(*i);
		}
		if (tmpuids.size() == num2DropID[0])
		{
			XItem giveItem;
			if (ComposeArtifact(pRole, tmpuids, giveItem) == KKSG::ERR_SUCCESS)
			{
				for (auto j = tmpuids.begin(); j != tmpuids.end(); ++j)
				{
					take.TakeUniqueItem(*j, 1);
				}
				giveItems.push_back(giveItem);
				roRes.add_newuids(giveItem.uid);
			}
		}
	}
	take.NotifyClient();
	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactComposeMulti_Give);
	for (auto i = giveItems.begin(); i != giveItems.end(); ++i)
	{
		give.GiveItem(&*i);
	}
	give.NotifyClient();

	if (roRes.newuids_size() == 0)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
}

KKSG::ErrorCode ArtifactMgr::RecastArtifact(Role* pRole, XItem* pItem)
{
	if (pRole == NULL || pItem == NULL || pItem->type != ARTIFACT)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(XSys_Artifact_Recast))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	const ArtifactListConf* listConf = ArtifactConfig::Instance()->GetArtifactListConf(pItem->itemID);
	if (listConf == NULL)
	{
		LogWarn("itemid [%u] not in ArtifactList", pItem->itemID);
		return KKSG::ERR_FAILED;
	}

	if (!listConf->IsCanRecast)
	{
		return KKSG::ERR_ARTIFACT_CANTRECAST;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactRecastCost);
	for (auto i = listConf->RecastMaterials.begin(); i != listConf->RecastMaterials.end(); ++i)
	{
		if (!take.TakeItem((*i)[0], (*i)[1]))
		{
			take.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
	}
	take.NotifyClient();

	InitArtifactData(pItem, true);

	BagUpdateItemTransition update(pRole);
	update.AddChangedItem(pItem);
	update.NotifyClient();

	XItem::PrintArtifactTLog(pRole, pItem, Tx_Artifact_Recast);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode ArtifactMgr::FuseArtifact(Role* pRole, XItem* pItem1, XItem* pItem2, bool isUsedStone)
{
	if (pRole == NULL || pItem1 == NULL || pItem2 == NULL || pItem1 == pItem2)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(XSys_Artifact_Fuse))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	const ArtifactListConf* listConf1 = ArtifactConfig::Instance()->GetArtifactListConf(pItem1->itemID);
	if (listConf1 == NULL)
	{
		LogWarn("itemid [%u] not in ArtifactList", pItem1->itemID);
		return KKSG::ERR_FAILED;
	}
	const ArtifactListConf* listConf2 = ArtifactConfig::Instance()->GetArtifactListConf(pItem2->itemID);
	if (listConf2 == NULL)
	{
		LogWarn("itemid [%u] not in ArtifactList", pItem2->itemID);
		return KKSG::ERR_FAILED;
	}

	if (listConf1->ArtifactPos != listConf2->ArtifactPos)
	{
		return KKSG::ERR_ARTIFACT_FUSE_CONDITION;
	}
	UINT32 level1 = XItem::GetLevel(pItem1->itemID), quality1 = XItem::GetQuality(pItem1->itemID);
	UINT32 level2 = XItem::GetLevel(pItem2->itemID), quality2 = XItem::GetQuality(pItem2->itemID);
	if (level1 == -1 || quality1 == -1 || level2 == -1 || quality2 == -1)
	{
		return KKSG::ERR_ARTIFACT_FUSE_CONDITION;
	}
	if (GetBelongLevel(level1) != GetBelongLevel(level2) || quality1 != quality2)
	{
		return KKSG::ERR_ARTIFACT_FUSE_CONDITION;
	}

	if (!listConf1->IsCanFuse || !listConf2->IsCanFuse)
	{
		return KKSG::ERR_ARTIFACT_CANTFUSE;
	}

	bool isFailed = false;
	if (isUsedStone)
	{
		BagTakeItemTransition take(pRole);
		take.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactFuseUseStone);
		for (auto i = listConf1->FuseMaterials.begin(); i != listConf1->FuseMaterials.end(); ++i)
		{
			if (!take.TakeItem((*i)[0], (*i)[1]))
			{
				take.RollBack();
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
		}
		take.NotifyClient();

		UINT32 percent = XRandom::randInt(1, 100 + 1);
		if (percent > listConf1->FuseSucRateUseStone)
		{
			isFailed = true;
		}
	}
	else
	{
		UINT32 percent = XRandom::randInt(1, 100 + 1);
		if (percent > listConf1->FuseSucRate[1])
		{
			isFailed = true;
		}
	}
	std::string strTransTag = TTag::Instance()->GetTagString();
	XItem::PrintArtifactTLog(pRole, pItem2, Tx_Artifact_FuseTake, strTransTag);//融合消耗;
	XItem::PrintArtifactTLog(pRole, pItem1, Tx_Artifact_Fuse, strTransTag, isFailed, isUsedStone?1:0);
	if (isFailed)
	{
		BagTakeItemTransition take(pRole);
		take.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactFuse);
		take.TakeUniqueItem(pItem2->uid, 1);
		take.NotifyClient();
		return KKSG::ERR_ARTIFACT_FUSEFAILED;
	}
	
	if (pItem1->effects.size() == pItem2->effects.size())
	{
		pItem1->effects = pItem2->effects;
	}
	else if (pItem1->effects.size() < pItem2->effects.size())
	{
		std::vector<UINT32> vec;
		_GetRandomList(pItem2->effects.size(), pItem1->effects.size(), vec);
		pItem1->effects.clear();
		for (auto i = vec.begin(); i != vec.end(); ++i)
		{
			pItem1->effects.push_back(pItem2->effects[*i]);
		}
	}
	else
	{
		std::vector<UINT32> vec;
		_GetRandomList(pItem1->effects.size(), pItem2->effects.size(), vec);
		for (UINT32 i = 0; i < vec.size(); ++i)
		{
			pItem1->effects[vec[i]] = pItem2->effects[i];
		}
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Artifact, ItemFlow_ArtifactFuse);
	take.TakeUniqueItem(pItem2->uid, 1);
	take.NotifyClient();

	EffectConfig::Instance()->UpdateEffectData(pItem1->effects);
	BagUpdateItemTransition update(pRole);
	update.AddChangedItem(pItem1);
	update.NotifyClient();

	XItem::PrintArtifactTLog(pRole, pItem1, Tx_Artifact_FuseOk, strTransTag);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode ArtifactMgr::InscriptionArtifact(Role* pRole, XItem* pItem1, XItem* pItem2)
{
	if (pRole == NULL || pItem1 == NULL || pItem2 == NULL || pItem1 == pItem2)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(XSys_Artifact_Inscription))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	const ArtifactListConf* listConf = ArtifactConfig::Instance()->GetArtifactListConf(pItem1->itemID);
	if (listConf == NULL)
	{
		LogWarn("itemid [%u] not in ArtifactList", pItem1->itemID);
		return KKSG::ERR_FAILED;
	}

	if (!listConf->IsCanInscription)
	{
		return KKSG::ERR_ARTIFACT_CANTINSCRIPTION;
	}

	UINT32 level1 = XItem::GetLevel(pItem1->itemID), level2 = XItem::GetLevel(pItem2->itemID);
	if (level1 == -1 || level2 == -1)
	{
		LogWarn("itemid [%u] or itemid [%u] not find in ItemList.txt", pItem1->itemID, pItem2->itemID);
		return KKSG::ERR_FAILED;
	}

	if (GetBelongLevel(level1) != GetBelongLevel(level2))
	{
		return KKSG::ERR_ARTIFACT_INSCRIPTION_LEVEL;
	}

	const ArtifactInscriptionConf* inscriptConf = ArtifactConfig::Instance()->GetArtifactInscriptionConf(pItem2->itemID);
	if (inscriptConf == NULL)
	{
		LogWarn("itemid [%u] not in ArtifactSkillInscription.txt", pItem2->itemID);
		return KKSG::ERR_FAILED;
	}

	KKSG::EffectData effectData;
	if (!EffectConfig::Instance()->GetEffectData(inscriptConf->EffectID, effectData))
	{
		LogWarn("itemid [%u], effectid [%u] not find", pItem2->itemID, inscriptConf->EffectID);
		return KKSG::ERR_FAILED;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Artifact, ItemFLow_ArtifactInscription);
	if (!take.TakeUniqueItem(pItem2->uid, 1))
	{
		take.RollBack();
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}
	take.NotifyClient();

	bool isAddNew = false;
	if (pItem1->effects.size() < listConf->InscriptionMaxNum)
	{
		UINT32 percent = XRandom::randInt(1, 100 + 1);
		if (percent <= listConf->InscriptionAddNewRate)
		{
			isAddNew = true;
		}
	}
	if (pItem1->effects.empty())
	{
		isAddNew = true;
	}	

	if (isAddNew)
	{
		pItem1->effects.push_back(effectData);
	}
	else
	{
		UINT32 pos = XRandom::randInt(0, pItem1->effects.size());
		pItem1->effects[pos] = effectData;
	}

	XItem::PrintArtifactTLog(pRole, pItem1, Tx_Artifact_Inscription, "", isAddNew, inscriptConf->EffectID);

	EffectConfig::Instance()->UpdateEffectData(pItem1->effects);
	BagUpdateItemTransition update(pRole);
	update.AddChangedItem(pItem1);
	update.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

UINT32 ArtifactMgr::GetBelongLevel(UINT32 level)
{
	UINT32 belongLevel = 0;
	for (auto j = GetGlobalConfig().ArtifactLevels.begin(); j != GetGlobalConfig().ArtifactLevels.end(); ++j)
	{
		if (*j > level)
		{
			break;
		}
		belongLevel = *j;
	}
	return belongLevel;
}

void ArtifactMgr::_GetRandomList(UINT32 count, UINT32 needCount, std::vector<UINT32>& vec)
{
	if (count < needCount || count == 0)
	{
		LogError("params error, count [%u] < needCount [%u] || count == 0", count, needCount);
		return;
	}
	for (UINT32 i = 0; i < count; ++i)
	{
		vec.push_back(i);
	}
	UINT32 diff = count - needCount;
	while (diff--)
	{
		if (vec.empty())
		{
			break;
		}
		UINT32 pos = XRandom::randInt(0, vec.size());
		std::vector<UINT32>::iterator iter = vec.begin();
		std::advance(iter, pos);
		vec.erase(iter);
	}
	if (vec.size() != needCount)
	{
		LogError("function %s, code error", __FUNCTION__);
		vec.clear();
	}
}

