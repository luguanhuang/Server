#ifndef __CSpriteRecord_H__
#define __CSpriteRecord_H__

#include "unit/roleserilizeinterface.h"

namespace KKSG
{
	class SpriteInfo;
};

class Role;
class Sprite;
class SpriteTransition;
class CombatAttribute;
class Scene;

class CSpriteRecord : public RoleSerializeInterface
{
public:
	CSpriteRecord(Role* pRole);
	~CSpriteRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);
	
	inline void SetModify() { m_isModify.Set(); }

	void OnEnterScene(Scene* scene);
	void OnFirstTimeEnterScene();

	void ApplyEffect(CombatAttribute* combatAttr);

	void AddSprite(UINT32 spriteid, SpriteTransition* listener);
	void ChangeSprite(Sprite* pSprite, SpriteTransition* listener);
	void RemoveSprite(UINT64 uid, SpriteTransition* listener);
	
	KKSG::ErrorCode SpriteAddExp(UINT64 uid, UINT32 itemID);
	KKSG::ErrorCode SpriteTrain(UINT64 uid, const std::vector<UINT32>& notToChoose, std::vector<std::pair<UINT32, UINT32>>& addAttr);
	KKSG::ErrorCode SpriteResetTrain(UINT64 uid, UINT32 chooseid);
	KKSG::ErrorCode SpriteRebirth(UINT64 uid);
	KKSG::ErrorCode SpriteEvolutionLevelUp(UINT64 uid);
	KKSG::ErrorCode SpriteAwake(UINT64 uid);
	KKSG::ErrorCode GetEvolutionLevelUpPPT(UINT64 uid, UINT32& ppt); //得到升星后战力

	Sprite* GetSprite(UINT64 uid);
	Sprite* GetInFightSprite(UINT32 pos);
	Sprite* GetNewAwake() const { return m_newAwake; }

	void SpriteAwakeRetain();
	KKSG::ErrorCode SpriteAwakeReplace();

	KKSG::ErrorCode SpriteInFight(UINT64 uid);
	KKSG::ErrorCode SpriteOutFight(UINT64 uid);
	KKSG::ErrorCode SpriteSwapLeader(UINT64 uid);

	bool IsInfight(UINT64 uid);
	void GetInFightList(std::vector<UINT64>& vecFight) const;

	KKSG::ErrorCode SpritesDecompose(std::vector<UINT64>& sprites);

	void UpdateMSRankList();
	void OnSkillCast();

	inline UINT32 GetLastAddNewSpritePPT() { return m_lastAddNewSpritePPT; } //just for sprite lottery

	void SetSummarySprite(std::vector<KKSG::SpriteInfo>& oSprites, void* arg);

	inline UINT32 GetSpriteNum() { return m_sprites.size(); }

	///> 变职业
	void OnChangeProfession();

	void GetAllList(std::vector<Sprite*>& sprites);
	bool IsLeader(UINT64 uid);

private:
	void _UpdateVersion();
	void _UpdateVersion1();

private:
	Role* m_pRole;

	UINT32 m_spriteSize;					// num of sprites
	std::vector<char> m_books;				// sprite handbooks

	UINT32 m_inFightSize;					// size of infight
	std::vector<UINT64> m_inFight;

	std::map<UINT64, Sprite*> m_sprites;
	Sprite* m_newAwake;

	Switch m_isModify;

	UINT32 m_lastAddNewSpritePPT;			// no need to save to db

	UINT32 m_version;
};


#endif