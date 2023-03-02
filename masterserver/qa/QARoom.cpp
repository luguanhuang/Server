#include "pch.h"
#include "QARoom.h"
#include "timeutil.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/qadef.h"
#include "util/XCommon.h"
#include "qamgr.h"
#include "role/rolesummarymgr.h"
#include "qa/ptcm2c_qaoverntf.h"
#include "qa/ptcm2c_answerackntf.h"
#include "qa/ptcm2c_qaenterroomntf.h"
#include "qa/ptcm2c_qaidnamentf.h"
#include "qa/ptcm2c_qaroomrankntf.h"
#include "qa/ptcm2c_pushquestionntf.h"
#include "qa/ptcm2g_updateguildqaroom.h"
#include "qa/ptcm2g_qaovertogs.h"
#include "mail/mailmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "network/gslink.h"
#include "notice/noticemgr.h"
#include "util/gametime.h"
#include "levelseal/levelsealMgr.h"

///>单人房间5道题
static UINT32 kMaxQuestionPerRoom = 5;
///>多人房间10道题
static UINT32 kMaxQuestionPerRoomMulti = 10;
///>每个题目时间限制: 30秒
static UINT32 kQuestionTimeLimit = 30;
///>答题结束，离开房间剩余时间32
static UINT32 kRoomLeftTime = 32;
///>人答题排行榜人数: 取前20
static UINT32 kMaxRankCount = 20;
///>多人房间人数限制
static UINT32 kMaxRoleMultiRoom = 20;

QARoom::QARoom(UINT32 id, QAConditionTable::RowData* data)
:m_id(id)
,m_type(data->QAType)
,m_beginPrepareTime(0)
,m_closeTime(0)
,m_totalRunningTime(0)
,m_lastUpateTime(0)
,m_data(data)
,m_curIndex(-1)
,m_curBeginTime(0)
,m_guild(0)
{
	m_status = RoomStatus_Prepare;
	m_beginPrepareTime = GameTime::GetTime();
	m_lastUpateTime = GameTime::GetTime();

	// 随机题库
	UINT32 maxNum = kMaxQuestionPerRoomMulti;
	if (m_type == QATYPE_ONE)
	{
		maxNum = kMaxQuestionPerRoom;
	}
	QAMgr::Instance()->RandQuestion(m_type, m_questions, maxNum);

	// 计算总答题时间
	m_totalRunningTime = 0;
	for (auto it = m_questions.begin(); it != m_questions.end(); ++it)
	{
		QuestionLibraryTable::RowData* question = QAMgr::Instance()->GetQuestion(*it);
		if (question)
		{
			m_totalRunningTime += question->TimeLimit;
		}
	}
}

QARoom::~QARoom()
{
	RemovePlayer();
	for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		delete it->second;
	}
	m_roles.clear();
	m_rank.clear();
}

UINT32 QARoom::GetEndTime()
{
	return m_beginPrepareTime + m_data->PrepareTime + m_totalRunningTime;
}

void QARoom::RemovePlayer()
{
	for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		QAMgr::Instance()->RemoveRole(it->second->roleId);
	}
}

UINT32 QARoom::Update()
{
	UINT32 now = GameTime::GetTime();

	if (m_status == RoomStatus_Prepare)
	{
		DoPrepare(now);		
	}
	else if (m_status == RoomStatus_Running)
	{
		DoRunning(now);	
	} 
	else if (m_status == RoomStatus_Over)
	{
		if (now >= m_closeTime)
		{
			m_status = RoomStatus_Close;
		}
	}

	m_lastUpateTime = now;

	return m_status;
}

