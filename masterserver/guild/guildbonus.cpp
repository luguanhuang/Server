#include "pch.h"
#include "guild/guildbonus.h"
#include "guildlistener.h"
#include "pb/project.pb.h"
#include "guild/rpcc2g_getguildbonuslist.h"
#include "guild/ptcg2c_newguildbonusntf.h"
#include "guild/ptcm2g_mnewguildbonusntf.h"
#include "table/guildConfig.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "foreach.h"
#include "util/gametime.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"
#include "network/gslink.h"
#include "guild/guildcheckin.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "notice/noticemgr.h"
#include "guild/ptcm2c_sendguildbonusntf.h"
#include "loghelper/tlogr.h"
#include "platforminfo/platformfriendmgr.h"
#include "guildbonus/ptcm2c_guildbonusgetall.h"

GuildBonusMgr::GuildBonusMgr(Guild* pGuild)
{
	m_pGuild = pGuild;
	m_curBonusID = 0;
	m_bonusList.clear();
	m_checkinBonusData.clear();
	m_updateDay = 0;

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timehandler = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&GuildBonusMgr::onTimer, this));

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 60 * 1000, -1, __FILE__, __LINE__);
}

GuildBonusMgr::~GuildBonusMgr()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timehandler);

	CTimerMgr::Instance()->KillTimer(m_timer);
}

void GuildBonusMgr::OnAddMember(Guild *pGuild, GuildMember *pMember)
{
	if (!pGuild || !pMember)
	{
		return;
	}

	CRole* role = CRoleManager::Instance()->GetByRoleID(pMember->qwRoleID);
	if (!role)
	{
		return;
	}

	onLogin(role);
}

void GuildBonusMgr::OnGuildLevelUp(Guild *pGuild)
{
	if (!pGuild)
	{
		return;
	}

	addGuildBonus(KKSG::GBONUS_LEVELUP, pGuild->GetLevel(), 0);
}

bool GuildBonusMgr::IsChanged()
{
	return m_isModified.TestAndReset();
}

bool GuildBonusMgr::Load(const KKSG::GuildAllDBData & guilddata)
{
	KKSG::GuildBonusList bonusList;
	if (!guilddata.has_guildbonus())
	{
		return false;
	}

	if (guilddata.guildbonus().empty())
	{
		return false;
	}

	if (!bonusList.ParseFromString(guilddata.guildbonus()))
	{
		return false;
	}
	
	m_curBonusID = bonusList.curbonusid();
	for(auto i = 0; i < bonusList.bonuslist_size(); ++i)
	{
		GuildBonusInfo bonus;
		bonus.load(bonusList.bonuslist(i));
		m_bonusList.insert(std::make_pair(bonus.bonusID, bonus));
	}
	if (bonusList.has_updateday())
	{
		m_updateDay = bonusList.updateday();
	}
	m_checkinBonusData.clear();
	for (auto i = 0; i < bonusList.checkinbonusdata_size(); ++i)
	{
		GuildCheckInBonus info;
		info.load(bonusList.checkinbonusdata(i));
		m_checkinBonusData.push_back(info);
	}

	for (auto i = 0; i < bonusList.sendlist_size(); ++i)
	{
		GuildBonusInfo bonus;
		bonus.load(bonusList.sendlist(i));
		m_manualSendBonusList.insert(std::make_pair(bonus.bonusID, bonus));
	}

	Update();

	return true;
}

void GuildBonusMgr::Save(KKSG::GuildAllDBData & guilddata)
{
	guilddata.mutable_guildbonus()->assign(ToBinary());
}

const std::string GuildBonusMgr::ToBinary()
{
	KKSG::GuildBonusList list;
	list.set_curbonusid(m_curBonusID);
	for (auto iter = m_bonusList.begin(); iter != m_bonusList.end(); ++iter)
	{
		KKSG::GuildBonus* temp = list.add_bonuslist();
		iter->second.save(*temp);
	}

	list.set_updateday(m_updateDay);
	for (auto iter = m_checkinBonusData.begin(); iter != m_checkinBonusData.end(); ++iter)
	{
		KKSG::GuildCheckInBonusData* data = list.add_checkinbonusdata();
		iter->save(*data);
	}

	for (auto iter = m_manualSendBonusList.begin(); iter != m_manualSendBonusList.end(); ++iter)
	{
		KKSG::GuildBonus* temp = list.add_sendlist();
		iter->second.save(*temp);
	}
	return list.SerializeAsString();

}

void GuildBonusMgr::_generateBonusAverageNum(GuildBonusInfo* pInfo)
{
	if (!pInfo)
	{
		return;
	}
	
	pInfo->getBonusList.clear();

	GetBonusInfo info;
	for (UINT32 i = 0; i < pInfo->maxPeopleNum; ++i)
	{
		info.getNum = pInfo->bonusNum / pInfo->maxPeopleNum;
		pInfo->getBonusList.push_back(info);
	}
}

