#include "pch.h"
#include "mailmgr.h"
#include "timeutil.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "network/dblink.h"
#include "network/gslink.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "mail/ptcm2d_mailoperation.h"
#include "mail/ptcm2d_addtemplatemail.h"
#include "common/ptcm2c_mseventnotify.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "mailconfig.h"
#include "cmdline.h"
#include "define/systemiddef.h"
#include "mail/rpcm2g_givemailprize.h"
#include "loghelper/tlogr.h"
#include "util/timespecificmgr.h"
#include "role/activerolerecord.h"
#include "mail/rpcm2d_readmailfromdb.h"

INSTANCE_SINGLETON(CMailMgr)

#define MailId  27
#define MAX_MAIL_COUNT (GlobalConfig::GetInstance().MailMax)
#define EXPIRED_MAIL_AUTO_CLEAN_HOUR_OF_DAY	3
#define EXPIRED_MAIL_AUTO_CLEAN_ROLE_COUNT	200


static bool CmdSendSystemMail(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() <= 1)
	{
		SSError << "Usage: sendmail id" << END;
		return false;
	}
	UINT32 templateId = convert<UINT32>(args[1]);

	CMailMgr::Instance()->SendSystemMail(templateId);
	return true;
}

static bool CmdTestRoleLogin(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() <= 1)
	{
		SSError << "Usage: role id" << END;
		return false;
	}
	UINT64 qwRoleID = convert<UINT64>(args[1]);

	std::stringstream ss;
	ss << "account_" << qwRoleID;

	RoleInfoOnMs oInfo;
	oInfo.set_roleid(qwRoleID);
	oInfo.set_name(ss.str());
	oInfo.set_accountid(ss.str());
	oInfo.set_level(10);
	static UINT64 qwSessionID = 1;
	CRole* poRole = CRoleManager::Instance()->LoginRole(&oInfo, qwSessionID++);

	CMailMgr::Instance()->OnRoleLogin(poRole);
	return true;
}

CMailMgr::CMailMgr()
:m_poCleanTimer(NULL)
{
}

CMailMgr::~CMailMgr()
{
}

bool CMailMgr::Init()
{
	m_oMailCacheCtrl.Init(GetGlobalConfig().m_dwRoleMailCacheMaxNum);

	CmdLine::RegistCmd("sendmail", CmdSendSystemMail);
	CmdLine::RegistCmd("rolelogin", CmdTestRoleLogin);

	if(!_ReadAllRoleMailFromDB()) return false;
	if(!_ReadAllGlobalMailFromDB()) return false;
	if(!_ReadAllRoleRecordFromDB()) return false;

    TimeSpecific time(EXPIRED_MAIL_AUTO_CLEAN_HOUR_OF_DAY, 0, 0);
    m_dwCleanHandle = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CMailMgr::OnAutoCleanMail, this));
	return true;
}

void CMailMgr::Uninit()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_dwCleanHandle);
}

void CMailMgr::Update()
{
	if (m_lMail.empty())
	{
		return;
	}

	std::list<DummyMail*> temp;
	UINT32 i = 0;
	UINT32 nowtime = (UINT32)time(NULL);
	while (!m_lMail.empty() && i < kMaxSendPerFrame)
	{
		DummyMail*  mail = m_lMail.front();
		m_lMail.pop_front();
		if (nowtime > mail->latesend)
		{
			DoSendMail(mail);
			delete mail;

			++i;
		}
		else
		{
			temp.push_back(mail);
		}
	}
	for (auto j = temp.begin(); j != temp.end(); ++j)
	{
		m_lMail.push_back(*j);
	}
}

void CMailMgr::DoSendMail(DummyMail* mail)
{
	if(GetRoleMailInfo(mail->roleId) != NULL)
	{
		//内存Cache中
		if(!_AddRoleMail(mail->roleId, mail->mailData))
		{
			LogError("Add mail failed, role [%llu] mail [%llu]", mail->roleId, mail->mailData.uid());
			return ;
		}

		CRole* poRole = CRoleManager::Instance()->GetByRoleID(mail->roleId);
		if(poRole != NULL)
		{
			SendHintNotify(poRole, true);
		}
	}

	_UpdateToDB(mail->roleId, KKSG::MAIL_OP_ADD, mail->mailData, mail->isGlobal, mail->globalId);
}

