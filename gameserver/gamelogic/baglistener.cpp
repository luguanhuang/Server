#include "pch.h"
#include "item.h"
#include "baglistener.h"
#include "unit/role.h"
#include "rolefashion.h"
#include "common/ptcg2c_itemchangedntf.h"
#include "mail/mailsender.h"
#include "common/ptcg2c_gserrornotify.h"
#include "mail/mailconfig.h"
#include "define/tlogenum.h"

BagItemOperationRecord::BagItemOperationRecord()
{
	m_isrecyle = false;
	m_mailSender = NULL;
	m_needMail = false;
	m_subReason = -1;
}

BagItemOperationRecord::~BagItemOperationRecord()
{
	delete m_mailSender;
	m_mailSender = NULL;
}

void BagItemOperationRecord::OnAddItem(XItem *pItem)
{
	ItemChangeRecord record;
	record.op = m_isrecyle ? RECYLEADD_ITEM : ADD_ITEM;
	record.pItem = pItem;
	record.uid1 = 0;
	record.uid2 = 0;
	m_ChangeRecord .push_back(record);
}

void BagItemOperationRecord::OnRemoveItem(XItem *pItem)
{
	ItemChangeRecord record;
	record.op = REMOVE_ITEM;
	record.pItem = NULL;
	record.uid1 = pItem->uid;
	record.uid2 = 0;
	m_ChangeRecord .push_back(record);
}

void BagItemOperationRecord::OnSwapItem(XItem *pItem1, XItem *pItem2, int pos)
{
	ItemChangeRecord record;
	record.op = SWAP_ITEM;
	record.pItem = pItem1; 
	record.uid1 = ((pItem1 == NULL) ? 0 : pItem1->uid);
	record.uid2 = ((pItem2 == NULL) ? 0 : pItem2->uid);
	m_ChangeRecord .push_back(record);
}

void BagItemOperationRecord::OnChangeItemCount(XItem *pItem)
{
	ItemChangeRecord record;
	record.op = m_isrecyle ? RECYLECHANGE_TIEM : CHANGE_ITEM;
	record.pItem = pItem;
	record.uid1 = 0;
	record.uid2 = 0;
	m_ChangeRecord .push_back(record);
}

void BagItemOperationRecord::OnChangeVirtualItem(UINT32 itemID, UINT64 Count)
{
	for (auto i = m_VirtualItemRecord.begin(); i != m_VirtualItemRecord.end(); ++i)
	{
		if (i->itemID == itemID)
		{
			i->count = Count;
			return;
		}
	}

	VirtualItemRecord record;
	record.itemID = itemID;
	record.count = Count;
	m_VirtualItemRecord.push_back(record);
}

void BagItemOperationRecord::OnChangeItemAttr(XItem* pItem)
{
	ItemChangeRecord record;
	record.op = CHANGEATTR_ITEM;
	record.pItem = pItem;
	record.uid1 = 0;
	record.uid2 = 0;
	m_ChangeRecord .push_back(record);
}

void BagItemOperationRecord::OnAddFashion(FashionInfo* fashion)
{
	m_fashions.push_back(fashion);
}

void BagItemOperationRecord::OnBagFullAndSendMail(const ItemDesc& desc)
{
	if(NULL == m_mailSender)
	{
		m_mailSender = new MailSender();
	}
	m_needMail = true;
	m_mailSender->AddItem(desc);
}

void BagItemOperationRecord::OnBagFullAndSendMail(const XItem* pItem)
{
	if(NULL == m_mailSender)
	{
		m_mailSender = new MailSender();	
	}
	m_needMail = true;
	m_mailSender->AddItem(pItem);
}

void BagItemOperationRecord::NotifyClient(Role *pRole)
{
	NotifyChangedRecord(pRole);

	if(m_needMail && m_mailSender)
	{
		const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_BagFull);
		if(NULL == pConf)
		{
			return;
		}

		m_mailSender->SetContent(Mail_System, pConf->m_title, pConf->m_content);
		m_mailSender->AddItemExplain();
		m_mailSender->SetReason(ItemFlow_MailBagFull, m_subReason);
		m_mailSender->Send(pRole->GetID());

		PtcG2C_GSErrorNotify oPtc;
		oPtc.m_Data.set_errorno(KKSG::ERR_BAG_FULL);
		pRole->Send(oPtc);
	}
}