void QARoom::DoPrepare(UINT32 now)
{
	if (m_beginPrepareTime + m_data->PrepareTime <= now)
	{
		if (m_type == QATYPE_MULTI && m_roles.size() < (UINT32)m_data->MinPlayerNum)
		{
			SetRoomOver();
			return;
		}
		m_status = RoomStatus_Running;
		PushNextQuestion();
	}

	UINT32 firstQuestionTime = m_beginPrepareTime + m_data->PrepareTime;
	if (m_type == QATYPE_GUILD)
	{
		///>出第一题前3分钟消息
		UINT32 time1 = firstQuestionTime - 180;
		if (m_lastUpateTime < time1 && now >= time1)
		{
			NoticeMgr::Instance()->NoticeJoinGuildQA(m_guild, 3);
		}
		///>出第一题前1分钟消息
		UINT32 time2 = firstQuestionTime - 60;
		if (m_lastUpateTime < time2 && now >= time2)
		{
			NoticeMgr::Instance()->NoticeJoinGuildQA(m_guild, 1);
		}
	}
}

void QARoom::DoRunning(UINT32 now)
{
	QuestionLibraryTable::RowData* pData = QAMgr::Instance()->GetQuestion(m_questions[m_curIndex]);
	if (!pData)
	{
		SSError << "Question Config Not Find, RoomID:" << m_id << ", QuestionID:" << m_questions[m_curIndex] << END;
		return;
	}
	if (m_curBeginTime + pData->TimeLimit > now)
	{
		return;
	}
	// 答题超时
	SSInfo << "Room Current Question Timeout, Push Next Question, RoomID:" << m_id << ", QuestionIndex:" << m_curIndex 
		<< ", Interval:" << now - m_curBeginTime << END;
	PushNextQuestion();
}

bool QARoom::IsFull()
{
	UINT32 maxNum = m_type == QATYPE_ONE ? 1 : kMaxRoleMultiRoom;
	return m_roles.size() >= maxNum;
}

QARoom::QARole* QARoom::GetQARole(UINT64 roleId)
{
	auto it = m_roles.find(roleId);
	if (it == m_roles.end())
	{
		return NULL;
	}
	return it->second;
}

void QARoom::UpdateRank(UINT64 roleId)
{
	// 用vector排序，每次找到自己的位置，往前找，交换
	QARole* myInfo = NULL;
	for (INT32 i = m_rank.size() - 1; i >= 0; i--)
	{
		QARole* info = m_rank[i];
		if (info->roleId == roleId)
		{
			myInfo = info;
			continue;
		}
		if (!myInfo)
		{
			continue;
		}
		if ((*myInfo) > (*info))
		{
			std::swap(m_rank[i], m_rank[i+1]);
		}
		else
		{
			break;
		}
	}
}

void QARoom::GetQADataReq(CRole* role, KKSG::GetQADataRes &roRes)
{
	UINT32 now = GameTime::GetTime();
	if (m_status == RoomStatus_Prepare)
	{
		roRes.set_lefttime(m_beginPrepareTime + m_data->PrepareTime - now);
		roRes.set_qid(0);
		roRes.set_serialnum(0);
	} 
	else if (m_status == RoomStatus_Running)
	{
		UINT32 qid = m_questions[m_curIndex];
		QuestionLibraryTable::RowData* question = QAMgr::Instance()->GetQuestion(qid);
		UINT32 leftTime = 0;
		if (question)
		{
			leftTime = m_curBeginTime + question->TimeLimit - now;	
		}
		roRes.set_lefttime(leftTime);
		roRes.set_qid(qid);
		roRes.set_serialnum(m_curIndex + 1);
	}
	roRes.set_result(KKSG::ERR_SUCCESS);

	PushIdName(role);
	PushRankNtf(role);
}

bool QARoom::Enter(CRole* role)
{
	QARole* info = GetQARole(role->GetID());
	if (info)
	{
		return false;
	}
	info = new QARole;
	info->roleId = role->GetID();
	info->name = role->GetName();
	info->updateTime = GameTime::GetTime();
	m_roles.insert(std::make_pair(role->GetID(), info));
	m_rank.push_back(info);

	if (m_type != QATYPE_ONE)
	{
		PushEnterRoomNtf(role->GetID());
		PushIdName(role);
		PushRankNtf(role);
	}
	return true;
}

