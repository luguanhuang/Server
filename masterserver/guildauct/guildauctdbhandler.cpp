#include "pch.h"
#include "guildauctdbhandler.h"
#include "auction/ptcm2d_guildauctupdatentf.h"
#include "network/dblink.h"
#include "guildauct/guildauctbase.h"
#include "pb/enum.pb.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "guildauct/guildauctmgr.h"

void GADBHandler::Add(GABaseData* data)
{
	if(NULL == data)
	{
		return ;
	}
	PtcM2D_GuildAuctUpdateNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_INSERT);
	data->FillDBData(*ptc.m_Data.mutable_dbdata());
	DBLink::Instance()->SendTo(ptc);
}

void GADBHandler::Update(GABaseData* data)
{
	if(NULL == data)
	{
		return ;
	}
	PtcM2D_GuildAuctUpdateNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_UPDATE);
	data->FillDBData(*ptc.m_Data.mutable_dbdata());
	DBLink::Instance()->SendTo(ptc);
}

void GADBHandler::Del(UINT64 UID)
{
	PtcM2D_GuildAuctUpdateNtf ptc;
	ptc.m_Data.set_op(KKSG::AUCTDBDATA_DELETE);
	ptc.m_Data.mutable_dbdata()->set_uid(UID);
	DBLink::Instance()->SendTo(ptc);
}

bool GADBHandler::Read()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from guildauctitem";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table guildauctitem failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;

		UINT64 UID = 0;
		std::string alldata;

		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			UID = convert<UINT64>(pVal);
		}
		else
		{
			SSError << " read uid failed " << END;
			return false;	
		}
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			alldata.assign(pVal, dwLen);
		}
		else
		{
			SSError << " read alldata failed " << END;
			return false;
		}
		
		KKSG::GAItemDBData itemDBData;
		if(!itemDBData.ParseFromString(alldata))
		{
			SSError << " parse dbdata err UID = " << UID << END;
			return false;
		}
		if(itemDBData.uid() != UID)
		{
			SSError << " uid err uid = " << itemDBData.uid() << " UID = " << UID << END;
			return false;
		}

		GuildAuctMgr::Instance()->AddGuildAuctDBData(itemDBData);

	}
	
	return true;
}