void CMailMgr::OnRoleLogin(CRole* role)
{
	if(m_oMailCacheCtrl.Get(role->GetID()))
	{
		//in the cache
		_ProcessRoleMailLogin(role);
	}
	else
	{
		//Read from db
		RpcM2D_ReadMailFromDB* rpc = RpcM2D_ReadMailFromDB::CreateRpc();
		rpc->m_oArg.set_roleid(role->GetID());
		DBLink::Instance()->SendTo(*rpc);
	}
}

void CMailMgr::OnReadRoleMailFromDB(CRole* role, const KKSG::ReadMailFromDBRes &roRes)
{
	CMailDataAddFunctor oAdder;
	CMailDataDelFunctor oDeler;
	oAdder.m_qwRoleID = role->GetID();
	oAdder.m_poRes = &roRes;

	m_oMailCacheCtrl.Put(role->GetID(), oAdder, oDeler);
}

void CMailMgr::_ProcessRoleMailLogin(CRole* role)
{
	UINT32 createtime = role->GetCreateTime();

	UINT64 qwRoleID = role->GetID();
	UINT32 dwNow = TimeUtil::GetTime();
	UINT64 qwMaxID = _GetRoleMaxGlobalMailID(qwRoleID);
	CMailMap::iterator it = m_oGlobalMailInfo.upper_bound(qwMaxID);
	for (; it != m_oGlobalMailInfo.end();)
	{
		KKSG::SMail& roMail = it->second;
		if(roMail.timeexpire() <= dwNow)
		{
			m_oGlobalMailInfo.erase(it++);
			continue;
		}
		if (0 == createtime || it->second.timestamp() <= createtime)
		{
			++it;
			continue;
		}

		KKSG::SMail oSaveData;
		oSaveData.CopyFrom(roMail);
		oSaveData.set_uid(NewMailID());
		SendMail(qwRoleID, oSaveData, true, roMail.uid(), 0);
		_UpdateRoleMaxGlobalMailID(qwRoleID, roMail.uid());
		++it;
	}
	SendHintNotify(role, false); 
}

bool CMailMgr::_ReadAllRoleMailFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from mail";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table mail failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	CActiveRoleRecord::CActiveRoleList oActiveRoleMap;
	CActiveRoleRecord::Instance()->GetActiveRoleTopN(GetGlobalConfig().m_dwRoleMailCacheMaxNum, oActiveRoleMap);

	std::vector<UINT64> oExpired;
	UINT32 dwNow = TimeUtil::GetTime();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwMailID = 0;
		UINT64 qwRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwMailID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			qwRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'roleid' column failed");
			return false;
		}

		std::string strData;
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'mail' column failed");
			return false;
		}

		KKSG::SMail oMail;
		if(!oMail.ParseFromString(strData))
		{
			LogError("Parse SMail failed, mailID %llu, roleID %llu", qwMailID, qwRoleID);
			return false;
		}
		if(oMail.timeexpire() <= dwNow)
		{
			oExpired.push_back(qwMailID);
			continue;
		}

		//不活跃
		if(oActiveRoleMap.find(qwRoleID) == oActiveRoleMap.end()) continue;

		if(!_AddRoleMail(qwRoleID, oMail))
		{
			LogError("Add Mail failed, mailID %llu, roleID %llu", qwMailID, qwRoleID);
		}
	}
	poRes->Release();

	for(std::vector<UINT64>::size_type i = 0; i < oExpired.size(); ++i)
	{
		ss.str("");
		ss << "delete from mail where _id=" << oExpired[i];
		poMySqlConn->Query(ss.str().c_str(), ss.str().size(), NULL);
		LogInfo("Delete expired mail [%llu] from db", oExpired[i]);
	}

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all role mail data from db use time: %llu ms", qwTimeUsed);

	//Init cache control info
	CMailDataAddFunctor oAdder;
	CMailDataDelFunctor oDeler;
	for(CAllRoleMailMap::iterator it = m_oRoleMailInfo.begin(); it != m_oRoleMailInfo.end(); ++it)
	{
		m_oMailCacheCtrl.Put(it->first, oAdder, oDeler);
	}

	LogInfo("Init role mail cache, initial count [%u], max count [%u]", (UINT32)m_oRoleMailInfo.size(), GetGlobalConfig().m_dwRoleMailCacheMaxNum);
	return true;
}