//void GuildBonusMgr::_generateBonusRandomNum(GuildBonusInfo* pInfo)
//{
//	if (!pInfo)
//	{
//		return;
//	}
//	std::multiset<UINT32> randPer;
//	std::vector<UINT32> result;
//	std::vector<std::vector<UINT32>::iterator> resultIterNotZero;
//	std::vector<std::vector<UINT32>::iterator> resultIterZero;
//
//	for (size_t i = 0; i < (UINT16)(pInfo->maxPeopleNum - 1); ++i)
//	{
//		UINT32 temp = XRandom::randInt(0, pInfo->bonusNum) + 1;
//		randPer.insert(temp);
//	}
//
//	randPer.insert(pInfo->bonusNum);
//	for (auto iter = randPer.begin(); iter != randPer.end(); ++iter)
//	{
//		if(iter == randPer.begin())
//		{
//			result.push_back(*iter);
//		}
//		else
//		{
//			auto temp = std::prev(iter);
//			result.push_back(*iter - *temp);
//		}
//	}
//
//	for (auto iter = result.begin(); iter != result.end(); ++iter)
//	{
//		if (*iter == 0)
//		{
//			resultIterZero.push_back(iter);
//		}
//		else if (* iter > 1)
//			resultIterNotZero.push_back(iter);
//	}
//
//	for (auto iter = resultIterZero.begin(); iter != resultIterZero.end(); ++iter)
//	{
//		UINT32 size = resultIterNotZero.size();
//		if (!size)
//		{
//			break;
//		}
//		UINT32 randRes = XRandom::randInt(0, size);
//		if ((-- * resultIterNotZero[randRes]) == 1)
//		{
//			resultIterNotZero.erase(resultIterNotZero.begin() + randRes);
//		}
//		(**iter) ++;
//	}
//
//	UINT32 total = 0;
//	GetBonusInfo info;
//	pInfo->getBonusList.clear();
//	for (auto iter = result.begin(); iter != result.end(); iter ++)
//	{
//		info.getNum = *iter;
//		total += *iter;
//		pInfo->getBonusList.push_back(info);
//	}
//
//	if (total != pInfo->bonusNum)
//	{
//		pInfo->getBonusList.clear();
//		return;
//	}
//}

void GuildBonusMgr::_generateBonusRandomNum(GuildBonusInfo* pInfo)
{
	if (!pInfo || !pInfo->maxPeopleNum || !pInfo->bonusNum)
	{
		return;
	}
	pInfo->getBonusList.clear();
	if (pInfo->bonusNum < pInfo->maxPeopleNum)
	{
		return;
	}

	int left = pInfo->bonusNum;
	int peoplenum = pInfo->maxPeopleNum;
	for (int i = 0; i < pInfo->maxPeopleNum; ++i, --peoplenum)
	{
		GetBonusInfo info;
		if (i == (pInfo->maxPeopleNum - 1))
		{
			info.getNum = left;
			pInfo->getBonusList.push_back(info);
			break;
		}
		int max = left * 2 / peoplenum;
		int temp = XRandom::randInt(1, max);
		info.getNum = temp;
		pInfo->getBonusList.push_back(info);
		left -= temp;
	}
	if (left <= 0)
	{
		pInfo->getBonusList.clear();
		LogError("bonus error, guild:%llu, bonus:%u, contenttype:%u", m_pGuild->GetID(), pInfo->bonusID, pInfo->templateID);
	}
}

const GuildBonusInfo* GuildBonusMgr::addGuildBonus(UINT32 templateID, UINT64 roleID)
{
	auto data = CGuildConfig::Instance()->GetBonusTemplateInfo(templateID);
	if (!data)
	{
		return NULL;
	}

	int rewardType = data->GuildBonusReward.seq[0];
	int rewardNum = data->GuildBonusReward.seq[1];
	int maxPeopleNum = _GetBonusSendPeopleNum(data);
	if (data->GuildBonusType == KKSG::GBONUS_CHECKIN)
	{
		int onlinenum = m_pGuild->GetOnLineNum();
		int addper = 0;
		foreach(i in GetGlobalConfig().GuildBonusOnLineNumAddPercent)
		{
			if (onlinenum >= i->seq[0])
			{
				addper = i->seq[1];
				continue;
			}

			break;
		}

		rewardNum += rewardNum * addper / 100;
	}
	return _addGuildBonus(roleID, data->GuildBonusName, rewardType, maxPeopleNum, rewardNum, data->GuildBonusID, data->GuildBonusIsManualSend, string(""), data->GuildBonusIsRandomAccess, data->GuildBonusType);
}

void GuildBonusMgr::addGuildBonus(UINT32 type, UINT32 var , UINT64 roleID)
{
	auto data = CGuildConfig::Instance()->GetBonusTemplateInfo(type,var);
	if (!data)
	{
		return;
	}

	int rewardType = data->GuildBonusReward.seq[0];
	int rewardNum = data->GuildBonusReward.seq[1];
	int maxPeopleNum = _GetBonusSendPeopleNum(data);
	_addGuildBonus(roleID, data->GuildBonusName, rewardType, maxPeopleNum, rewardNum, data->GuildBonusID, data->GuildBonusIsManualSend, string(""), data->GuildBonusIsRandomAccess, data->GuildBonusType);
}

