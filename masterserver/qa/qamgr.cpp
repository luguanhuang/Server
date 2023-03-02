#include "pch.h"
#include "qamgr.h"
#include "define/qadef.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "qa/ptcm2g_updateguildqaroom.h"
#include "network/gslink.h"
#include "notice/noticemgr.h"
#include "table/MultActivityMgr.h"
#include "util/gametime.h"
#include "global/GlobalConfigDBBuild.h"
#include "util.h"

INSTANCE_SINGLETON(QAMgr);

QAMgr::QAMgr()
:m_timerId(0)
,m_curRoomId(1)
,m_lastUpdateTime(0)
,m_curRoom(NULL)
,m_pLogFile(NULL)
,m_nLogTime(0)
{
	m_nWeek = 0;
}

QAMgr::~QAMgr()
{

}


bool QAMgr::Init()
{
	m_timerId = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	LoadFile();
	return true;
}



bool QAMgr::CheckFile()
{
	QuestionLibraryTable tmpQuestionLibraryTable;
	if( !tmpQuestionLibraryTable.LoadFile("table/QuestionLibrary.txt"))
	{
		SSError<<"check file QuestionLibrary.txt failed"<<END;
		return false;
	}

	QAConditionTable tmpQAConditionTable;
	if( !tmpQAConditionTable.LoadFile("table/QACondition.txt") )
	{
		SSError<<"check file QACondition.txt failed"<<END;
		return false;
	}

	QAPointRankRewardTable tmpQAPointRankRewardTable;
	if( !tmpQAPointRankRewardTable.LoadFile("table/QAPointRankReward.txt") )
	{
		SSError<<"check file QAPointRankReward.txt failed"<<END;
		return false;
	}

	QALevelRewardTable tmpQALevelRewardTable;
	if( !tmpQALevelRewardTable.LoadFile("table/QALevelReward.txt") )
	{
		SSError<<"check file QALevelReward.txt failed"<<END;
		return false;
	}
	return true;
}

bool QAMgr::LoadFile()
{

	bool bRet = CheckFile();
	if (bRet==false)
	{
		return false;
	}
	ClearFile();
	m_nWeek = 0;

	if( !m_QuestionLibraryTable.LoadFile("table/QuestionLibrary.txt"))
	{
		SSError<<"load file QuestionLibrary.txt failed"<<END;
		return false;
	}
	if( !m_QAConditionTable.LoadFile("table/QACondition.txt") )
	{
		SSError<<"load file QACondition.txt failed"<<END;
		return false; 
	}
	if( !m_QAPointRankRewardTable.LoadFile("table/QAPointRankReward.txt") )
	{
		SSError<<"load file QAPointRankReward.txt failed"<<END;
		return false; 
	}
	if( !m_levelRewardTable.LoadFile("table/QALevelReward.txt") )
	{
		SSError<<"load file QALevelReward.txt failed"<<END;
		return false; 
	}

	// 题目列表 key：qatype   value：该答题类型下的所有题目编号;
	m_mapQuestionLib.clear();
	for( UINT32 i=0;  i<m_QuestionLibraryTable.Table.size(); i++ )
	{
		UINT32 qaType = m_QuestionLibraryTable.Table[i]->QAType;
		UINT32 qid = m_QuestionLibraryTable.Table[i]->ID;
		UINT32 week =  m_QuestionLibraryTable.Table[i]->Week;
		if (week>m_nWeek)
		{
			m_nWeek = week;
		}
		m_mapQuestionLib[MakeUINT64(qaType,week)].push_back(qid);
	}
	return true;
}

void QAMgr::ClearFile()
{
	m_QuestionLibraryTable.Clear();
	m_QAConditionTable.Clear();
	m_QAPointRankRewardTable.Clear();
	m_levelRewardTable.Clear();
}


void QAMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timerId);
	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
	{
		QARoom* room = it->second;
		SSInfo << "remove qaroom, id: " << room->m_id << ", type: " << room->m_type << END;
		delete room;
	}
	m_rooms.clear();
	ClearFile();

}

void QAMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Update();

}

