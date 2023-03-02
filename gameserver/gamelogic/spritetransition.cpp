#include "pch.h"
#include "spritetransition.h"
#include "sprite.h"
#include "spriteconfig.h"
#include "unit/role.h"
#include "spriterecord.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "idip/idiprecord.h"

SpriteTransition::SpriteTransition(Role* pRole):m_pRole(pRole)
{
	m_pSpriteRecord = m_pRole->Get<CSpriteRecord>();
}

SpriteTransition::~SpriteTransition()
{

}

void SpriteTransition::SetReason(INT32 nReason, INT32 nSubReason, bool bIsCheckZeroProfit)
{
	m_nReason = nReason;
	m_nSubReason = nSubReason;
	m_bIsCheckZeroProfit = bIsCheckZeroProfit;
}

void SpriteTransition::OnGiveSprite(Sprite* pSprite)
{
	if (m_bIsCheckZeroProfit && m_pRole->Get<CIdipRecord>()->CheckZeroProFit())
	{
		SSWarn << "roleid:"<<m_pRole->GetID() <<" is zero profit now, can't give Sprite="<<pSprite->GetSpriteID() <<END;
		return ;
	}
	pSprite->SpriteToKKSG(m_oPtc.m_Data.add_newsprites());

	SpriteTLogVar var;
	var.m_iOperType = TX_Sprite_Get;
	var.m_iReason = m_nReason;
	var.m_iSubReason = m_nSubReason;
	pSprite->PrintTLog(var);
}

void SpriteTransition::OnChangeSprite(Sprite* pSprite)
{
	pSprite->SpriteToKKSG(m_oPtc.m_Data.add_changedsprites());
}

void SpriteTransition::OnRemoveSprite(UINT64 uid)
{
	m_oPtc.m_Data.add_removedsprites(uid);

	Sprite* pSprite = m_pSpriteRecord->GetSprite(uid);
	assert(NULL != pSprite);

	SpriteTLogVar var;
	var.m_iOperType = TX_Sprite_Take;
	pSprite->PrintTLog(var);
}

void SpriteTransition::GiveSprite(UINT32 spriteid)
{
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(spriteid);
	if (pSpriteConf == NULL)
	{
		LogWarn("spriteid [%u] is not exist in SpriteTable.txt", spriteid);
		return;
	}
	m_pSpriteRecord->AddSprite(spriteid, this);
}

void SpriteTransition::ChangeSprite(Sprite* pSprite)
{
	if(NULL == pSprite)
	{
		return;
	}
	m_pSpriteRecord->ChangeSprite(pSprite, this);
}

void SpriteTransition::RemoveSprite(UINT64 uid)
{
	m_pSpriteRecord->RemoveSprite(uid, this);
}

void SpriteTransition::NotifyClient()
{
	if(m_oPtc.m_Data.newsprites_size() != 0 || m_oPtc.m_Data.changedsprites_size() != 0 || m_oPtc.m_Data.removedsprites_size() != 0)
	{
		m_pRole->Send(m_oPtc);
	}
}