bool CMailMgr::_ReadAllGlobalMailFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from mailtemplate";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table mailtemplate failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	std::vector<UINT64> oExpired;
	UINT32 dwNow = TimeUtil::GetTime();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwMailID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwMailID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'mail' column failed");
			return false;
		}

		KKSG::SMail oMail;
		if(!oMail.ParseFromString(strData))
		{
			LogError("Parse SMail failed, Global mailID %llu", qwMailID);
			return false;
		}
		if(oMail.timeexpire() <= dwNow)
		{
			oExpired.push_back(qwMailID);
			continue;
		}

		if(!_AddGlobalMail(oMail))
		{
			return false;
		}
	}
	poRes->Release();

	for(std::vector<UINT64>::size_type i = 0; i < oExpired.size(); ++i)
	{
		ss.str("");
		ss << "delete from mailtemplate where _id=" << oExpired[i];
		poMySqlConn->Query(ss.str().c_str(), ss.str().size(), NULL);
		LogInfo("Delete expired mail [%llu] from db", oExpired[i]);
	}

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all mail template data from db use time: %llu ms", qwTimeUsed);
	return true;
}

bool CMailMgr::_ReadAllRoleRecordFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from mailrecord";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table mail failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwRoleID = 0;
		UINT64 qwMailID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'roleid' column failed");
			return false;
		}
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			qwMailID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'maxtemplateid' column failed");
			return false;
		}

		_UpdateRoleMaxGlobalMailID(qwRoleID, qwMailID);
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all role mail record from db use time: %llu ms", qwTimeUsed);
	return true;
}

bool CMailMgr::SendMail(UINT64 qwRoleID, const MailData& roData)
{
	if(CRoleSummaryMgr::Instance()->GetRoleSummary(qwRoleID) == NULL)
	{
		LogError("Role %llu does not exist", qwRoleID);
		return false;
	}

	///> 判断道具ID合法性，判断参数合法性 to added
	
	KKSG::SMail oSaveData;
	roData.ToKKSG(oSaveData);

	return SendMail(qwRoleID, oSaveData, 0, 0, roData.m_latesend);	
}

bool CMailMgr::SendMail(UINT64 qwRoleID, const KKSG::SMail& oSaveData, bool isGlobal, UINT64 templateId, UINT32 latesend)
{
	DummyMail* mail = new DummyMail;
	mail->roleId = qwRoleID;
	mail->mailData.CopyFrom(oSaveData);
	mail->isGlobal = isGlobal;
	mail->globalId = templateId;
	mail->latesend = latesend;
	m_lMail.push_back(mail);
	DoTxLog(qwRoleID, oSaveData, Tx_Mail_Get);
	return true;
}

bool CMailMgr::SendMail(UINT64 qwRoleID, UINT32 templateId)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(templateId);
	if (!conf)
	{
		return false;
	}
	MailData mail;
	mail.m_title = conf->m_title;
	mail.m_content = conf->m_content;
	mail.m_expiretime = MAIL_EXPIRE_TIME;
	return SendMail(qwRoleID, mail);
}

bool CMailMgr::SendMail(UINT64 qwRoleID, UINT32 templateId, const std::vector<ItemDesc>& items)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(templateId);
	if (!conf)
	{
		return false;
	}
	MailData mail;
	mail.m_title = conf->m_title;
	mail.m_content = conf->m_content;
	mail.m_items = items;
	mail.m_expiretime = MAIL_EXPIRE_TIME;
	return SendMail(qwRoleID, mail);
}

bool CMailMgr::SendSystemMail(const MailData& roData)
{
	//判断道具ID合法性，判断参数合法性 to added

	KKSG::SMail oSaveData;
	roData.ToKKSG(oSaveData);
	if(!_AddGlobalMail(oSaveData))
	{
		LogError("Add global mail failed, mail [%llu]", oSaveData.uid());
		return false;
	}

	_AddTemplateMailToDB(oSaveData);

	CRoleManager::role_iterator it = CRoleManager::Instance()->Begin();
	CRoleManager::role_iterator itEnd = CRoleManager::Instance()->End();
	for(; it != itEnd; ++it)
	{
		CRole* poRole = *it;

		oSaveData.set_uid(NewMailID());
		SendMail(poRole->GetID(), oSaveData, true, oSaveData.uid(), 0);
		_UpdateRoleMaxGlobalMailID(poRole->GetID(), oSaveData.uid());
	}

	return true;
}