const GuildBonusInfo* GuildBonusMgr::_addGuildBonus(UINT64 roleID, const std::string& sourceName, UINT8 bonusType, UINT16 maxPeopleNum, UINT32 bonusNum, UINT32 templateID, bool manualSend, const std::string& content, bool isRandomDispatch, UINT32 guildBonusType)
{
	if (!bonusNum || !maxPeopleNum || bonusNum < maxPeopleNum)
	{
		return NULL;
	}

	int now = time(0);
	GuildBonusInfo temp;
	temp.bonusID = _getNextBonusID();
	temp.bonusType = bonusType;
	temp.maxPeopleNum = maxPeopleNum;
	temp.bonusNum = bonusNum;
	//temp.leftOpenTime = BONUS_GET_TIME;
	//temp.leftBringBackTime = BONUS_BRINGBACK_TIME;
	temp.templateID = templateID;
	if (roleID)
	{
		CRoleSummary* pCRole = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
		if (pCRole)
		{
			temp.sourceCRoleID = pCRole->GetID();
			temp.source = pCRole->GetName();
		}
	}
	else
	{
		temp.sourceCRoleID = 0;
		temp.source = sourceName;
	}
	
	temp.content = content;

	if(isRandomDispatch)
		_generateBonusRandomNum(&temp);
	else
		_generateBonusAverageNum(&temp);
	
	if (guildBonusType == KKSG::GBONUS_TIMEBONUS)
	{
		NoticeMgr::Instance()->SendGuildBonusNoticeOnTime(m_pGuild->GetID());
	}

	if (manualSend)
	{
		if (!temp.sourceCRoleID)
		{
			LogError("manual send bonus content type :%u, no roleid.", templateID);
			return NULL;
		}
		temp.getTime = now;
		m_manualSendBonusList.insert(std::make_pair(temp.bonusID, temp));
		m_isModified.Set();

		CRole* role = CRoleManager::Instance()->GetByRoleID(temp.sourceCRoleID);
		_NotifySendBonus(true, role);

		return NULL;
	}
	else
	{
		_InsertIntoBonusList(temp);

		auto find = m_bonusList.find(temp.bonusID);
		if (find != m_bonusList.end())
		{
			return &find->second;
		}

		return NULL;
	}

}

void GuildBonusMgr::_addGuildBonus(const GuildBonusInfo& bonusInfo)
{
	if (!bonusInfo.bonusID)
	{
		return;
	}
	auto find = m_bonusList.find(bonusInfo.bonusID);
	if (find != m_bonusList.end())
	{
		LogError("bonus duplicate:%u, %llu, %u", bonusInfo.bonusID, bonusInfo.sourceCRoleID, bonusInfo.templateID);
		return;
	}

	_InsertIntoBonusList(bonusInfo);
}

void GuildBonusMgr::_CheckAutoSendBonusInSendList()
{
	int now = time(0);
	std::set<UINT64> roleIDSet;
	for (auto iter = m_manualSendBonusList.begin(); iter != m_manualSendBonusList.end();)
	{
		if (now - iter->second.getTime > 86400)
		{
			roleIDSet.insert(iter->second.sourceCRoleID);
			_SendBonusInSendList(&iter->second);
			m_manualSendBonusList.erase(iter ++);
		}
		else
			++iter;
	}

	for (auto iter = roleIDSet.begin(); iter != roleIDSet.end(); ++iter)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(*iter);
		if (role)
		{
			if (!_CheckHasSendBonus(role))
			{
				_NotifySendBonus(false, role);
			}
		}
	}
}

void GuildBonusMgr::_SendBonusInSendList(const GuildBonusInfo* info)
{
	if (!info)
	{
		return;
	}

	_addGuildBonus(*info);
}

void GuildBonusMgr::_RemoveExceedBonus()
{
	UINT32 minTime = -1;
	bool change = false;
	std::map<UINT32,GuildBonusInfo>::iterator minTimeIter = m_bonusList.end();
	for (auto iter = m_bonusList.begin(); iter != m_bonusList.end();)
	{
		if (0 == _getLeftOpenTime(iter->second.sendTime, iter->second.templateID))
		{
			change = true;
			m_bonusList.erase(iter++);
		}
		else
		{
			if (iter->second.sendTime < minTime)
			{
				minTime = iter->second.sendTime;
				minTimeIter = iter;
			}
			++iter;
		}
	}
	if (m_bonusList.size() > BONUS_MAXNUM)
	{
		if (minTimeIter != m_bonusList.end())
		{
			change = true;
			LogWarn("delete bonus:%u, %u, leftopen:%u,%u", minTimeIter->second.bonusID, minTimeIter->second.templateID, _getLeftOpenCount(&minTimeIter->second), minTime);
			m_bonusList.erase(minTimeIter);
		}
	}
	if (change)
	{
		m_isModified.Set();
	}
}

void GuildBonusMgr::onLogin(CRole* pCRole)
{
	if (!pCRole)
	{
		return;
	}

	bool hasNewBonus = false;
	bool hasCheckInBonus = false;
	PtcM2G_MNewGuildBonusNtf oNewGuildBonusPtc;
	for (auto iter = m_bonusList.begin(); iter != m_bonusList.end(); ++iter)
	{
		if (!_getLeftOpenTime(iter->second.sendTime, iter->second.templateID))
		{
			continue;
		}
		if (!_getLeftOpenCount(&iter->second))
		{
			continue;
		}
		size_t i = 0;
		for (; i < iter->second.getBonusList.size();  ++i)
		{
			if (iter->second.getBonusList[i].roleID == pCRole->GetID())
			{
				break;
			}
		}
		if (i == iter->second.getBonusList.size())
		{
			if (_IsCheckInBonus(iter->second.templateID))
				hasCheckInBonus = true;
			else
				hasNewBonus = true;
			//break;
			oNewGuildBonusPtc.m_Data.add_bonustemplateids(iter->second.templateID);
		}
	}
	if (hasNewBonus || hasCheckInBonus)
	{
		oNewGuildBonusPtc.m_Data.set_roleid(pCRole->GetID());
		oNewGuildBonusPtc.m_Data.set_hascheckinbonus(hasCheckInBonus);
		oNewGuildBonusPtc.m_Data.set_hasnormalbonus(hasNewBonus);
		GSLink::Instance()->SendToLine(pCRole->GetGsLine(), oNewGuildBonusPtc);
	}

	if (_CheckHasSendBonus(pCRole))
	{
		_NotifySendBonus(true, pCRole);
	}
}