void QAMgr::Update()
{
	UINT32 now = GameTime::GetTime();
	// 房间更新
	for (auto it = m_rooms.begin(); it != m_rooms.end(); )
	{
		QARoom* room = it->second;
		UINT32 status = room->Update();
		if (status == RoomStatus_Close)
		{
			SSInfo << "QaRoom close, id: " << room->m_id << ", type: " << room->m_type << END;
			if (room->m_type == QATYPE_GUILD)
			{
				m_guildRooms.erase(room->m_guild);
			}
			if (room == m_curRoom)
			{
				m_curRoom = NULL;
			}
			delete room;
			m_rooms.erase(it++);
		}
		else
		{
			it++;
		}
	}

	// 全民答题倒计时
	UINT32 firstQuestionTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_MULTI_QA) + GetPrepareTime(QATYPE_MULTI);
	UINT32 time1 = firstQuestionTime - 180;
	if (m_lastUpdateTime < time1 && now >= time1)
	{
		NoticeMgr::Instance()->NoticeJoinWorldQA(3);
	}
	UINT32 time2 = firstQuestionTime - 60;
	if (m_lastUpdateTime < time2 && now >= time2)
	{
		NoticeMgr::Instance()->NoticeJoinWorldQA(1);
	}

	m_lastUpdateTime = now;
}

QARoom* QAMgr::GetAvailableMultiRoom()
{
	if (NULL == m_curRoom || m_curRoom->IsFull())
	{
		m_curRoom = CreateRoom(QATYPE_MULTI);
	} 
	return m_curRoom;
}

QARoom* QAMgr::GetGuildRoom(CRole* role)
{
	if(!role) return NULL;
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if(!guild) return NULL;

	auto it = m_guildRooms.find(guild->GetID());
	if (it != m_guildRooms.end())
	{
		return it->second;
	}
	return NULL;
}

QARoom* QAMgr::EnterQARoom(CRole* role, UINT32 type, KKSG::ErrorCode& code)
{
	code = KKSG::ERR_SUCCESS;
	UINT64 roleId = role->GetID();
	SSInfo << "HandleAgree, RoleID:" << roleId << ", QAType: " << type << END;
	
	if (IsRolePlaying(roleId))
	{
		SSInfo << "Role is Already playing! roleId: " << roleId << END;
		code = KKSG::ERR_QA_ALEADY_IN_ROOM;
		return NULL;
	}

	// 建立(或者找到)房间
	QARoom* room = NULL;
	if (type == QATYPE_ONE)
	{
		room = CreateRoom(type);
	}
	else if (type == QATYPE_MULTI)
	{
		// 获得可用房间
		room = GetAvailableMultiRoom();
	}
	else
	{
		room = GetGuildRoom(role);
	}

	if (!room)
	{
		SSInfo << "CRole Enter failed, roleId: " << roleId << ", type: " << type << END;
		code = KKSG::ERR_UNKNOWN;
		return NULL;
	}

	room->Enter(role);
	m_roleId2RoomId.insert(std::make_pair(roleId, room->m_id));
	return room;
}

QARoom* QAMgr::CreateGuildQARoom(CRole* role, KKSG::ErrorCode& code)
{
	code = KKSG::ERR_SUCCESS;
	UINT32 now = GameTime::GetTime();
	UINT32 type = QATYPE_GUILD;

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if(!guild)
	{
		SSInfo << "CRole OpenGuild QA failed, roleId: " << role->GetID() << ", type: " << type << END;
		code = KKSG::ERR_UNKNOWN;
		return NULL;
	}

	if (GetGuildRoom(role))
	{
		LogError("Alreay Have Guild QA Room, RoleID:%llu, GuildID:%llu", role->GetID(), guild->GetID());
		code = KKSG::ERR_UNKNOWN;
		return NULL;
	}

	// 不是会长，也不是副会长，则不能开启公会答题
	if (!guild->isLeader(role->GetID()) && !guild->isViceLeader(role->GetID()))
	{
		code = KKSG::ERR_GUILD_NO_PERMISSION;
		return NULL;
	}

	//创建一个公会房间;
	QARoom* room = CreateRoom(type);
	if(!room)
	{
		code = KKSG::ERR_UNKNOWN;
		return NULL;
	}
	room->m_guild = guild->GetID();
	m_guildRooms.insert(std::make_pair(room->m_guild, room));

	SSInfo << "Create Guild Room Success, RoleID:" << role->GetID() << ", RoomID:" << room->m_id << END;

	// 公会频道通知
	NoticeMgr::Instance()->NoticeGuildLeaderOpenQA(guild->GetID());

	// 通知所有GS
	PtcM2G_UpdateGuildQARoom ntf;
	ntf.m_Data.set_guild_id(guild->GetID());
	ntf.m_Data.set_status(room->m_status);
	ntf.m_Data.set_room_id(room->m_id);
	GSLink::Instance()->SendToAllLine(ntf);

	return room;
}