bool CMailMgr::SendSystemMail(UINT32 templateId)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(templateId);
	if (!conf)
	{
		return false;
	}
	MailData mail;
	mail.m_title = conf->m_title;
	mail.m_content = conf->m_content;
	mail.m_expiretime = MAIL_EXPIRE_TIME;
	return SendSystemMail(mail);
}

bool CMailMgr::SendGuildMail(UINT64 guildId, const MailData& roData)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return false;
	}

	const std::map<UINT64, GuildMember>& allMember = pGuild->GetGuildMember();
	for (auto i = allMember.begin(); i != allMember.end(); ++i)
	{
		SendMail(i->first, roData);
	}
	
	return true;
}

bool CMailMgr::SendGuildMail(UINT64 guildId, UINT32 templateId)
{
	const MailConf* conf = MailConfig::Instance()->GetMailConf(templateId);
	if (!conf)
	{
		return false;
	}
	MailData mail;
	mail.m_title = conf->m_title;
	mail.m_content = conf->m_content;
	mail.m_expiretime = MAIL_EXPIRE_TIME;
	return SendGuildMail(guildId, mail);
}

bool CMailMgr::Fetch(UINT64 qwRoleID, UINT32& dwPage, UINT32& dwPageNum, UINT32 dwCountPerPage, std::vector<KKSG::SMail*>& roMailList)
{
	if(dwCountPerPage == 0) return false;

	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	if(it == m_oRoleMailInfo.end())
	{
		return false;
	}

	UINT32 dwNow = TimeUtil::GetTime();
	CMailMap& roMailMap = it->second;
	for(CMailMap::iterator k = roMailMap.begin(); k != roMailMap.end();)
	{
		if(k->second.timeexpire() < dwNow)
		{
			_UpdateToDB(qwRoleID, KKSG::MAIL_OP_DELETE, k->second);
			roMailMap.erase(k++);
		}
		else
		{
			++k;
		}
	}

	roMailList.clear();
	dwPageNum = (UINT32)(roMailMap.size() / dwCountPerPage);
	if(roMailMap.size() % dwCountPerPage != 0)
	{
		dwPageNum++;
	}
	if(dwPage >= dwPageNum)
	{
		dwPage = 0;
	}
	if(roMailMap.empty())
	{
		return true;
	}

	UINT32 dwIndex = 0;
	UINT32 dwStart = dwPage * dwCountPerPage;
	for(CMailMap::reverse_iterator k = roMailMap.rbegin(); k != roMailMap.rend(); ++k, ++dwIndex)
	{
		if(dwIndex >= dwStart && dwIndex < dwStart + dwCountPerPage)
		{
			roMailList.push_back(&k->second);
		}
	}
	return true;
}

bool CMailMgr::Read(UINT64 qwRoleID, UINT64 qwMailID)
{
	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	if(it == m_oRoleMailInfo.end())
	{
		return false;
	}

	CMailMap& roMailMap = it->second;
	CMailMap::iterator itFind = roMailMap.find(qwMailID);
	if(itFind == roMailMap.end())
	{
		return false;
	}

	KKSG::SMail& roMail = itFind->second;
	if(roMail.timeexpire() < TimeUtil::GetTime())
	{
		_UpdateToDB(qwRoleID, KKSG::MAIL_OP_DELETE, roMail);
		roMailMap.erase(itFind);
		return false;
	}

	roMail.set_isread(true);
	_UpdateToDB(qwRoleID, KKSG::MAIL_OP_UPDATE, roMail);

	CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if(poRole != NULL)
	{
		SendHintNotify(poRole, false);
	}

	return true;
}