UINT32 GuildBonusMgr::_getLeftOpenCount(GuildBonusInfo* info)
{
	UINT32 num = 0;
	if (!info)
	{
		return 0;
	}
	for (size_t i = 0; i < info->getBonusList.size(); ++i)
	{
		if (info->getBonusList[i].roleID)
		{
			continue;
		}
		num++;
	}

	return num;
}

UINT32 GuildBonusMgr::_getLeftOpenTime(UINT32 sendTime, UINT32 templateID)
{
	auto data = CGuildConfig::Instance()->GetBonusTemplateInfo(templateID);
	if (!data)
	{
		return 0;
	}
	UINT32 lastTime = data->GuildBonusOpenTime;
	UINT32 cur = (UINT32)time(0);
	UINT32 leftOpenTime = cur < (sendTime + lastTime) ? (sendTime + lastTime - cur) : 0;
	return leftOpenTime;
}

UINT32 GuildBonusMgr::_getLeftBringBackTime(UINT32 sendTime, UINT32 templateID)
{
	auto data = CGuildConfig::Instance()->GetBonusTemplateInfo(templateID);
	if (!data)
	{
		return 0;
	}
	UINT32 lastTime = data->GuildBonusOpenTime;
	UINT32 cur = (UINT32)time(0);
	UINT32 leftBringBackTime = cur < (sendTime + lastTime + BONUS_BRINGBACK_TIME) ? (sendTime + lastTime + BONUS_BRINGBACK_TIME - cur) : 0;
	return leftBringBackTime;
}

void GuildBonusMgr::getBonusList(CRole* pCRole, KKSG::GetGuildBonusListResult &roRes, KKSG::GuildRecord* record)
{
	if(!pCRole)
		return;

	for (auto iter = m_bonusList.begin(); iter != m_bonusList.end(); ++iter)
	{
		if (!_getLeftOpenTime(iter->second.sendTime,iter->second.templateID))
		{
			continue;
		}
		KKSG::GuildBonusAppear* pAppear = roRes.add_bonuslist();
		_fillBonusAppear(pCRole, pAppear, &iter->second, record);
	}

}

UINT32 GuildBonusMgr::_getAlreadyGetSizeOfBonus(GuildBonusInfo* pInfo)
{
	if (!pInfo)
	{
		return 0;
	}
	UINT32 get = 0;
	for (auto iter = pInfo->getBonusList.begin(); iter != pInfo->getBonusList.end(); ++iter)
	{
		if (iter->roleID)
		{
			++get;
		}
	}
	return get;
}

void GuildBonusMgr::_fillBonusAppear(CRole* pCRole, KKSG::GuildBonusAppear* dest, GuildBonusInfo* src, KKSG::GuildRecord* record)
{
	if (!pCRole || !dest || !src)
	{
		return;
	}
	UINT32 cur = (UINT32)time(0);
	UINT32 sendTime = src->sendTime;
	UINT32 leftOpenTime = _getLeftOpenTime(sendTime,src->templateID);
	UINT32 leftBringBackTime = _getLeftBringBackTime(sendTime,src->templateID);
	
	dest->set_bonusid(src->bonusID);
	dest->set_bonustype(src->bonusType);
	dest->set_maxpeoplenum(src->maxPeopleNum);
	dest->set_leftopentime(leftOpenTime);
	dest->set_leftbringbacktime(leftBringBackTime);
	dest->set_sourcename(src->source);
	dest->set_alreadygetpeoplenum(_getAlreadyGetSizeOfBonus(src));

	dest->set_bonusstatus(_GetBonusStatus(pCRole, src, record));

	dest->set_bonuscontenttype(src->templateID);
	dest->set_sourceid(src->sourceCRoleID);

	if (src->sourceCRoleID)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(src->sourceCRoleID);
		if (summary)
		{
			dest->set_sourcename(summary->GetName());

			const PlatUserInfo* platSelfInfo = CPlatformFriendMgr::Instance()->GetPlatSelfInfo(summary->GetAccount());
			if (platSelfInfo)
			{
				dest->set_iconurl(platSelfInfo->bigpic);
			}
		}
	}
}

void GuildBonusMgr::getBonusDetailInfo(CRole* pCRole, UINT32 bonusID, KKSG::GetGuildBonusDetailInfoResult &roRes, KKSG::GuildRecord* record)
{
	if(!pCRole)
		return;

	UINT32 cur = (UINT32)time(0);
	auto find = m_bonusList.find(bonusID);
	auto end = m_bonusList.end();
	if (find == end)
	{
		return;
	}
	KKSG::GuildBonusAppear* pAppear = roRes.mutable_bonusinfo();
	_fillBonusAppear(pCRole, pAppear, &find->second, record);

	std::string* content = roRes.mutable_content();
		
	roRes.set_bonusnum(find->second.bonusNum);
	roRes.set_bonuscontenttype(find->second.templateID);
	roRes.set_leaderid(find->second.leaderID);
	roRes.set_luckestid(find->second.luckestID);
	roRes.set_canthank(false);

	for (auto iterList = find->second.getBonusList.begin(); iterList != find->second.getBonusList.end(); ++iterList)
	{
		if (!iterList->roleID)
		{
			continue;
		}
		KKSG::GetGuildBonusInfo* info = roRes.add_getbonusrolelist();
		info->set_roleid(iterList->roleID);
		info->set_rolename(iterList->roleName);
		info->set_getnum(iterList->getNum);
		info->set_gettime(cur - iterList->getTime);
		if (iterList->roleID == pCRole->GetID() && iterList->canThank)
		{
			roRes.set_canthank(true);
		}
	}

}

