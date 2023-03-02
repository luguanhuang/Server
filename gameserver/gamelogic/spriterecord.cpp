#include "pch.h"
#include "spriterecord.h"
#include "sprite.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "spriteconfig.h"
#include "spritetransition.h"
#include "utility/uidhelper.h"
#include "bagtransition.h"
#include "unit/combatattribute.h"
#include "unit/roleoutlook.h"
#include "buff/XBuffTemplateManager.h"
#include "scene/scene.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "config/itemdropconfig.h"
#include "notice.h"
#include "scene/sceneteam.h"
#include "tablemgr/backflowtablemgr.h"
#include "utility/tlogmgr.h"

#define CallBack(Fun, ...) do { if(listener) listener->Fun( __VA_ARGS__ ); } while(0)

const UINT32 versionNow = 1;

CSpriteRecord::CSpriteRecord(Role* pRole):m_pRole(pRole)
{
	m_newAwake = NULL;

	m_spriteSize = SpriteConfig::Instance()->GetSpriteSize();
	m_books.resize(m_spriteSize, 0);

	m_inFightSize = GetGlobalConfig().SpritePositionLevel.size();
	m_inFight.resize(m_inFightSize, 0);

	m_lastAddNewSpritePPT = 0;

	m_version = 0;
}

CSpriteRecord::~CSpriteRecord()
{
	for(auto i = m_sprites.begin(); i != m_sprites.end(); ++i)
	{
		delete i->second;
	}

	delete m_newAwake;
}

bool CSpriteRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{	
	if(false == poRoleAllInfo->has_spriterecord())
	{
		return true;
	}
	//Sprites
	const KKSG::SpriteRecord& oSpriteRecord = poRoleAllInfo->spriterecord();
	for(int i = 0; i < oSpriteRecord.spritedata_size(); ++i)
	{
		UINT64 uid = oSpriteRecord.spritedata(i).uid();
		if(m_sprites.find(uid) != m_sprites.end())
		{
			LogWarn("sprite's uid:%llu is repeated", uid);
		}
		else
		{
			m_sprites[uid] = new Sprite(m_pRole);
		}
		m_sprites[uid]->KKSGToSprite(oSpriteRecord.spritedata(i));
		m_sprites[uid]->RecalculateAttr();
	}
	//InFight 
	for(int i = 0; i < oSpriteRecord.infight_size(); ++i)
	{		
		if((UINT32)i >= m_inFightSize)
		{
			LogWarn("infight size:%d in db > size:%u in table", oSpriteRecord.infight_size(), m_inFightSize);
			break;
		}
		m_inFight[i] = oSpriteRecord.infight(i);
	}
	for(UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if(0 == m_inFight[i])
		{
			continue;
		}
		if(m_sprites.find(m_inFight[i]) == m_sprites.end())
		{
			LogError("The sprite(uid:%llu) infight in db can't find in sprites bag", m_inFight[i]);
			m_inFight[i] = 0;
		}
	}
	//books
	for(int i = 0; i < oSpriteRecord.books_size(); ++i)
	{
		if((UINT32)i >= m_spriteSize)
		{
			LogWarn("sprites size:%d in db(books) > size:%u in table", oSpriteRecord.books_size(), m_spriteSize);
			break;
		}
		if(oSpriteRecord.books(i)) m_books[i] = 1;
		else m_books[i] = 0;
	}
	//NewAwake
	/*if(oSpriteRecord.has_newawake())
	{
		m_newAwake = new Sprite(m_pRole);
		m_newAwake->KKSGToSprite(oSpriteRecord.newawake());
	}*/

	m_version = oSpriteRecord.version();	

	return true;
}

void CSpriteRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if(m_isModify.TestAndReset())
	{
		KKSG::SpriteRecord* pSpriteRecord = poRoleAllInfo->mutable_spriterecord();
		pSpriteRecord->Clear();
		//Sprites
		for(auto i = m_sprites.begin(); i != m_sprites.end(); ++i)
		{
			i->second->SpriteToKKSG(pSpriteRecord->add_spritedata());
		}
		//InFight
		for(UINT32 i = 0; i < m_inFightSize; ++i)
		{
			pSpriteRecord->add_infight(m_inFight[i]);
		}
		//books
		for(UINT32 i = 0; i < m_spriteSize; ++i)
		{
			if(0 == m_books[i])
			{
				pSpriteRecord->add_books(false);
			}
			else
			{
				pSpriteRecord->add_books(true);
			}
		}
		//NewAwake
		/*if(NULL != m_newAwake)
		{
			m_newAwake->SpriteToKKSG(pSpriteRecord->mutable_newawake());
		}*/
		pSpriteRecord->set_version(m_version);

		roChanged.insert(pSpriteRecord);
	}
}

void CSpriteRecord::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		const BackFlowSpriteConf* conf = BackFlowTableMgr::Instance()->GetSpriteConf(GSConfig::Instance()->GetBackFlowLevel(), m_pRole->GetProfession());
		if (conf != NULL)
		{
			for (auto i = conf->Sprite.begin(); i != conf->Sprite.end(); ++i)
			{
				UINT32 spriteid = (*i)[0], spritelevel = (*i)[1];
				const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(spriteid);
				if (pSpriteConf == NULL)
				{
					LogWarn("BackFlowSprite, spriteid [%u] is not exist in SpriteTable.txt", spriteid);
					continue;
				}
				const SpriteLevelInfo* pLevelInfo = SpriteConfig::Instance()->GetSpriteLevelInfo(pSpriteConf->SpriteQuality, spritelevel);	
				if (pLevelInfo == NULL)
				{
					LogWarn("BackFlowSprite, spriteid [%u], quality [%u], level [%u], can't find in SpriteLevel.txt", pSpriteConf->SpriteID, pSpriteConf->SpriteQuality, spritelevel);
					return;
				}
				UINT64 uid = UIDHelper::CreateUID(UID_Common);
				Sprite* pSprite = new Sprite(uid, spriteid, m_pRole);
				pSprite->SetLevel(spritelevel);
				pSprite->RecalculateAttr();
				m_sprites.insert(std::make_pair(uid, pSprite));
				KKSG::ErrorCode code = SpriteInFight(uid);
				if (code != KKSG::ERR_SUCCESS)
				{
					LogWarn("BackFlowFirstInitSprite, Sprite [%u] in fight failed, ErrorCode [%d]", spriteid, code);
					continue;
				}
			}
		}
	}
}

void CSpriteRecord::OnEnterScene(Scene* scene)
{
	if (SceneConfig::Instance()->IsBattleSceneExcept(scene->GetSceneType()))
	{
		return;
	}

	Sprite* pSprite = GetInFightSprite(0);
	if (pSprite == NULL)
	{
		return;
	}
	const SpriteSkillInfo* pSpriteSkillInfo = SpriteConfig::Instance()->GetSpriteSkillInfo(pSprite->GetSkillID(), pSprite->GetEvolutionLevel());
	if (pSpriteSkillInfo == NULL)
	{
		return;
	}
	const std::vector<Sequence<UINT32,2>>& buffs = pSpriteSkillInfo->buff;
	XAddBuffActionArgs args;
	for (auto i = buffs.begin(); i != buffs.end(); ++i)
	{
		BuffDesc buff;
		buff.buffID = (*i)[0];
		buff.BuffLevel = (*i)[1];

		args.vecBuffs.push_back(buff);
		//args.BuffIDs.push_back((*i)[0]);
		//args.BuffLevels.push_back((*i)[1]);

		m_pRole->AddReviveBuff(buff);
	}
	args.Caster = m_pRole->GetID();
	args.bClientOnly = !scene->IsSyncScene();
	m_pRole->GetXObject()->DispatchAction(&args);
}

void CSpriteRecord::OnFirstTimeEnterScene()
{
	if (m_version > versionNow)
	{
		m_version = versionNow;
		m_isModify.Set();
	}
	
	_UpdateVersion();
}

