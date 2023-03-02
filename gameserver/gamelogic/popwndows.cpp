#include "pch.h"
#include "unit/role.h"
#include "rolemisc.h"
#include "globalconfig.h"
#include "popwindows.h"
#include "util/gametime.h"
#include "pandora/ptcg2c_pushpraisentf.h"
#include "define/systemiddef.h"

INSTANCE_SINGLETON(PopWindows);

bool PopWindows::Init()
{
	if (!CheckFile())
		return false;
	if (!m_table.LoadFile("table/ShareTable.txt"))
	{
		return false;
	}
	return true;
}


void PopWindows::Uninit()
{
	m_table.Clear();
}


bool PopWindows::CheckFile()
{
	return true;
}


void PopWindows::CheckComment(Role *pRole,KKSG::CommentType type,int nParam1,int nParam2)
{
	ShareTable::RowData *pRowData =m_table.GetByCondition(type);
	if (!pRowData)
	{
		return;
	}
	auto* pRoleSys = pRole->Get<CRoleSystem>();
	if (pRoleSys->IsSystemOpened(SYS_LINK_SHARE))
	{
		if (type == KKSG::COMMENT_DRAGON||type == KKSG::COMMENT_NEST)
		{
			for (UINT32 i = 0; i < pRowData->param.size();i++)
			{
				if (pRowData->param[i] == nParam1)
				{
					SyncCommon(pRole,type,i);
					break;
				}
			}
		}else if (type == KKSG::COMMENT_LADDER&&pRowData->param.size()==1&&nParam1 >=pRowData->param[0])
		{
			SyncCommon(pRole,type,0);
		}else if (type == KKSG::COMMENT_ATLAS&&pRowData->param.size()==2 && nParam1 == pRowData->param[0] && nParam2 >= pRowData->param[1])
		{
			SyncCommon(pRole,type,0);
		}else if (type == KKSG::COMMENT_SPRITE&&pRowData->param.size()==1&&nParam1 >=pRowData->param[0])
		{
			PtcG2C_PushPraiseNtf ntf;
			ntf.m_Data.set_type(type);
			ntf.m_Data.set_spriteid(nParam2);
			pRole->Send(ntf);
		}

	}
	if(pRoleSys->IsSystemOpened(SYS_APPSTORE_PRAISE))
	{
		if (type == KKSG::COMMENT_PANDORA)
		{
			PopPandora(pRole,type,nParam1);
		}
	}
}

void PopWindows::ClearWindows(Role *pRole)
{
	RoleMisc* record = pRole->Get<RoleMisc>();
	record->SetLastPush(0);
	record->SetSaveFlag();
}

void PopWindows::PopPandora(Role *pRole,KKSG::CommentType type,UINT32 nItemID)
{
	RoleMisc* record = pRole->Get<RoleMisc>();
	UINT32 nLastTime = record->GetLastTime();
	if (GameTime::GetTime()>=nLastTime+GetGlobalConfig().PandoraPushDay*24*3600)
	{
		record->SetLastTime((UINT32)GameTime::GetTime());
		record->SetSaveFlag();
		PtcG2C_PushPraiseNtf ntf;
		ntf.m_Data.set_type(type);
		ntf.m_Data.set_spriteid(nItemID);
		pRole->Send(ntf);
	}
}

void PopWindows::SyncCommon(Role *pRole,KKSG::CommentType type,UINT32 nIndex)
{
	UINT32 nPos = 0;
	switch (type)
	{
	case KKSG::COMMENT_NEST:
			{
				nPos = 1+nIndex;
			}
			break;
		case KKSG::COMMENT_DRAGON:
			{
				nPos = 11+nIndex;	
			}
			break;
		case KKSG::COMMENT_LADDER:
			{
				nPos = 15+nIndex;	
			}
			break;
		case KKSG::COMMENT_ATLAS:
			{
				nPos = 16+nIndex;	
			}
			break;
		default:
			{
				break;
			}
	}
	RoleMisc* record = pRole->Get<RoleMisc>();
	UINT32 nLastPush = record->GetLastPush();
	if ((nLastPush&(1<<nPos))==0)
	{
		LogDebug("---SyncCommon------nLastPush=%d",nLastPush);
		nLastPush |= (1<<nPos);
		record->SetLastPush(nLastPush);
		record->SetSaveFlag();
		PtcG2C_PushPraiseNtf ntf;
		ntf.m_Data.set_type(type);
		pRole->Send(ntf);
	}
}



void PopWindows::PopQuestionire(Role *pRole)
{
	RoleMisc* record = pRole->Get<RoleMisc>();
	UINT32 nLv		 = pRole->GetLevel();
	UINT32 nIndex	 = record->GetAnswersIndex();
	UINT32 nVersion  = record->GetAnswersVersion();
	UINT32 nHint	 = record->GetHintFlag();
	bool  bFlag      = true;
	for(UINT32 i = 0; i <  GetGlobalConfig().QuestionnaireLevel.size(); i++)
	{
		if(nLv>=GetGlobalConfig().QuestionnaireLevel[i][0]&&nLv<=GetGlobalConfig().QuestionnaireLevel[i][1])
		{
			if (nVersion!=GetGlobalConfig().QuestionnaireVersion[i]||nIndex!=i)
			{
				if (nHint==0)
				{
					record->SetHintFlag(1);
					pRole->HintNotify(SYS_QUESTIONNAIRE, false);
					record->SetAnswersVersion(GetGlobalConfig().QuestionnaireVersion[i]);
					record->SetAnswersIndex(i);
					record->SetSaveFlag();
					bFlag = false;
					break;
				}
			}
			break;
		}
	}
	if (bFlag&&nHint)
	{
		pRole->HintNotify(SYS_QUESTIONNAIRE, true);
		record->SetHintFlag(0);
		record->SetSaveFlag();
	}

}