KKSG::ErrorCode QARoom::Answer(UINT64 roleId, const KKSG::CommitAnswerNtf& req, KKSG::GiveAnserQARewardNtf& res)
{
	QARole* info = GetQARole(roleId);
	if (!info)
	{
		return KKSG::ERR_INVALID_REQUEST;
	}
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (!pSum)
	{
		return KKSG::ERR_INVALID_REQUEST;
	}
	UINT32 destId = pSum->GetCoverDesignationID();

	std::string input = req.answer();
	std::string answer = XCommon::ReplaceStr(input, GetGlobalConfig().StopMark, "");

	if (m_status != RoomStatus_Running)
	{
		PushAnswerAck(roleId, req.audiouid(), req.answer(), false, 0, 0, destId, req.audiotime());
		return KKSG::ERR_INVALID_REQUEST;
	}

	UINT32 questionId = m_questions[m_curIndex];
	QuestionLibraryTable::RowData* question = QAMgr::Instance()->GetQuestion(questionId);
	if (!question)
	{
		SSError << "Get Question Config Failed, RoleID:" << roleId << ", QuestionID:" << questionId << END;
		return KKSG::ERR_QA_NO_DATA;
	}

	bool success = QAMgr::IsAnswerCorrect(question->Answer, answer);
	res.set_is_correct(success);
	res.set_type(m_type);
	res.set_curindex(m_curIndex);
	res.set_questionid(questionId);

	UINT32 rank	= 0; 
	UINT32 times	= 0;
	bool repeat = false;
	if (success)
	{
		std::vector<UINT64>& roleIds = m_correctList[questionId];
		auto it = std::find(roleIds.begin(), roleIds.end(), roleId);
		if (it == roleIds.end())
		{
			roleIds.push_back(roleId);
			rank = roleIds.size();
			times = GetAwardTimes(question);
		}
		else
		{
			repeat = true;
			rank = 999;
		}
	}

	// 推送
	PushAnswerAck(roleId, req.audiouid(), req.answer(), success && !repeat, rank, times, destId, req.audiotime());

	// 玩家首次回答正确, 奖励与排名
	if (success && !repeat)
	{
		UINT32 score = 0;
		if (rank >= question->Point.size())
		{
			score = question->Point[question->Point.size() - 1];
		}
		else
		{
			score = question->Point[rank -1];
		}
		SSWarn << "Anwer Right Score, RoleID:" << roleId << ", Rank:" << rank << ", RewardPoint:" << score << END;

		info->correctNum++;
		info->score += score;
		info->updateTime = GameTime::GetTime();

		// 奖励
		GetReward(roleId, rank, times, res);
		// 更新排名 
		UpdateRank(roleId);
		// 推送排名
		PushRankNtfToAll();
	}

	// 单人直接推送下一题
	if (success && m_type == QATYPE_ONE)
	{
		PushNextQuestion();
	}
	return KKSG::ERR_SUCCESS;
}

void QARoom::GiveUp()
{
	if (m_type != QATYPE_ONE)
	{
		return;
	}
	if (m_status != RoomStatus_Running)
	{
		return;
	}
	PushNextQuestion();
}