void CSpriteRecord::ApplyEffect(CombatAttribute* combatAttr)
{
	for(UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if(0 == m_inFight[i])
		{
			continue;
		}
		if(m_sprites.find(m_inFight[i]) == m_sprites.end())
		{
			LogError("uid in infight does not in sprites map");
			continue;
		}
		m_sprites[m_inFight[i]]->ApplyEffect(combatAttr);
	}
}

void CSpriteRecord::AddSprite(UINT32 spriteid, SpriteTransition* listener)
{
	UINT64 uid = UIDHelper::CreateUID(UID_Common);
	Sprite* pSprite = new Sprite(uid, spriteid, m_pRole);

	m_sprites.insert(std::make_pair(uid, pSprite));

	UINT32 index = SpriteConfig::Instance()->GetSpriteIndex(spriteid);
	if (index != (UINT32)(-1))
	{
		m_books[index] = 1;
	}

	CallBack(OnGiveSprite, pSprite);
	m_isModify.Set();

	m_lastAddNewSpritePPT = pSprite->GetPowerPoint();
}

void CSpriteRecord::ChangeSprite(Sprite* pSprite, SpriteTransition* listener)
{
	if (pSprite == NULL)
	{
		LogWarn("change sprite pointer is NULL");
		return;
	}
	if (m_sprites.find(pSprite->Getuid()) == m_sprites.end())
	{
		return;	
	}

	UINT32 pos = (UINT32)(-1);
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if (pSprite->Getuid() == m_inFight[i])
		{
			pos = i;
			break;
		}
	}
	if (pos != (UINT32)(-1))
	{
		m_pRole->OnChangeSpriteInFight(pos);
		UpdateMSRankList();
	}

	CallBack(OnChangeSprite, pSprite);
	m_isModify.Set();
}

void CSpriteRecord::RemoveSprite(UINT64 uid, SpriteTransition* listener)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return;
	}

	CallBack(OnRemoveSprite, uid);
	delete m_sprites[uid];
	m_sprites.erase(uid);
	m_isModify.Set();
}

KKSG::ErrorCode CSpriteRecord::SpriteAddExp(UINT64 uid, UINT32 itemID)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	KKSG::ErrorCode code = m_sprites[uid]->AddExp(itemID);
	if (code == KKSG::ERR_SUCCESS)
	{
		m_isModify.Set();
	}
	return code;
}

KKSG::ErrorCode CSpriteRecord::SpriteTrain(UINT64 uid, const std::vector<UINT32>& notToChoose, std::vector<std::pair<UINT32, UINT32>>& addAttr)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}
	
	KKSG::ErrorCode code = m_sprites[uid]->Train(notToChoose, addAttr);
	if (code == KKSG::ERR_SUCCESS)
	{
		m_isModify.Set();
	}
	return code;
}

KKSG::ErrorCode CSpriteRecord::SpriteResetTrain(UINT64 uid, UINT32 chooseid)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	KKSG::ErrorCode code = m_sprites[uid]->ResetTrain(chooseid);
	if (code == KKSG::ERR_SUCCESS)
	{
		m_isModify.Set();
	}
	return code;
}

KKSG::ErrorCode CSpriteRecord::SpriteRebirth(UINT64 uid)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	KKSG::ErrorCode code = m_sprites[uid]->Rebirth();
	if (code == KKSG::ERR_SUCCESS)
	{
		m_isModify.Set();
	}
	return code;
}

KKSG::ErrorCode CSpriteRecord::SpriteEvolutionLevelUp(UINT64 uid)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	m_isModify.Set();
	return m_sprites[uid]->EvolutionLevelUp();
}