bool GuildBonusMgr::_checkGetBonusReward(CRole* pCRole, GuildBonusInfo& info)
{
	if (!pCRole)
	{
		return false;
	}
	for (auto iter = info.getBonusList.begin(); iter != info.getBonusList.end(); ++iter)
	{
		if (iter->roleID == pCRole->GetID())
		{
			return true;
		}
	}

	return false;
}

UINT32 GuildBonusMgr::_checkNotGetMaxNum(UINT32 bonusTemplateID, KKSG::GuildRecord* record)
{
	auto data = CGuildConfig::Instance()->GetBonusTemplateInfo(bonusTemplateID);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	if (!record->has_bonusdata())
	{
		return KKSG::ERR_SUCCESS;
	}

	if (data->ForeverMaxNum)
	{
		for (int i = 0; i < record->bonusdata().gotguildbonustotalnum_size(); ++i)
		{
			auto getInfo = record->bonusdata().gotguildbonustotalnum(i);
			if (getInfo.key() == bonusTemplateID)
			{
				if (getInfo.value() >= data->ForeverMaxNum)
				{
					return KKSG::ERR_FAILED;
				}

				break;
			}
		}
	}
	if (data->DayMaxNum)
	{
		for (int i = 0; i < record->bonusdata().gotguildbonusdaynum_size(); ++i)
		{
			auto getInfo = record->bonusdata().gotguildbonusdaynum(i);
			if (getInfo.key() == bonusTemplateID)
			{
				if (getInfo.value() >= data->DayMaxNum)
				{
					return KKSG::ERR_FAILED;
				}

				break;
			}
		}
	}

	return KKSG::ERR_SUCCESS;
}

bool GuildBonusMgr::_getRandReward(GuildBonusInfo* info, UINT32& result)
{
	//std::vector<UINT32> index;
	for (size_t i = 0; i < info->getBonusList.size(); ++i)
	{
		if (info->getBonusList[i].roleID)
		{
			continue;
		}
		//index.push_back(i);
		result = i;
		return true;
	}

	return false;

	//if (index.empty())
	//{
	//	return false;
	//}

	//result = index[XRandom::randInt(0, index.size())];
	//return true;
}


void GuildBonusMgr::_OnGetBonusReward(CRole* pRole, const GuildBonusInfo& info)
{
	TGuildBonusFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_iOp = 1;
	oLog.m_uRoleID = pRole->GetID();
	oLog.oBonusInfo = info;
	oLog.m_iGetCount = _getLeftOpenCount((GuildBonusInfo*)&info);
	oLog.m_iGetAwardNum = _GetMemberGotNum((GuildBonusInfo*)&info, pRole->GetID());
	oLog.Do();
}

void GuildBonusMgr::_OnBonusGetAll(CRole* pRole, const GuildBonusInfo& info)
{
	PtcM2C_GuildBonusGetAll ntf;
	ntf.m_Data.set_bonusid(info.bonusID);
	m_pGuild->sendCmdToAllMember(ntf);
}

void GuildBonusMgr::getBonusReward(CRole* pCRole, UINT32 bonusID, KKSG::MGuildBonusOperateReqRes &roRes, KKSG::GuildRecord* record)
{
	if(!pCRole)
		return;


	auto find = m_bonusList.find(bonusID);
	if (find == m_bonusList.end())
	{
		roRes.set_error(KKSG::ERR_GUILDBONUS_NOTEXIST);
		return;
	}

	UINT32 status = _GetBonusStatus(pCRole, &find->second, record);
	if(status != BONUS_CAN_GET)
	{
		if (status == BONUS_ALL_GET)
		{
			roRes.set_error(KKSG::ERR_GUILDBONUS_ALLGET);
			return;
		}
		else if (status == BONUS_CANNOT_GET)
		{
			roRes.set_error(KKSG::ERR_GUILDBONUS_MAXNUM);
			return;
		}
		roRes.set_error(KKSG::ERR_GUILDBONUS_ALREADYGET);
		return;
	}
	/*if (checkGetBonusReward(pCRole, find->second))
	{
		roRes.set_errorcode(KKSG::ERR_GUILDBONUS_ALREADYGET);
		return;
	}*/


	UINT32 cur = (UINT32)time(0);
	UINT32 sendTime = find->second.sendTime;
	UINT32 leftOpenTime = _getLeftOpenTime(sendTime,find->second.templateID);

	if (!leftOpenTime)
	{
		roRes.set_error(KKSG::ERR_GUILDBONUS_EXCEED);
		return;
	}

	UINT32 index = 0;
	if (!_getRandReward(&find->second,index))
	{
		return;
	}

	GetBonusInfo* pInfo = &find->second.getBonusList[index];
	pInfo->roleID = pCRole->GetID();
	pInfo->roleName = pCRole->GetName();
	pInfo->getTime = (UINT32)time(NULL);

	roRes.mutable_getbonusreward()->set_getvalue(pInfo->getNum);

	if(_IsCheckInBonus(find->second.templateID))
	{
		roRes.set_ischeckinbonus(true);
	}

	roRes.set_bonustemplateid(find->second.templateID);

	bool getDouble = false;
	/*if (!find->second.leaderID && m_pGuild->GetLeaderID() == pCRole->GetID())
	{
		find->second.leaderID = pCRole->GetID();
		getDouble = true;
	}*/

	m_isModified.Set();

	roRes.set_realgetrewardnum(getDouble ? pInfo->getNum * 2 : pInfo->getNum);
	roRes.set_bonustype(find->second.bonusType);

	roRes.set_error(KKSG::ERR_SUCCESS);
	
	UINT32 itemID = find->second.bonusType;
	UINT32 count = getDouble ? pInfo->getNum * 2 : pInfo->getNum;

	int leftcount = _getLeftOpenCount(&find->second);
	if(!leftcount)
	{
		find->second.luckestID = _CalcLuckestID(&find->second);
		_OnBonusGetAll(pCRole, find->second);
	}
	CRoleSummary* leader = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pGuild->GetLeaderID());
	CRoleSummary* tempSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pCRole->GetID()); 

	std::string sourceName = find->second.source;
	CRoleSummary* srcSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(find->second.sourceCRoleID); 
	if (srcSummary)
	{
		sourceName = srcSummary->GetName();
	}

	NoticeMgr::Instance()->GetGuildBonusNotice(leader, tempSummary, m_pGuild->GetID(), itemID, count);
	if (leftcount == (find->second.maxPeopleNum - 1))
	{
		NoticeMgr::Instance()->FirstGetGuildBonusNotice(/*find->second.source*/sourceName, tempSummary, m_pGuild->GetID() , itemID, count);
	}
	if (!leftcount && (time(0) - find->second.sendTime) < GetGlobalConfig().GuildBonusNoticeGetAllTime)
	{
		bool isLuckLeader = false;
		if (find->second.luckestID == m_pGuild->GetLeaderID())
		{
			isLuckLeader = true;
		}
		int num = _GetMemberGotNum(&find->second, find->second.luckestID);
		UINT32 luckNum = isLuckLeader ? num * 2 : num;
		//auto data = CGuildConfig::Instance()->getBonusTypeInfo(find->second.templateID);
		//if (data)
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(find->second.luckestID);
			NoticeMgr::Instance()->LuckestGetGuildBonusNotice(leader, /*data->GuildBonusName*//*find->second.source*/sourceName, summary, itemID, luckNum, m_pGuild->GetID());
		}
	}

	_OnGetBonusReward(pCRole, find->second);
}