void QARoom::SetRoomOver(bool reward)
{
	m_status = RoomStatus_Over;
	m_closeTime = GameTime::GetTime() + kRoomLeftTime;
	UINT32 levelSeal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	levelSeal = QAMgr::Instance()->GetLevelSeal(levelSeal,m_type);
	// 推送答题结果
	for (size_t i = 0 ; i < m_rank.size(); ++i)
	{
		QARole* info = m_rank[i];
		CRole* role = CRoleManager::Instance()->GetByRoleID(info->roleId);
		if (!role)
		{
			continue;
		}

		PtcM2C_QAOverNtf ntf;
		ntf.m_Data.set_correct(info->correctNum);
		ntf.m_Data.set_total(m_questions.size());
		for (size_t j = 0; j < info->items.size(); ++j)
		{
			KKSG::ItemBrief* item = ntf.m_Data.add_datalist();
			item->CopyFrom(info->items[j]);
		}
		role->Send(ntf);
	}
	NotifyGSOver();

	// 不发奖励
	if (!reward)
	{
		return;
	}
	if (m_type == QATYPE_ONE)
	{
		return;
	}


	// 排行奖励
	for (size_t i = 0 ; i < m_rank.size(); ++i)
	{
		QARole* info = m_rank[i];
		CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(info->roleId);
		if(!pSum)
		{
			continue; 
		}
		UINT32 rank = i + 1;
		QAPointRankRewardTable::RowData* pData = QAMgr::Instance()->GetPointRankReward(m_type, rank,levelSeal);
		if (!pData)
		{
			continue;
		}
		std::vector<ItemDesc> items;
		for( UINT32 inner = 0;  inner < pData->Reward.size(); inner++ )
		{
			ItemDesc item; 
			item.itemID = pData->Reward[inner][0];
			item.itemCount = pData->Reward[inner][1];
			items.push_back(item);
		}

		MailData mail(pData->Title, pData->Content, items, MAIL_EXPIRE_TIME);
		mail.SetReason(ItemFlow_QAAward);
		CMailMgr::Instance()->SendMail(info->roleId, mail);
	}
}

void QARoom::NotifyGSOver()
{
	PtcM2G_QAOverToGS over;
	for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		CRole* pushRole = CRoleManager::Instance()->GetByRoleID(it->first);
		if (!pushRole)
		{
			continue;
		}
		over.m_Data.set_role_id(pushRole->GetID());
		pushRole->SendMsgToGS(over);
	}

	if (m_type == QATYPE_GUILD)
	{
		PtcM2G_UpdateGuildQARoom ntf;
		ntf.m_Data.set_guild_id(m_guild);
		ntf.m_Data.set_room_id(m_id);
		ntf.m_Data.set_status(RoomStatus_Over);
		GSLink::Instance()->SendToAllLine(ntf);
	}
}

UINT32 QARoom::GetAwardTimes(const QuestionLibraryTable::RowData *pConfig)
{
	UINT32 nRandnum = XRandom::randInt(1, 100);
	UINT32 nTotal	= 0; 
	UINT32 nTime    = 0;
	for( UINT32 i = 0; i<pConfig->Luck.size(); i++ )
	{
		if( nTotal < nRandnum && nRandnum <= nTotal + (UINT32)pConfig->Luck[i][0] )
		{
			nTime = (UINT32)pConfig->Luck[i][1];
			break; 
		}
		nTotal += pConfig->Luck[i][0];
	}
	if(nTime == 0)
		nTime = 1; 
	return nTime;
}

void QARoom::GetReward(UINT64 roleId, int rank, int times, KKSG::GiveAnserQARewardNtf &res)
{
	QARole* info = GetQARole(roleId);
	if (!info) return;
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (!role) return;

	// 标准奖励
	std::vector<KKSG::ItemBrief> vecStandardReward;
	QAMgr::Instance()->GetStandardReward(m_type, role->GetLevel(), vecStandardReward);
	for (int i = 0; i < vecStandardReward.size(); i++ )
	{
		UINT32 itemId = vecStandardReward[i].itemid();
		UINT32 itemCount = (UINT32)vecStandardReward[i].itemcount() * times;
		info->AddReward(itemId, itemCount);
		KKSG::ItemBrief* item = res.add_items();
		item->set_itemid(itemId);
		item->set_itemcount(itemCount);
	}

	// 额外奖励
	UINT32 extraItemId		= 0; 
	UINT32 extraItemCount	= 0; 
	QAMgr::Instance()->GetExtraReward(m_type, role->GetLevel(), rank, extraItemId, extraItemCount);
	if(extraItemId > 0 && extraItemCount > 0) 
	{
		info->AddReward(extraItemId, extraItemCount);
		KKSG::ItemBrief* item = res.add_items();
		item->set_itemid(extraItemId);
		item->set_itemcount(extraItemCount);
	}
}