UINT32 QAMgr::GetCount(UINT32 nType)
{
	std::string szData = CGlobalConfigDBBuild::Instance()->GetConfigValue(eQa);
	KKSG::QaBaseList info;
	info.ParseFromString(szData);
	UINT32 dataCount = 0;
	UINT32 dataDay  = 0;
	UINT32 curday      = GameTime::GetTodayUnique();
	UINT32 dataType = nType;

	if (QATYPE_ONE == nType&&info.has_one())
	{
		dataCount = info.one().count();
		dataDay   = info.one().time();
	}else if (QATYPE_MULTI == nType&&info.has_multi())
	{
		dataCount = info.multi().count();
		dataDay   = info.multi().time();
	}else if (QATYPE_GUILD == nType&&info.has_guild())
	{
		dataCount = info.guild().count();
		dataDay   = info.guild().time();
	}

	if (curday!=dataDay)
	{
		dataDay = curday;
		if (dataCount >= m_nWeek)
		{
			dataCount = 1;
		}else
		{
			dataCount++;
		}

		if (QATYPE_ONE == nType)
		{
			info.mutable_one()->set_count(dataCount);
			info.mutable_one()->set_time(dataDay);

		}else if (QATYPE_MULTI == nType)
		{
			info.mutable_multi()->set_count(dataCount);
			info.mutable_multi()->set_time(dataDay);
		}else if (QATYPE_GUILD == nType)
		{
			info.mutable_guild()->set_count(dataCount);
			info.mutable_guild()->set_time(dataDay);
		}

		std::string refData = "";
		info.SerializeToString(&refData);
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eQa,refData);
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eQa);
	}
	return dataCount;
}

void QAMgr::RandQuestion(UINT32 type, std::vector<UINT32>& questions, UINT32 maxNum)
{

	UINT32 dataCount = GetCount(type);
	UINT64 key = MakeUINT64(type,dataCount);
	auto it = m_mapQuestionLib.find(key);
	if (it == m_mapQuestionLib.end())
	{
		return;
	}
	if (it->second.size() <= maxNum)
	{
		questions = it->second;
		return;
	}
	questions.resize(maxNum);
	std::vector<UINT32>& source = it->second;
	UINT32 sourceSize = source.size();
	for (INT32 i = 0; i < maxNum; i++)
	{
		INT32 index = XRandom::randInt(0, sourceSize - i);
		questions[i] = source[index];
		std::swap(source[sourceSize -1 - i], source[index]);
	}
}

QARoom* QAMgr::GetRoom(UINT32 roomId)
{
	auto it = m_rooms.find(roomId);
	if (it == m_rooms.end())
	{
		return NULL;
	}	
	return it->second;
}

UINT32 QAMgr::GetRoleRoomId(UINT64 roleId)
{
	auto it = m_roleId2RoomId.find(roleId);
	if (it == m_roleId2RoomId.end())
	{
		return 0;
	}
	return it->second;
}

QARoom* QAMgr::GetRoomByRoleId(UINT64 roleId)
{
	UINT32 roomId = GetRoleRoomId(roleId);
	if (!roomId)
	{
		return NULL;
	}
	return GetRoom(roomId);
}

QARoom* QAMgr::CreateRoom(UINT32 type)
{
	QAConditionTable::RowData* row = m_QAConditionTable.GetByQAType(type);
	if(!row)
	{
		SSError << "Get QA Condition Data Failed, QAType:" << type << END;
		return NULL;
	}
	UINT32 roomId = m_curRoomId++;
	QARoom* room = new QARoom(roomId, row);
	if (type == QATYPE_MULTI)
	{
		room->m_beginPrepareTime =  MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_MULTI_QA);
	}

	m_rooms.insert(std::make_pair(roomId, room));
	return room;
}

QuestionLibraryTable::RowData* QAMgr::GetQuestion(UINT32 id)
{
	return m_QuestionLibraryTable.GetByID(id);
}



UINT32 QAMgr::GetLevelSeal(UINT32 levelseal,UINT32 type)
{
	UINT32 maxSeal = 0;
	for (size_t i = 0; i < m_QAPointRankRewardTable.Table.size(); i++)
	{
		QAPointRankRewardTable::RowData* row = m_QAPointRankRewardTable.Table[i];
		if (row->QAType == type)
		{
			if (levelseal == row->LevelSeal)
			{
				return levelseal;
			}else
			{
				if (row->LevelSeal > maxSeal)
				{
					maxSeal = row->LevelSeal;
				}
			}

		}
	}
	return maxSeal;
}