KKSG::ErrorCode CSpriteRecord::SpriteAwake(UINT64 uid)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}
	/*if (m_newAwake != NULL)
	{
		return KKSG::ERR_UNKNOWN; //已经有精灵觉醒，还未选择
	}*/
	if (m_newAwake != NULL)
	{
		delete m_newAwake;
		m_newAwake = NULL;
	}

	if ((UINT32)m_pRole->GetLevel() < GetGlobalConfig().SpriteAwakeMinLevel)
	{
		return KKSG::ERR_SPRITE_AWAKE_ROLE_LEVELLIMIT; //人物等级不满足
	}

	Sprite* pSprite = m_sprites[uid];
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(pSprite->GetSpriteID());
	if (pSpriteConf == NULL)
	{
		LogWarn("spriteid:%u is not exist in SpriteTable.txt", pSprite->GetSpriteID());
		return KKSG::ERR_UNKNOWN;
	}
	KKSG::ErrorCode code = pSprite->CheckAwake();
	if (code != KKSG::ERR_SUCCESS)
	{
		return code;
	}

	UINT32 pos = pSpriteConf->SpriteQuality;
	if (GetGlobalConfig().SpriteAwakeConsume.size() <= pos)
	{
		pos = GetGlobalConfig().SpriteAwakeConsume.size() - 1;
	}

	BagTakeItemTransition oTakeItemTransition(m_pRole);
	oTakeItemTransition.SetReason(ItemFlow_Sprite, ItemFlow_SpriteAwake);
	std::string strbill = TTag::Instance()->GetTagString();
	oTakeItemTransition.SetTransTag(strbill);
	if (!oTakeItemTransition.TakeItem(GetGlobalConfig().SpriteAwakeItemID, GetGlobalConfig().SpriteAwakeConsume[pos]))
	{
		oTakeItemTransition.RollBack();
		return KKSG::ERR_SPRITE_AWAKE_LACKOFCOST;
	}
	oTakeItemTransition.NotifyClient();

	m_newAwake = new Sprite(*pSprite);
	m_newAwake->AwakeRandom();

	if (pSprite->GetPowerPoint() < m_newAwake->GetPowerPoint())
	{
		// replace
		*pSprite = *m_newAwake;
		pSprite->NotifyChangeSprite();

		if (IsInfight(uid))
		{
			m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
		}
	}

	//TLog
	SpriteTLogVar var;	
	var.m_iOperType = TX_Sprite_Awake;
	m_sprites[uid]->PrintTLog(var, strbill);

	m_isModify.Set();
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CSpriteRecord::GetEvolutionLevelUpPPT(UINT64 uid, UINT32& ppt)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	return m_sprites[uid]->GetEvolutionLevelUpPPT(ppt);
}

Sprite* CSpriteRecord::GetSprite(UINT64 uid)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return NULL;
	}
	return m_sprites[uid];
}

Sprite* CSpriteRecord::GetInFightSprite(UINT32 pos)
{
	if (pos >= m_inFightSize)
	{
		LogWarn("pos:%u >= inFightSize:%u", pos, m_inFightSize);
		return NULL;
	}
	if (0 == m_inFight[pos])
	{
		return NULL;
	}
	if (m_sprites.find(m_inFight[pos]) == m_sprites.end())
	{
		LogError("infight uid:%llu can't find in sprites", m_inFight[pos]);
		return NULL;
	}
	return m_sprites[m_inFight[pos]];
}

void CSpriteRecord::SpriteAwakeRetain()
{
	delete m_newAwake;
	m_newAwake = NULL;
	m_isModify.Set();
}