void QARoom::PushAnswerAck(UINT64 roleId, UINT64 audioUID, const std::string& answer, bool correct, 
	INT32 rank, INT32 times, UINT32 titleId, UINT32 audioTime)
{
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (!role) return;

	PtcM2C_AnswerAckNtf ntf;
	ntf.m_Data.set_audiouid(audioUID);
	ntf.m_Data.set_answer(answer);
	ntf.m_Data.set_correct(correct);
	ntf.m_Data.set_roleid(roleId);
	ntf.m_Data.set_rank(rank);
	ntf.m_Data.set_times(times);
	ntf.m_Data.set_username(role->GetName());
	ntf.m_Data.set_answertime(GameTime::GetTime() - XCommon::GetTodayBeginTime(false));
	ntf.m_Data.set_coverdesignationid(titleId);
	ntf.m_Data.set_audiotime(audioTime);
	ntf.m_Data.set_profession(role->GetProfession());
	BroadCast(ntf);
}

void QARoom::PushEnterRoomNtf(UINT64 roleId)
{
	if (m_type == QATYPE_ONE) return;

	UINT32 now = GameTime::GetTime();
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (!role) return;

	PtcM2C_QAEnterRoomNtf ntf;
	ntf.m_Data.set_roleid(role->GetID());
	ntf.m_Data.set_name(role->GetName());
	ntf.m_Data.set_vip(role->GetVipLevel());
	ntf.m_Data.set_profession(role->GetProfession());
	ntf.m_Data.set_coverdesignationid(role->GetCoverDesignationID());
	ntf.m_Data.set_time(now - (UINT32)XCommon::GetDayBeginTime(now, false));
	BroadCast(ntf);
}

void QARoom::PushIdName(CRole* role)
{
	if (m_type == QATYPE_ONE) return;

	PtcM2C_QAIDNameNtf ntf;
	for (auto it = m_rank.begin(); it != m_rank.end(); ++it)
	{
		KKSG::QAIDName* pData = ntf.m_Data.add_idname();
		pData->set_uuid((*it)->roleId);
		pData->set_name((*it)->name);
	}
	role->Send(ntf);
}

void QARoom::PushRankNtfToAll()
{
	if (m_type == QATYPE_ONE) return;

	for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(it->first);
		if (!role)
		{
			continue;
		}
		PushRankNtf(role);
	}
}

void QARoom::PushRankNtf(CRole* role)
{
	if (m_type == QATYPE_ONE) return;

	PtcM2C_QARoomRankNtf ntf;
	for (auto it = m_rank.begin(); it != m_rank.end(); ++it)
	{
		if( (UINT32)ntf.m_Data.datalist_size() >= kMaxRankCount) break; 
		KKSG::QARoomRankData* pData = ntf.m_Data.add_datalist();
		QARole* info = *it;
		pData->set_uuid(info->roleId);
		pData->set_score(info->score);
	}
	auto it = m_roles.find(role->GetID());
	if (it != m_roles.end())
	{
		QARole* info = it->second;
		ntf.m_Data.set_myscore(info->score);
	}
	role->Send(ntf);
}

void QARoom::PushNextQuestion()
{
	m_curIndex++;
	if (m_curIndex >= (INT32)m_questions.size())
	{
		m_status = RoomStatus_Over;
		SetRoomOver();
		return;
	}
	// 第一次问题的时候推送一下信息？
	if (m_curIndex == 0)
	{
		for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
		{
			CRole* pushRole = CRoleManager::Instance()->GetByRoleID(it->first);
			if (!pushRole)
			{
				continue;
			}
			PushRankNtf(pushRole);
			PushIdName(pushRole);
		}
	}

	PtcM2C_PushQuestionNtf ntf;
	ntf.m_Data.set_qid(m_questions[m_curIndex]);
	ntf.m_Data.set_serialnum(m_curIndex + 1);
	BroadCast(ntf);
	m_curBeginTime = GameTime::GetTime();
}

void QARoom::BroadCast(const CProtocol &ptc)
{
	for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		CRole* pushRole = CRoleManager::Instance()->GetByRoleID(it->first);
		if (!pushRole)
		{
			continue;
		}
		pushRole->Send(ptc);
	}
}