void BagItemOperationRecord::NotifyChangedRecord(Role *pRole)
{
	// fashion notify
	if (!m_fashions.empty())
	{
		pRole->Get<RoleFashion>()->NotifyChangeSelf(KKSG::ADD_FASHION, pRole, m_fashions, false);	
	}

	if (m_ChangeRecord.empty() && m_VirtualItemRecord.empty())
	{
		return;
	}
	std::unordered_set<UINT64> changedItems;

	std::vector<XItem*> vecChangeAttrItem;
	PtcG2C_ItemChangedNtf oItemChangePtc;
	oItemChangePtc.m_Data.set_isrearrange(false);
	for (auto i = m_ChangeRecord.begin(); i != m_ChangeRecord.end(); ++i)
	{
		switch (i->op)
		{
		case ADD_ITEM:
			{
				KKSG::Item *pDBItem = oItemChangePtc.m_Data.add_newitems();
				XItem::Item2KKSGItem(i->pItem, pDBItem);
				break;
			}
		case CHANGE_ITEM:
			{
				if(changedItems.find(i->pItem->uid) == changedItems.end())
				{
					oItemChangePtc.m_Data.add_changeitems(i->pItem->uid);
					oItemChangePtc.m_Data.add_changeitems(i->pItem->itemCount);

					changedItems.insert(i->pItem->uid);
				}
				break;
			}
		case REMOVE_ITEM:
			{
				oItemChangePtc.m_Data.add_removeitems(i->uid1);
				break;
			}
		case SWAP_ITEM:
			{
				oItemChangePtc.m_Data.add_swapitems(i->uid1);
				oItemChangePtc.m_Data.add_swapitems(i->uid2);
				if (NULL != i->pItem)
				{
					vecChangeAttrItem.push_back(i->pItem);
				}
				break;
			}
		case RECYLEADD_ITEM:
			{
				KKSG::Item *pDBItem = oItemChangePtc.m_Data.add_recyleadditems();
				XItem::Item2KKSGItem(i->pItem, pDBItem);
				break;
			}
		case RECYLECHANGE_TIEM:
			{
				oItemChangePtc.m_Data.add_recylechangeitems(i->pItem->uid);
				oItemChangePtc.m_Data.add_recylechangeitems(i->pItem->itemCount);
				break;
			}
		case CHANGEATTR_ITEM:
			{
				if (NULL == i->pItem)
				{
					continue;
				}
				bool bIsIn = false;
				for (auto j = vecChangeAttrItem.begin(); j != vecChangeAttrItem.end(); ++j)
				{
					if (NULL == (*j))
					{
						continue;
					}
					if ((*j)->uid == i->pItem->uid)
					{
						bIsIn = true;
					}
				}
				if (!bIsIn)
				{
					vecChangeAttrItem.push_back(i->pItem);
				}
				break;
			}
		default:
			break;
		}
	}

	for (auto j = vecChangeAttrItem.begin(); j != vecChangeAttrItem.end(); ++j)
	{
		KKSG::Item *pDBItem = oItemChangePtc.m_Data.add_attrchangeitems();
		XItem::Item2KKSGItem(*j, pDBItem);
	}

	for (auto k = m_VirtualItemRecord.begin(); k != m_VirtualItemRecord.end(); ++k)
	{
		oItemChangePtc.m_Data.add_virtualitemid(k->itemID);
		if (EXP == k->itemID)
		{
			oItemChangePtc.m_Data.add_virtualitemcount(pRole->GetRealExp());
		}
		else
		{
			oItemChangePtc.m_Data.add_virtualitemcount(k->count);
		}
	}

	pRole->Send(oItemChangePtc);
}