bool CMailMgr::Reward(UINT64 qwRoleID, UINT64 qwMailID, CRole* role, UINT32 delayid)
{
	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	if (it == m_oRoleMailInfo.end())
	{
		return false;
	}

	CMailMap& roMailMap = it->second;
	CMailMap::iterator itFind = roMailMap.find(qwMailID);
	if (itFind == roMailMap.end())
	{
		return false;
	}

	KKSG::SMail& roMail = itFind->second;
	if (roMail.timeexpire() < TimeUtil::GetTime())
	{
		_UpdateToDB(qwRoleID, KKSG::MAIL_OP_DELETE, roMail);
		roMailMap.erase(itFind);
		return false;
	}
	if (roMail.state() != Mail_Given)
	{
		return false;
	}
	roMail.set_state(Mail_Taken);
	//Send rpc to gs
	RpcM2G_GiveMailPrize* oRpc = RpcM2G_GiveMailPrize::CreateRpc();
	oRpc->m_oArg.add_mail()->CopyFrom(roMail);
	oRpc->m_oArg.set_roleid(qwRoleID);
	oRpc->m_oArg.set_delayid(delayid);

	role->SendMsgToGS(*oRpc);

	return true;
}

bool CMailMgr::RewardAll(UINT64 qwRoleID, CRole* role, UINT32 delayid)
{
	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	if (it == m_oRoleMailInfo.end())
	{
		return false;
	}
	
	UINT32 nowTime = TimeUtil::GetTime();
	RpcM2G_GiveMailPrize* oRpc = RpcM2G_GiveMailPrize::CreateRpc();
	oRpc->m_oArg.set_roleid(qwRoleID);
	oRpc->m_oArg.set_delayid(delayid);
	for (auto i = it->second.begin(); i != it->second.end(); )
	{
		KKSG::SMail& roMail = i->second;
		if (roMail.timeexpire() < nowTime)
		{
			_UpdateToDB(qwRoleID, KKSG::MAIL_OP_DELETE, roMail);
			it->second.erase(i++);
			continue;
		}
		if (roMail.state() == Mail_Given)
		{
			roMail.set_state(Mail_Taken);
			oRpc->m_oArg.add_mail()->CopyFrom(roMail);
		}
		else
		{
			roMail.set_isread(true);
			_UpdateToDB(qwRoleID, KKSG::MAIL_OP_UPDATE, roMail);
		}
		i++;
	}
	if (oRpc->m_oArg.mail_size() == 0)
	{
		SendHintNotify(role, false);
		return false;
	}
	role->SendMsgToGS(*oRpc);
	return true;
}

void CMailMgr::DoReward(UINT64 qwRoleID, UINT64 qwMailID, bool isSucceed)
{
	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	if (it == m_oRoleMailInfo.end())
	{
		return;
	}

	CMailMap& roMailMap = it->second;
	CMailMap::iterator itFind = roMailMap.find(qwMailID);
	if (itFind == roMailMap.end())
	{
		return;
	}
	KKSG::SMail& roMail = itFind->second;
	if (roMail.state() != Mail_Taken)
	{
		LogError("roMail state error, state:%u, mailid:%llu", roMail.state(), qwMailID);
		return;
	}

	if (isSucceed)
	{
		roMail.set_isread(true);
		_UpdateToDB(qwRoleID, KKSG::MAIL_OP_UPDATE, roMail);
		LogDebug("Get mail succeed, mailid:%llu", qwMailID);
		DoTxLog(qwRoleID, roMail, Tx_Mail_Reward);
	}
	else
	{
		roMail.set_state(Mail_Given);
		LogDebug("Get mail failed, mailid:%llu", qwMailID);
	}

	CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if (poRole != NULL)
	{
		SendHintNotify(poRole, false);
	}
}

bool CMailMgr::Delete(UINT64 qwRoleID, UINT64 qwMailID)
{
	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	if(it == m_oRoleMailInfo.end())
	{
		return false;
	}

	return _InnerDelete(it->second, qwMailID, qwRoleID);
}

bool CMailMgr::DeleteAllMail(UINT64 qwRoleID, bool bByIdip)
{
	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	if(it == m_oRoleMailInfo.end())
	{
		if(!bByIdip)
		{
			return false;
		}

		_UpdateToDB(qwRoleID, KKSG::MAIL_OP_DELETE_ALL, KKSG::SMail());
	}
	else
	{
		it->second.clear();
		_UpdateToDB(qwRoleID, KKSG::MAIL_OP_DELETE_ALL, KKSG::SMail());

		CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
		if (poRole != NULL)
		{
			SendHintNotify(poRole, false);
		}
	}

	return true;
}

