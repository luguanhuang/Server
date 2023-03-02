#include "pch.h"
#include "auctiondbhandler.h"
#include "auctionbase.h"
#include "network/dblink.h"
#include "auction/ptcm2d_auctsaleitemdbntf.h"
#include "db/mysqlmgr.h"
#include "auctionmgr.h"
#include "util.h"
#include "auction/ptcm2d_aucttraderecorddbntf.h"
#include "traderecord.h"

void AuctDbHandler::AddSaleItem(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	SSInfo << __FUNCTION__ << " auctuid = " << pSaleItem->GetUID() << END;
	PtcM2D_AuctSaleItemDBNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_INSERT);
	pSaleItem->FillDBData(ptc.m_Data);
	DBLink::Instance()->SendTo(ptc);
}

void AuctDbHandler::UpdateSaleItem(AuctionItem* pSaleItem)
{
	if(NULL == pSaleItem)
	{
		return ;
	}
	SSInfo << __FUNCTION__ << " auctuid = " << pSaleItem->GetUID() << " state = " << pSaleItem->GetState() << " itemcount = " << pSaleItem->GetItemCount() << END;
	PtcM2D_AuctSaleItemDBNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_UPDATE);
	pSaleItem->FillDBData(ptc.m_Data);
	DBLink::Instance()->SendTo(ptc);
}

void AuctDbHandler::DelSaleItem(UINT64 auctuid)
{
	SSInfo << __FUNCTION__ << " auctuid = " << auctuid << END;
	PtcM2D_AuctSaleItemDBNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_DELETE);
	ptc.m_Data.set_auctuid(auctuid);
	DBLink::Instance()->SendTo(ptc);
}

bool AuctDbHandler::ReadSaleItemFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from auctsaleitem";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table auctsaleitem failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;

		UINT64 auctuid = 0;
		UINT64 roleid = 0;
		std::string roleaccount;
		int state = 0;
		UINT32 perprice = 0;
		UINT32 reqsaletime = 0;
		UINT32 onsaletime = 0;
		UINT32 outsaletime = 0;
		std::string itemstr;

		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			auctuid = convert<UINT64>(pVal);
		}
		else
		{
			SSError << " read _id failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			roleid = convert<UINT64>(pVal);
		}
		else
		{
			SSError << " read roleid failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			roleaccount.assign(pVal, dwLen);
		}
		else
		{
			SSError << " read roleaccount failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(3, &pVal, dwLen))
		{
			state = convert<int>(pVal);
		}
		else
		{
			SSError << " read state failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(4, &pVal, dwLen))
		{
			perprice = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read perprice failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(5, &pVal, dwLen))
		{
			reqsaletime = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read reqsaletime failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(6, &pVal, dwLen))
		{
			onsaletime = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read onsaletime failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(7, &pVal, dwLen))
		{
			outsaletime = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read outsaletime failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(8, &pVal, dwLen))
		{
			itemstr.assign(pVal, dwLen);
		}
		else
		{
			SSError << " read itemstr failed " << END;
			return false;
		}
	
		AuctionMgr::Instance()->AddDBSaleItem(auctuid, roleid, (AuctionItemState)state, perprice, (time_t)reqsaletime, (time_t)onsaletime, itemstr, roleaccount, (time_t)outsaletime);
	}
	poRes->Release();

	return true;
}

void AuctDbHandler::AddTradeRecord(ATRecord& record)
{
	PtcM2D_AuctTradeRecordDBNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_INSERT);
	record.FillDBData(ptc.m_Data);
	DBLink::Instance()->SendTo(ptc);
}

void AuctDbHandler::DelTradeRecord(UINT32 attime)
{
	PtcM2D_AuctTradeRecordDBNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_DELETE);
	ptc.m_Data.set_attime(attime);
	DBLink::Instance()->SendTo(ptc);
}

bool AuctDbHandler::ReadTradeRecordFromDB(ATradeRecord* pRecord)
{
	if(NULL == pRecord)
	{
		return false;
	}

	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from aucttraderecord";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table aucttraderecord failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;
		
		UINT64 uid = 0;
		UINT32 itemid = 0;
		UINT32 itemcount = 0;
		UINT32 perprice = 0;
		UINT32 attime = 0;
		UINT64 auctuid = 0;
		std::string buyaccount;
		std::string saleaccount;

		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			uid = convert<UINT64>(pVal);
		}
		else
		{
			SSError << " read _id failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			itemid = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read itemid failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			itemcount = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read itemcount failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(3, &pVal, dwLen))
		{
			perprice = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read perprice failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(4, &pVal, dwLen))
		{
			attime = convert<UINT32>(pVal);
		}
		else
		{
			SSError << " read attime failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(5, &pVal, dwLen))
		{
			auctuid = convert<UINT64>(pVal);
		}
		else
		{
			SSError << " read auctuid failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(6, &pVal, dwLen))
		{
			buyaccount.assign(pVal, dwLen);
		}
		else
		{
			SSError << " read buyaccount failed " << END;
			return false;
		}
		if(poRes->GetFieldValue(7, &pVal, dwLen))
		{
			saleaccount.assign(pVal, dwLen);
		}
		else
		{
			SSError << " read saleccount failed " << END;
			return false;
		}	
	
		pRecord->AddOneDBRecord(uid, itemid, itemcount, perprice, (time_t)attime, auctuid, buyaccount, saleaccount);

	}
	poRes->Release();

	return true;
}