KKSG::ErrorCode CSpriteRecord::SpriteAwakeReplace()
{
	if (m_newAwake == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 uid = m_newAwake->Getuid();
	if (m_sprites.find(uid) == m_sprites.end())
	{
		LogWarn("strange, origin sprite uid:%llu disappeared", uid);
		delete m_newAwake;
		m_newAwake = NULL;
		m_isModify.Set();
		return KKSG::ERR_FAILED;
	}

	delete m_sprites[uid];
	m_sprites[uid] = m_newAwake;
	m_sprites[uid]->NotifyChangeSprite();

	if (IsInfight(uid))
	{
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	}

	m_newAwake = NULL;

	m_isModify.Set();

	//TLog
	SpriteTLogVar var;	
	var.m_iOperType = TX_Sprite_AwakeReplace;
	m_sprites[uid]->PrintTLog(var);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CSpriteRecord::SpriteInFight(UINT64 uid)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	UINT32 pos = (UINT32)(-1);
	//bool isSameID = false;
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if (m_inFight[i] == uid)
		{
			pos = i;
			break;
		}
		/*if(m_inFight[i] != 0 && m_sprites[m_inFight[i]]->GetSpriteID() == m_sprites[uid]->GetSpriteID())
		{
			isSameID = true;
		}*/
	}
	if (pos != (UINT32)(-1))
	{
		return KKSG::ERR_SPRITE_ALREADY_INFIGHT;
	}
	/*if(isSameID)
	{
		return KKSG::ERR_SPRITE_INFIGHT_SAMETYPE;
	}*/

	pos = (UINT32)(-1);
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if (m_pRole->GetLevel() < (int)GetGlobalConfig().SpritePositionLevel[i])
		{
			break;
		}
		if (m_inFight[i] == 0)
		{
			m_inFight[i] = uid;
			pos = i;
			break;
		}
	}
	if ((UINT32)(-1) == pos)
	{
		return KKSG::ERR_SPRITE_INFIGHT_FULL;
	}	

	m_pRole->OnChangeSpriteInFight(pos);
	UpdateMSRankList();
	if (0 == pos)
	{
		RoleOutLook outlook(m_pRole);
		outlook.Fill(SUMMARY_SPRITELEADER, NULL);
		outlook.BroadCast();
	}

	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	m_isModify.Set();

	//TLog
	SpriteTLogVar var;	
	var.m_iOperType = TX_Sprite_InFight;
	var.m_iChangeValue = pos;
	m_sprites[uid]->PrintTLog(var);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CSpriteRecord::SpriteOutFight(UINT64 uid)
{
	if (0 == uid)
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	UINT32 pos = (UINT32)(-1);
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if (m_inFight[i] == uid)
		{
			pos = i;
			break;
		}
	}
	if ((UINT32)(-1) == pos)
	{
		return KKSG::ERR_SPRITE_ALREADY_OUTFIGHT;
	}

	m_inFight[pos] = 0;	

	m_pRole->OnChangeSpriteInFight(pos);
	UpdateMSRankList();
	if (0 == pos)
	{
		RoleOutLook outlook(m_pRole);
		outlook.Fill(SUMMARY_SPRITELEADER, NULL);
		outlook.BroadCast();
	}

	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	m_isModify.Set();

	//TLog
	SpriteTLogVar var;	
	var.m_iOperType = TX_Sprite_OutFight;
	var.m_iChangeValue = pos;
	m_sprites[uid]->PrintTLog(var);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CSpriteRecord::SpriteSwapLeader(UINT64 uid)
{
	if (m_sprites.find(uid) == m_sprites.end())
	{
		return KKSG::ERR_SPRITE_NOTFIND;
	}

	UINT32 pos = (UINT32)(-1);
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if (m_inFight[i] == uid)
		{
			pos = i;
			break;
		}
	}
	if ((UINT32)(-1) == pos)
	{
		return KKSG::ERR_SPRITE_ALREADY_OUTFIGHT;
	}
	if (0 == pos)
	{
		return KKSG::ERR_SPRITE_ALREADY_ISLEADER;
	}
	m_inFight[pos] = m_inFight[0];
	m_inFight[0] = uid;

	m_pRole->OnChangeSpriteInFight(pos);
	m_pRole->OnChangeSpriteInFight(0);

	UpdateMSRankList();
	RoleOutLook outlook(m_pRole);
	outlook.Fill(SUMMARY_SPRITELEADER, NULL);
	outlook.BroadCast();

	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Sprite);
	m_isModify.Set();

	SpriteTLogVar var;	
	var.m_iOperType = TX_Sprite_SetLeader;
	var.m_iChangeValue = 0;
	m_sprites[uid]->PrintTLog(var);

	if (m_inFight[pos] != 0)
	{
		SpriteTLogVar var;
		var.m_iOperType = TX_Sprite_CancelLeader;
		var.m_iChangeValue = pos;
		m_sprites[m_inFight[pos]]->PrintTLog(var);
	}

	return KKSG::ERR_SUCCESS;
}