void CMailMgr::_UpdateToDB(UINT64 qwRoleID, KKSG::MailOpType op, const KKSG::SMail& data, bool bByTemplate, UINT64 qwTemplateID)
{
	PtcM2D_MailOperation oPtc;
	oPtc.m_Data.set_roleid(qwRoleID);
	oPtc.m_Data.set_op(op);
	oPtc.m_Data.set_bytemplate(bByTemplate);
	oPtc.m_Data.set_templateid(qwTemplateID);
	*oPtc.m_Data.mutable_maildata() = data;
	DBLink::Instance()->SendTo(oPtc);
}

void CMailMgr::_AddTemplateMailToDB(const KKSG::SMail& data)
{
	PtcM2D_AddTemplateMail oPtc;
	*oPtc.m_Data.mutable_maildata() = data;
	DBLink::Instance()->SendTo(oPtc);
}

bool CMailMgr::_AddRoleMail(UINT64 qwRoleID, const KKSG::SMail& data)
{
	CMailMap& roMap = m_oRoleMailInfo[qwRoleID];
	if (roMap.size() >= MAX_MAIL_COUNT)
	{
		//CRole* role = CRoleManager::Instance()->GetByRoleID(qwRoleID);
		//if (NULL != role)
		//{
		//	_MailFull(role);
		//}
		///> 删掉时间最早的一份邮件
		if (!roMap.empty())
		{
			auto i = roMap.begin();
			SSWarn<<"mail is full, delete mail:"<<i->first<<END;
			_InnerDelete(roMap, i->first, qwRoleID);
		}
	}
	std::pair<CMailMap::iterator, bool> pr = roMap.insert(std::make_pair(data.uid(), data));
	return pr.second;
}

bool CMailMgr::_AddGlobalMail(const KKSG::SMail& data)
{
	std::pair<CMailMap::iterator, bool> pr = m_oGlobalMailInfo.insert(std::make_pair(data.uid(), data));
	return pr.second;
}

void CMailMgr::_UpdateRoleMaxGlobalMailID(UINT64 qwRoleID, UINT64 qwMaxGlobalMailID)
{
	m_oRoleMaxIDInfo[qwRoleID] = qwMaxGlobalMailID;
}

UINT64 CMailMgr::_GetRoleMaxGlobalMailID(UINT64 qwRoleID)
{
	CMaxMailIDMap::iterator it = m_oRoleMaxIDInfo.find(qwRoleID);
	return it == m_oRoleMaxIDInfo.end() ? 0 : it->second;
}

void CMailMgr::SendHintNotify(CRole* role, bool istrue)
{
	CMailMap* mails = GetMails(role->GetID());
	if (NULL == mails)
	{
		return;
	}
	bool isunread = false;
	if (istrue)
	{
		isunread = true;
	}
	else
	{
		for (auto i = mails->begin(); i != mails->end(); ++i)
		{
			if (!i->second.isdelete() && (!i->second.isread() || i->second.state() == Mail_Given))	
			{
				isunread = true;
				break;
			}
		}
	}
	role->HintNotify(SysMailId, !isunread);	
}

std::map<UINT64, KKSG::SMail>* CMailMgr::GetMails(UINT64 roleid)
{
	auto i = m_oRoleMailInfo.find(roleid);
	if (i != m_oRoleMailInfo.end())
	{
		return &i->second;
	}
	return NULL;
}

void CMailMgr::_MailFull(CRole* role)
{
	PtcM2C_MSEventNotify ntf;
	ntf.m_Data.set_notify("mail full");
	role->Send(ntf);
}

bool CMailMgr::_InnerDelete(std::map<UINT64, KKSG::SMail>& mails, UINT64 mailid, UINT64 roleid)
{
	CMailMap::iterator itFind = mails.find(mailid);
	if(itFind == mails.end())
	{
		return false;
	}

	KKSG::SMail& roMail = itFind->second;
	if(roMail.timeexpire() < TimeUtil::GetTime())
	{
		_UpdateToDB(roleid, KKSG::MAIL_OP_DELETE, roMail);
		mails.erase(itFind);
		return false;
	}

	if (roMail.state() == Mail_Given)
	{
		SSWarn<<"try delete mail, attachment not taken, roleid:"<<roleid<<END;
		return false;
	}

	roMail.set_isdelete(true);
	_UpdateToDB(roleid, KKSG::MAIL_OP_DELETE, roMail);

	mails.erase(itFind);

	CRole* poRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (poRole != NULL)
	{
		SendHintNotify(poRole, false);
	}
	return true;
}

