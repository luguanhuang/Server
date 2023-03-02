#include "pch.h"
#include "titlerecord.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "titleconfig.h"
#include "bagtransition.h"
#include "title/ptcg2c_titlechangenotify.h"
#include "rolesummary.h"
#include "title/ptcg2m_getmaxtitlentf.h"
#include "notice.h"
#include "rolesummarymgr.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"

TitleRecord::TitleRecord(Role* pRole):m_pRole(pRole)
{
	m_titleID = 0;
}

TitleRecord::~TitleRecord()
{
	
}

void TitleRecord::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(m_pRole->GetProfession(), GSConfig::Instance()->GetBackFlowLevel());
		if (pData)
		{
			const TitleConf* titleData = TitleConfig::Instance()->GetTitleTableData(pData->Title, m_pRole->GetProfession());
			if (titleData)
			{
				m_titleID = pData->Title;
				m_isModify.Set();
			}
		}
	}
}

bool TitleRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->brief().has_titleid())
	{
		m_titleID = poRoleAllInfo->brief().titleid();
	}
	return true;
}

void TitleRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_isModify.TestAndReset())
	{
		poRoleAllInfo->mutable_brief()->set_titleid(m_titleID);

		roChanged.insert(poRoleAllInfo->mutable_brief());
	}
}

UINT32 TitleRecord::GetTitle()
{
	return m_titleID;
}

KKSG::ErrorCode TitleRecord::TitleLevelUp()
{
	const TitleConf* titleData = TitleConfig::Instance()->GetTitleTableData(m_titleID+1, m_pRole->GetProfession());
	if(NULL == titleData)
	{
		return KKSG::ERR_TITLE_MAX;
	}

	CombatAttribute* combatAttr = m_pRole->GetAttrPtr();
	if(titleData->NeedPowerPoint > combatAttr->GetAttr(BASIC_POWERPOINT))
	{
		return KKSG::ERR_TITLE_PPTLIMIT;
	}

	BagTakeItemTransition stTaketransition(m_pRole);
	stTaketransition.SetReason(ItemFlow_Title, ItemFlow_TitleLevelUp);
	for (auto i = titleData->NeedItem.begin(); i != titleData->NeedItem.end(); i++)
	{
		if (!stTaketransition.TakeItem((*i)[0], (*i)[1]))
		{
			stTaketransition.RollBack();
			return KKSG::ERR_TITLE_LACKITEM;
		}
	}
	stTaketransition.NotifyClient();

	m_titleID++;

	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Title);
	m_pRole->OnTitleLevelUp();

	TitleLevelUpNotice();

	m_isModify.Set();
	return KKSG::ERR_SUCCESS;
}

void TitleRecord::ApplyEffect(CombatAttribute* combatAttr)
{
	const TitleConf* titleData = TitleConfig::Instance()->GetTitleTableData(m_titleID, m_pRole->GetProfession());
	if(NULL == titleData)
	{
		return;
	}
	for (auto i = titleData->Attribute.begin(); i != titleData->Attribute.end(); i++)
	{
		combatAttr->AddAttr((CombatAttrDef)(*i)[0], (*i)[1]);
	}
}

void TitleRecord::TitleLevelUpNotice()
{
	const TitleConf* titleData = TitleConfig::Instance()->GetTitleTableData(m_titleID, m_pRole->GetProfession());
	Notice notice(TitleGuildNotice);
	notice.Add(m_pRole);
	notice.Replace("$C", titleData->RankName);
	notice.SendGuild(m_pRole->getGuildId());

	if(m_titleID == TitleConfig::Instance()->GetMaxTitle())
	{
		PtcG2M_GetMaxTitleNtf oPtc;
		oPtc.m_Data.set_uid(m_pRole->GetID());
		oPtc.m_Data.set_titlemaxlevelnow(m_titleID);
		oPtc.m_Data.set_titlename(titleData->RankName);
		m_pRole->SendToMS(oPtc);
	}
}

void TitleRecord::DebugSetTitle(UINT32 title)
{
	m_titleID = title;
	m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Title);
	m_pRole->OnTitleLevelUp();

	TitleLevelUpNotice();

	PtcG2C_TitleChangeNotify titleNotify;
	titleNotify.m_Data.set_titleid(m_titleID);
	m_pRole->Send(titleNotify);

	m_isModify.Set();
}