bool CSpriteRecord::IsInfight(UINT64 uid)
{
	if (0 == uid)
	{
		return false;
	}
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		if (m_inFight[i] == uid)
		{
			return true;
		}
	}
	return false;
}

void CSpriteRecord::GetInFightList(std::vector<UINT64>& vecFight) const
{
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		vecFight.push_back(m_inFight[i]);
	}
}

KKSG::ErrorCode CSpriteRecord::SpritesDecompose(std::vector<UINT64>& sprites)
{
	KKSG::ErrorCode ret = KKSG::ERR_SUCCESS;

	BagGiveItemTransition oGiveItemTransition(m_pRole);
	oGiveItemTransition.SetReason(ItemFlow_Sprite, ItemFlow_SpriteDecompose);

	SpriteTransition oSpriteTransition(m_pRole);

	std::map<UINT32, UINT32> backItem;

	UINT32 num =  GetGlobalConfig().GetInt("SpriteResolveMaxNum", 12);
	if (num > sprites.size())
	{
		num = sprites.size();
	}
	for (UINT32 i = 0; i < num; ++i)
	{
		if (m_sprites.find(sprites[i]) == m_sprites.end())
		{
			LogWarn("Can't find this sprite, uid [%llu]", sprites[i]);
			ret = KKSG::ERR_UNKNOWN;
			continue;
		}
		if (IsInfight(sprites[i]))
		{
			continue;
		}
		const Sprite* pSprite = m_sprites[sprites[i]];
		const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(pSprite->GetSpriteID());
		if (pSpriteConf == NULL)
		{
			LogWarn("Can't find this sprite in table, spriteid [%u]", pSprite->GetSpriteID());
			ret = KKSG::ERR_UNKNOWN;
			continue;
		}
		
		SpriteConfig::Instance()->GetAllEvolutionCost(pSpriteConf->SpriteID, pSprite->GetEvolutionLevel(), pSprite->GetTrainExp(), backItem);
		SpriteConfig::Instance()->GetAllExpCost(pSpriteConf->SpriteQuality, pSprite->GetLevel(), pSprite->GetExp(), backItem);

		for (UINT32 j = 0; j < pSpriteConf->DropID.size(); ++j)
		{
			const ItemDesc* pDropConf = ItemDropConfig::Instance()->GetRandomDrop(pSpriteConf->DropID[j]);
			if (pDropConf == NULL)
			{
				continue;
			}
			oGiveItemTransition.GiveItem(*pDropConf);
		}

		oSpriteTransition.RemoveSprite(sprites[i]);
	}
	oSpriteTransition.NotifyClient();

	for (auto i = backItem.begin(); i != backItem.end(); ++i)
	{
		oGiveItemTransition.GiveItem(i->first, i->second);
	}
	oGiveItemTransition.NotifyClient();
	
	m_isModify.Set();
	return ret;
}

void CSpriteRecord::UpdateMSRankList()
{
	UINT32 newppt = 0;
	for (UINT32 i = 0; i < m_inFightSize; ++i)
	{
		Sprite* pSprite = GetInFightSprite(i);
		if (pSprite == NULL)
		{
			continue;
		}
		newppt += pSprite->GetPowerPoint();
	}

	RpcG2M_UpdateMSRankList* rpc = RpcG2M_UpdateMSRankList::CreateRpc();
	rpc->m_oArg.set_ranktype(KKSG::SpritePowerPointRank);
	KKSG::RankData* data = rpc->m_oArg.mutable_info();
	data->set_roleid(m_pRole->GetID());
	data->set_powerpoint(newppt);
	data->set_time((UINT32)time(NULL));

	m_pRole->SendToMS(*rpc);
}