void CMailMgr::Test()
{
	static UINT32 t1 = TimeUtil::GetTime();
	UINT32 t2 = TimeUtil::GetTime();

	if(t2 - t1 > 10)
	{
		t1 = t2;
		MailData data;
		data.m_title = "test";
		data.m_content = "mail";
		data.m_expiretime = 10000;
		data.m_items.push_back(ItemDesc(100, 2));
		SendSystemMail(data);
	}
}

void CMailMgr::DoTxLog(UINT64 qwRoleID, const KKSG::SMail& data, INT32 type)
{
	TMailFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_Roleid = qwRoleID;
	oLog.m_Op = type;
	oLog.m_Data = data;
	oLog.Do();
}

bool CMailMgr::OnAutoCleanMail()
{
	if(GetMailCleanTimer() != NULL)
	{
		LogError("################### Error, Prev day auto clean mail does not finished ####################");
		return true;
	}

	ExpiredMailCleanTimer* poTimer = new ExpiredMailCleanTimer();
	poTimer->m_itBeg = m_oRoleMailInfo.begin();
	poTimer->m_itEnd = m_oRoleMailInfo.end();
	poTimer->m_hTimerHandle = CTimerMgr::Instance()->SetTimer(poTimer, 0, 1000, -1, __FILE__, __LINE__);

	SetMailCleanTimer(poTimer);
	return true;
}

void CMailMgr::AddRoleMailInfo(UINT64 qwRoleID)
{
	CMailMap oMap;
	m_oRoleMailInfo.insert(std::make_pair(qwRoleID, oMap));
}

void CMailMgr::DelRoleMaillInfo(UINT64 qwRoleID)
{
	if(m_poCleanTimer == NULL)
	{
		m_oRoleMailInfo.erase(qwRoleID);
	}
	else
	{
		CAllRoleMailMap::iterator itFind = m_oRoleMailInfo.find(qwRoleID);
		if(itFind == m_oRoleMailInfo.end())
		{
			return;
		}

		if(itFind == m_poCleanTimer->m_itBeg)
		{
			m_poCleanTimer->m_itBeg++;
		}

		m_oRoleMailInfo.erase(itFind);
	}
}

std::map<UINT64, KKSG::SMail>* CMailMgr::GetRoleMailInfo(UINT64 qwRoleID)
{
	CAllRoleMailMap::iterator it = m_oRoleMailInfo.find(qwRoleID);
	return it == m_oRoleMailInfo.end() ? NULL : &it->second;
}

void CMailMgr::ExpiredMailCleanTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 dwNow = TimeUtil::GetTime();
	for(INT32 i = 0; i < EXPIRED_MAIL_AUTO_CLEAN_ROLE_COUNT && m_itBeg != m_itEnd; ++i, ++m_itBeg)
	{
		UINT64 qwRoleID = m_itBeg->first;
		CMailMap& roMailMap = m_itBeg->second;
		for(CMailMap::iterator k = roMailMap.begin(); k != roMailMap.end();)
		{
			if(k->second.timeexpire() < dwNow)
			{
				CMailMgr::Instance()->_UpdateToDB(qwRoleID, KKSG::MAIL_OP_DELETE, k->second);
				roMailMap.erase(k++);
				++m_dwTotalCount;
			}
			else
			{
				++k;
			}
		}
	}

	if(m_itBeg == m_itEnd)
	{
		if(m_hTimerHandle != INVALID_HTIMER)
		{
			CTimerMgr::Instance()->KillTimer(m_hTimerHandle);
			m_hTimerHandle = INVALID_HTIMER;
			CMailMgr::Instance()->SetMailCleanTimer(NULL);
		}
		LogInfo("Expired mail auto clean finished, delete expired mail count [%u]", m_dwTotalCount);
		delete this;
	}
}