QAPointRankRewardTable::RowData* QAMgr::GetPointRankReward(UINT32 type, UINT32 rank,UINT32 levelseal)
{
	for (size_t i = 0; i < m_QAPointRankRewardTable.Table.size(); i++)
	{
		QAPointRankRewardTable::RowData* row = m_QAPointRankRewardTable.Table[i];
		if (row->QAType != type) continue;
		if (row->RANK != rank) continue;
		if (row->LevelSeal != levelseal) continue;
		return row;
	}
	return NULL;
}

void QAMgr::GetExtraReward(UINT32 qaType, UINT32 level, UINT32 rank, UINT32& itemId, UINT32& itemCount )
{
	itemId = 0; 
	itemCount = 0; 
	if( 0 == rank )
		return; 
	UINT32 id = 0; 
	for( UINT32 i=0; i<m_levelRewardTable.Table.size(); i++ )
	{
		if(qaType != m_levelRewardTable.Table[i]->QAType||level < m_levelRewardTable.Table[i]->MinLevel||level > m_levelRewardTable.Table[i]->MaxLevel)
			continue; 
		id = m_levelRewardTable.Table[i]->ID;
		break;
	}
	QALevelRewardTable::RowData* pData = m_levelRewardTable.GetByID(id);
	if(!pData || pData->ExtraReward.size() == 0 || rank >= pData->ExtraReward.size())
		return; 
	itemId = pData->ExtraReward[ rank - 1 ][0];
	itemCount = pData->ExtraReward[ rank - 1 ][1];
}

void QAMgr::GetStandardReward( UINT32 qaType, UINT32 level, std::vector<KKSG::ItemBrief>& itemList )
{
	itemList.clear();
	UINT32 tableSize = m_levelRewardTable.Table.size();
	for( UINT32 i=0; i<m_levelRewardTable.Table.size(); i++ )
	{
		if(level < m_levelRewardTable.Table[i]->MinLevel||level > m_levelRewardTable.Table[i]->MaxLevel||m_levelRewardTable.Table[i]->QAType != qaType)
			continue; 
		for(UINT32 inner = 0; inner != m_levelRewardTable.Table[i]->Reward.size(); inner++)
		{
			UINT32 itemId = m_levelRewardTable.Table[i]->Reward[inner][0];
			UINT32 itemCount = m_levelRewardTable.Table[i]->Reward[inner][1];
			KKSG::ItemBrief item;
			item.set_itemid( itemId );
			item.set_itemcount( itemCount );
			itemList.push_back(item);
		}
		break; 
	}
}

bool QAMgr::IsAnswerCorrect(std::vector<std::string>& config, const std::string& answer)
{
	if (answer.size() > 24)
	{
		return false;
	}

	bool correct = false;
	for (auto it = config.begin(); it != config.end(); ++it)
	{
		const std::string& correctAnswer = *it;
		size_t found = answer.find(correctAnswer);
		if (it == config.begin())
		{
			if (found != std::string::npos)
			{
				correct = true;
			}
			else 
			{
				correct = false;
				break;
			}
		}
		else
		{
			if (found != std::string::npos)
			{
				correct = false;
				break;
			}
		}
	}
	return correct;
}

void QAMgr::RemoveRole(UINT64 roleId)
{
	m_roleId2RoomId.erase(roleId);
}

bool QAMgr::IsRolePlaying(UINT64 roleId)
{
	return m_roleId2RoomId.find(roleId) != m_roleId2RoomId.end();
}

UINT32 QAMgr::GetPrepareTime(UINT32 type)
{
	QAConditionTable::RowData* pData = m_QAConditionTable.GetByQAType(type);
	if (!pData)
	{
		return 0;
	}
	return pData->PrepareTime;
}

void QAMgr::GMOpenQA(UINT32 type)
{
	UINT32 now = GameTime::GetTime();
	UINT32 zero = XCommon::GetTodayBeginTime(false);
	UINT32 startTime = now + 2 - zero;
	UINT32 endTime = now + 2 + 60 * 15 - zero;
	
	if (type == QATYPE_GUILD)
	{
		MultActivityMgr::Instance()->GMAddOpenDayTime(MULT_ACTIVITY_GUILD_QA, startTime, endTime);
	}
	else if (type == QATYPE_MULTI)
	{
		MultActivityMgr::Instance()->GMAddOpenDayTime(MULT_ACTIVITY_MULTI_QA, startTime, endTime);
	}
	SSInfo << "GM open type: " << type << END;
}