void GuildBonusMgr::GetBonusLeftList(CRole* pCRole, KKSG::MGuildBonusOperateReqRes &roRes, KKSG::GuildRecord* record)
{
	if(!pCRole)
		return;

	KKSG::GetGuildBonusLeftRes* leftRes = roRes.mutable_leftbonus();
	for (auto iter = m_bonusList.begin(); iter != m_bonusList.end(); ++iter)
	{
		UINT32 status = _GetBonusStatus(pCRole, &iter->second, record);
		if(status != BONUS_CAN_GET)
		{
			continue;
		}
		int leftopentime = _getLeftOpenTime(iter->second.sendTime, iter->second.templateID);
		if (!leftopentime)
		{
			continue;
		}
		KKSG::GuildBonusBriefInfo* info = leftRes->add_bonusinfos();
		info->set_bonusid(iter->second.bonusID);
		info->set_sendtime(iter->second.sendTime);
	}

}

UINT64 GuildBonusMgr::_CalcLuckestID(const GuildBonusInfo* info)
{
	if (!info)
	{
		return 0;
	}
	int max = 0;
	UINT64 maxCRoleID = 0;
	foreach(i in info->getBonusList)
	{
		if (!i->roleID)
		{
			return 0;
		}
		if (i->getNum > max)
		{
			max = i->getNum;
			maxCRoleID = i->roleID;
		}
	}

	return maxCRoleID;
}

GuildBonusInfo* GuildBonusMgr::GetBonusInfoByID(UINT32 bonusID)
{
	auto find = m_bonusList.find(bonusID);
	if (find == m_bonusList.end())
	{
		return NULL;
	}

	return &find->second;
}

bool GuildBonusMgr::onTimer()
{
	Update();
	return true;
}

void GuildBonusMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	_CheckAutoSendBonusInSendList();

	_RemoveExceedBonus();
}

void GuildBonusMgr::Update()
{
	UINT32 day = XCommon::GetTodayUnique();
	if (m_updateDay != day)
	{
		_GenerateCheckInBonus();
		m_updateDay = day;

		_RemoveExceedBonus();
	}
}

void GuildBonusMgr::_GenerateCheckInBonus()
{
	m_checkinBonusData.clear();
	GuildConf* conf = CGuildConfig::Instance()->GetGuildConf(m_pGuild->GetLevel());
	if (conf)
	{
		foreach(i in conf->GuildCheckInBonus)
		{
			GuildCheckInBonus info;
			info.needCheckInNum = (*i).seq[0];
			info.templateID = (*i).seq[1];
			m_checkinBonusData.push_back(info);
		}
	}
	else
		LogError("guild level error:%llu,%s,%u", m_pGuild->GetID(), m_pGuild->GetName().c_str(), m_pGuild->GetLevel());

	if (m_checkinBonusData.size())
	{
		m_isModified.Set();
	}

	LogInfo("guild checkin bonus :%llu,%s,%u,%u", m_pGuild->GetID(), m_pGuild->GetName().c_str(), m_checkinBonusData.size(), m_pGuild->GetLevel());
}

