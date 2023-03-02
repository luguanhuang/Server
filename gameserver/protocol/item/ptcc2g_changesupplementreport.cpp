#include "pch.h"
#include "item/ptcc2g_changesupplementreport.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/item.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/1/18 17:24:47

void PtcC2G_ChangeSupplementReport::Process(UINT32 dwConnID)
{
	//Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	//if (NULL == role)
	//{
	//	return;
	//}
	//Bag* bag = role->Get<Bag>();
	//XItem* item = bag->FindItemUnique(m_Data.uniqueid());
	//if (NULL == item)
	//{
	//	bag->NotifySupplement(KKSG::ERR_ITEM_NOTEXIST);
	//	return;
	//}
	//if (m_Data.slot() >= (UINT32) GetGlobalConfig().SupplementCount)
	//{
	//	bag->NotifySupplement(KKSG::ERR_UNKNOWN);
	//	return;
	//}
	//bag->SetSupplement(m_Data.uniqueid(), m_Data.slot());
	//bag->NotifySupplement(KKSG::ERR_SUCCESS);
	return;
}