void CSpriteRecord::OnSkillCast()
{
	Sprite* pSprite = GetInFightSprite(0);
	if (pSprite == NULL)
	{
		return;
	}
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(pSprite->GetSpriteID());
	if (pSpriteConf == NULL)
	{
		return;
	}
	const SpriteSkillInfo* pSpriteSkillInfo = SpriteConfig::Instance()->GetSpriteSkillInfo(pSprite->GetSkillID(), pSprite->GetEvolutionLevel());
	if (pSpriteSkillInfo == NULL || !pSpriteSkillInfo->bShowNotice)
	{
		return;
	}

	UINT32 noticeID = SpriteSkillCastSolo; 
	Scene* pScene = m_pRole->GetCurrScene();
	if(pScene == NULL)
		return;

	bool bInTeam = false;
	SceneTeam* pSceneTeam = pScene->GetSceneTeam();
	if(pSceneTeam != NULL)
	{
		if(pSceneTeam->FindMember(m_pRole->GetID()))
			bInTeam = true;
	}

	if(bInTeam)
		noticeID = SpriteSkillCastTeam;
	Notice notice(noticeID);
	if(bInTeam)
		notice.Add(m_pRole);
	notice.Replace("$C", pSpriteConf->SpriteName);
	notice.Replace("$C", pSpriteSkillInfo->skillName);
	notice.Replace("$C", pSpriteSkillInfo->noticeDetail);
	if(bInTeam)
	{
		const auto& roles = pScene->GetAllRoles();
		for(auto it = roles.begin(); it != roles.end(); ++it)
		{
			notice.SendSelf((*it));
		}
		//notice.SendTeam(pSceneTeam->GetTeamID());
	}
	else
		notice.SendSelf(m_pRole);
}

void CSpriteRecord::SetSummarySprite(std::vector<KKSG::SpriteInfo>& oSprites, void* arg)
{
	if (arg != NULL)
	{
		UINT32 pos = *(UINT32*)arg;
		if (pos < oSprites.size())
		{
			Sprite* pSprite = GetInFightSprite(pos);
			oSprites[pos].Clear();
			if (pSprite == NULL)
			{
				oSprites[pos].set_spriteid(0);
			}
			else
			{
				pSprite->SpriteToKKSG(&oSprites[pos]);
			}
		}
		else 
		{
			LogWarn("update sprite infight pos%u >= %u", pos, oSprites.size());
		}
	}
	else
	{
		for (UINT32 i = 0; i < oSprites.size(); ++i)
		{
			Sprite* pSprite = GetInFightSprite(i);
			oSprites[i].Clear();
			if (pSprite == NULL)
			{
				oSprites[i].set_spriteid(0);
			}
			else
			{
				pSprite->SpriteToKKSG(&oSprites[i]);
			}
		}
	}
}

void CSpriteRecord::OnChangeProfession()
{
	for (auto i = m_sprites.begin(); i != m_sprites.end(); ++i)
	{
		i->second->RecalculateAttr();
	}
}

void CSpriteRecord::GetAllList(std::vector<Sprite*>& sprites)
{
	for (auto i = m_sprites.begin(); i != m_sprites.end(); ++i)
	{
		sprites.push_back(i->second);
	}
}

bool CSpriteRecord::IsLeader(UINT64 uid)
{
	if (m_inFightSize && m_inFight[0] == uid)
	{
		return true;
	}
	return false;
}

void CSpriteRecord::_UpdateVersion()
{
	if (m_version >= versionNow)
	{
		return;
	}
	while (m_version < versionNow)
	{
		++m_version;
		switch (m_version)
		{
		case 1:
			{
				_UpdateVersion1();
				break;
			}
		default:
			{
				break;
			}
		}	
	}
	m_isModify.Set();
}

void CSpriteRecord::_UpdateVersion1()
{
	if (m_version != 1)
	{
		return;
	}
	for (auto i = m_sprites.begin(); i != m_sprites.end(); ++i)
	{
		Sprite* pSprite = i->second;
		if (pSprite->GetSpriteID() == 3301)
		{
			pSprite->ResetTrain(0, false);
			pSprite->Rebirth(false);
		}
	}
}