UINT32 GuildBonusMgr::_GetBonusStatus(CRole* role, GuildBonusInfo* info, KKSG::GuildRecord* record)
{
	if (!role || !info)
	{
		return 0;
	}

	if (info->maxPeopleNum == _getAlreadyGetSizeOfBonus(info))
	{
		return BONUS_ALL_GET;
	}

	if (_checkGetBonusReward(role, *info))
	{
		return BONUS_ALREADY_GET;
	}

	if (!record)
	{
		return 0;
	}

	if (record->getcheckinbonusnum() >= GetGlobalConfig().MaxGetGuildCheckInBonusNum && _IsCheckInBonus(info->templateID))
	{
		return BONUS_CANNOT_GET;
	}
	if (_checkNotGetMaxNum(info->templateID, record) != KKSG::ERR_SUCCESS)
	{
		return BONUS_CANNOT_GET;
	}

	return BONUS_CAN_GET;
}

UINT32 GuildBonusMgr::GetCheckInBonusInfo(CRole* role, KKSG::GuildCheckInBonusInfoRes &roRe, KKSG::GuildRecord* record)
{
	CGuildCheckin* checkin = m_pGuild->Get<CGuildCheckin>();
	roRe.set_checkinnum(checkin->GetCount());
	roRe.set_onlinenum(m_pGuild->GetOnLineNum());
	roRe.set_guildmembernum(m_pGuild->GetMemberCount());
	foreach(i in m_checkinBonusData)
	{
		KKSG::GuildBonusAppear* appear = roRe.add_checkinbonusinfo();
		appear->set_bonusid(i->bonusID);
		appear->set_needcheckinnum(i->needCheckInNum);
		GuildBonusInfo* info = GetBonusInfoByID(i->bonusID);
		if (info)
		{
			appear->set_bonusstatus(_GetBonusStatus(role, info, record));
		}
		else
			appear->set_bonusstatus(0);
		const GuildBonusTable::RowData* data = CGuildConfig::Instance()->GetBonusTemplateInfo(i->templateID);
		if (data)
		{
			appear->set_maxpeoplenum(_GetBonusSendPeopleNum(data));
		}
		else
			appear->set_maxpeoplenum(0);
		appear->set_bonuscontenttype(i->templateID);
		appear->set_bonustype(0);
		appear->set_sourcename("");
		appear->set_alreadygetpeoplenum(0);
	}

	int now = time(0);
	roRe.set_timeofday(now - XCommon::GetDayBeginTime(now, false));

	roRe.set_errorcode(KKSG::ERR_SUCCESS);

	return KKSG::ERR_SUCCESS;
}

bool GuildBonusMgr::IsInCheckInBonusTime()
{
	auto conf = GetGlobalConfig().GuildCheckInBonusTime;
	if (conf.size() != 2)
	{
		return false;
	}

	time_t begin = XCommon::GetTodayBeginTime(false);
	UINT32 span = time(0) - begin;
	if (span >= (conf[0].seq[0] * 3600 + conf[0].seq[1] * 60) && span <= (conf[1].seq[0] * 3600 + conf[1].seq[1] * 60))
	{
		return true;
	}

	return false;
}

UINT32 GuildBonusMgr::SendCheckInBonus(CRole* role)
{
	GuildMember *pMember = m_pGuild->GetMember(role->GetID());
	if (!pMember)
	{
		return KKSG::ERR_FAILED;
	}
	if (!CGuildConfig::Instance()->HasPermission(pMember->position, GPEM_SENDCHECKINBONUS))
	{
		return KKSG::ERR_GUILD_NO_PERMISSION;
	}

	if (!IsInCheckInBonusTime())
	{
		return KKSG::ERR_GUILD_CHECKINBONUS_TIMEERROR;
	}

	bool send = false;
	
	int checkInNum = m_pGuild->Get<CGuildCheckin>()->GetCount();
	foreach(i in m_checkinBonusData)
	{
		if (i->bonusID)
		{
			continue;
		}

		if (checkInNum < i->needCheckInNum)
		{
			return KKSG::ERR_FAILED;
		}

		const GuildBonusInfo* info = addGuildBonus(i->templateID, role->GetID());
		if (!info)
		{
			return KKSG::ERR_FAILED;
		}

		i->bonusID = info->bonusID;

		return KKSG::ERR_SUCCESS;
	}

	return KKSG::ERR_FAILED;
}

void GuildBonusMgr::_InsertIntoBonusList(const GuildBonusInfo& bonusInfo)
{
	m_bonusList.insert(std::make_pair(bonusInfo.bonusID, bonusInfo));
	m_bonusList[bonusInfo.bonusID].sendTime = time(0);

	m_isModified.Set();

	_NotifyAddNewBonus(&m_bonusList[bonusInfo.bonusID]);

	_OnAddNewBonus(m_bonusList[bonusInfo.bonusID]);

	_RemoveExceedBonus();
}

void GuildBonusMgr::_OnAddNewBonus(const GuildBonusInfo& bonusInfo)
{
	TGuildBonusFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_iOp = 0;
	if (bonusInfo.sourceCRoleID)
	{
		oLog.m_uRoleID = bonusInfo.sourceCRoleID;
	}
	oLog.oBonusInfo = bonusInfo;
	oLog.Do();
}

void GuildBonusMgr::_NotifyAddNewBonus(const GuildBonusInfo* info)
{
	if (!info)
	{
		return;
	}

	bool isCheckIn = _IsCheckInBonus(info->templateID);
	PtcM2G_MNewGuildBonusNtf oNewGuildBonusPtc;
	oNewGuildBonusPtc.m_Data.set_guildid(m_pGuild->GetID());
	if (isCheckIn)
		oNewGuildBonusPtc.m_Data.set_hascheckinbonus(true);
	else
		oNewGuildBonusPtc.m_Data.set_hasnormalbonus(true);
	oNewGuildBonusPtc.m_Data.set_showiconinscreen(true);
	oNewGuildBonusPtc.m_Data.add_bonustemplateids(info->templateID);
	const GuildBonusTable::RowData* data = CGuildConfig::Instance()->GetBonusTemplateInfo(info->templateID);
	if (data && data->GuildBonusIsManualSend)
	{
		oNewGuildBonusPtc.m_Data.add_excluderoleid(info->sourceCRoleID);
	}
	GSLink::Instance()->SendToAllLine(oNewGuildBonusPtc);
}

