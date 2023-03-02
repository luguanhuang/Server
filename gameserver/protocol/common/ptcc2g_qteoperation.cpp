#include "pch.h"
#include "common/ptcc2g_qteoperation.h"
#include "unit/enemy.h"
#include "unit/enemymanager.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "buff/XBuff.h"
#include "entity/XRole.h"
#include "component/XBuffComponent.h"

// generate by ProtoGen at date: 2016/2/25 14:13:02

static void SubHp(Role* role, UINT64 monsterid)
{
	Enemy* enemy = EnemyManager::Instance()->FindEnemy(monsterid);
	if (NULL == enemy)
	{
		SSWarn<<"enemy not find, monsterid:"<<monsterid<<END;
		return;
	}
	double hurt = role->GetAttr(TOTAL_PhysicalAtkMod) + role->GetAttr(TOTAL_MagicAtkMod);
	enemy->SubHp(role, hurt);
}

void PtcC2G_QTEOperation::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}

	switch (m_Data.type())
	{
	case 1:
		{
			SubHp(role, m_Data.monsterid());
			break;
		}
	case 2:
		{
			XBuff *pBuff = role->GetXRole()->BuffComponent()->GetBuffByID((int)m_Data.monsterid());
			if (NULL != pBuff)
			{
				double hurt = role->GetAttr(TOTAL_PhysicalAtkMod) + role->GetAttr(TOTAL_MagicAtkMod);
				pBuff->ChangeBuffHP(-hurt);
			}
			
			break;
		}
	default:
		break;
	}
}
