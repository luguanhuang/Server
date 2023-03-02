#ifndef __SPRITECHANGEHELPER_H__
#define __SPRITECHANGEHELPER_H__

#include "sprite/ptcg2c_spritechangedntf.h"

class Role;
class Sprite;
class CSpriteRecord;

class SpriteTransition
{
public:
	SpriteTransition(Role* pRole);
	~SpriteTransition();

	void SetReason(INT32 nReason, INT32 nSubReason = -1, bool bIsCheckZeroProfit = true);

	void OnGiveSprite(Sprite* pSprite);
	void OnChangeSprite(Sprite* pSprite);
	void OnRemoveSprite(UINT64 uid);

	void GiveSprite(UINT32 spriteid);
	void ChangeSprite(Sprite* pSprite);
	void RemoveSprite(UINT64 uid);

	void NotifyClient();

private:
	Role* m_pRole;
	CSpriteRecord* m_pSpriteRecord;
	PtcG2C_SpriteChangedNtf m_oPtc;

	INT32	m_nReason;			//一级原因
	INT32	m_nSubReason;		//二级原因
	bool	m_bIsCheckZeroProfit;//true：需要零收益检查  false：不需要检查(默认是true需要检查，目前只有商城龙币钻石购买和充值不需要检查)
};

#endif