UINT32 GuildBonusMgr::ThankForBonus(CRole* role, UINT32 bonusID)
{
	auto find = m_bonusList.find(bonusID);
	if (find == m_bonusList.end())
	{
		return KKSG::ERR_FAILED;
	}
	int leftopentime = _getLeftOpenTime(find->second.sendTime, find->second.templateID);
	if (!leftopentime)
	{
		return KKSG::ERR_FAILED;
	}

	if (!_checkGetBonusReward(role, find->second))
		return KKSG::ERR_FAILED;

	foreach(i in find->second.getBonusList)
	{
		if (i->roleID == role->GetID())
		{
			if (!i->canThank)
				return KKSG::ERR_FAILED;
			i->canThank = false;
			m_isModified.Set();
			break;
		}
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 GuildBonusMgr::_GetMemberGotNum(const GuildBonusInfo* info, UINT64 roleID)
{
	if (!info)
	{
		return 0;
	}

	foreach(i in info->getBonusList)
	{
		if (i->roleID == roleID)
		{
			return i->getNum;
		}
	}

	return 0;
}

bool GuildBonusMgr::_CheckHasSendBonus(CRole* role)
{
	if (!role)
	{
		return false;
	}
	bool has = false;
	for (auto iter = m_manualSendBonusList.begin(); iter != m_manualSendBonusList.end(); ++iter)
	{
		if (iter->second.sourceCRoleID == role->GetID())
		{
			has = true;
			break;
		}
	}

	return has;
}

void GuildBonusMgr::_NotifySendBonus(bool hasLeftSend, CRole* role)
{
	if (!role)
	{
		return;
	}

	PtcM2C_SendGuildBonusNtf ntf;
	ntf.m_Data.set_hasleftsend(hasLeftSend);

	role->Send(ntf);
}

UINT32 GuildBonusMgr::ManualSendBonusInSendList(CRole* role, UINT32 bonusID)
{
	if (!role)
	{
		return KKSG::ERR_FAILED;
	}
	auto find = m_manualSendBonusList.find(bonusID);
	if (find == m_manualSendBonusList.end())
	{
		return KKSG::ERR_FAILED;
	}

	if (find->second.sourceCRoleID != role->GetID())
	{
		return KKSG::ERR_FAILED;
	}

	_SendBonusInSendList(&find->second);

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
	NoticeMgr::Instance()->SendGuildBonusNotice(summary, find->second.bonusType, find->second.bonusNum, m_pGuild->GetID());

	m_manualSendBonusList.erase(find);

	if (!_CheckHasSendBonus(role))
	{
		_NotifySendBonus(false, role);
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 GuildBonusMgr::GetGuildBonusInSendList(CRole* role, KKSG::GetGuildBonusSendListRes& roRes)
{
	for (auto iter = m_manualSendBonusList.begin(); iter != m_manualSendBonusList.end(); ++iter)
	{
		if (role->GetID() == iter->second.sourceCRoleID && strcmp(role->GetName().c_str(), iter->second.source.c_str()) != 0)
		{
			iter->second.source.assign(role->GetName());
			m_isModified.Set();
		}
		auto info = roRes.add_sendlist();
		_fillBonusAppear(role, info, &iter->second, NULL);
	}

	return KKSG::ERR_SUCCESS;
}

bool GuildBonusMgr::_IsCheckInBonus(UINT32 templateID)
{
	const GuildBonusTable::RowData* data = CGuildConfig::Instance()->GetBonusTemplateInfo(templateID);
	if (data && data->GuildBonusType == KKSG::GBONUS_CHECKIN)
		return true;

	return false;
}

UINT32 GuildBonusMgr::_GetBonusSendPeopleNum(const GuildBonusTable::RowData* data)
{
	if (!data)
	{
		return 0;
	}

	if (!data->GuildBonusMaxPeopleNum)
	{
		return m_pGuild->GetMemberCount();
	}

	return data->GuildBonusMaxPeopleNum;
}

UINT32 GuildBonusMgr::GetLastCheckinBonusTime()
{
	UINT32 t = 0;
	for (auto it = m_bonusList.begin(); it != m_bonusList.end(); ++it)
	{
		if (!_IsCheckInBonus(it->second.templateID))
		{
			continue;
		}
		if (it->second.sendTime > t)
		{
			t = it->second.sendTime;
		}
	}
	return t;
}

bool GuildBonusMgr::IsCheckInBonusAllGot()
{
	if (m_checkinBonusData.empty())
	{
		return false;
	}

	bool isAllSendAndGot = true;
	for (auto iter = m_checkinBonusData.begin(); iter != m_checkinBonusData.end(); ++iter)
	{
		if (!iter->bonusID)
		{
			isAllSendAndGot = false;
			break;
		}
		auto find = m_bonusList.find(iter->bonusID);
		if (find != m_bonusList.end())
		{
			if (_getLeftOpenCount(&find->second))
			{
				isAllSendAndGot = false;
				break;
			}
		}
	}

	return isAllSendAndGot;
}
