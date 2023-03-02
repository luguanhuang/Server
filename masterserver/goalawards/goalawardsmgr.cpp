#include "pch.h"
#include "goalawardsmgr.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "pb/project.pb.h"
#include "timeutil.h"
#include "table/GoalAwards.h"
#include "goalawards/ptcm2c_goalawardsredpoint.h"
#include "common/rpcm2g_msgiveitem.h"
#include "define/tlogenum.h"
#include "dbgoalawards/ptcm2d_dbgoalawardssaventf.h"
#include "network/dblink.h"
#include "db/mysqlmgr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "garden/garden_utility.h"
#include "friend/friendop.h"
#include "loghelper/tlogr.h"

INSTANCE_SINGLETON(GoalAwardsMgr);

GoalAwardsMgr::GoalAwardsMgr(){}

GoalAwardsMgr::~GoalAwardsMgr(){}

bool GoalAwardsMgr::Init()
{
    m_timeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1*1000, -1, __FILE__, __LINE__);
    if(m_timeHandler == INVALID_HTIMER)
    {
        return false;
    }

    if(!LoadFile()) return false;

    QueryDBGoalAwardsInfo();
    return true;
}

void GoalAwardsMgr::Uninit()
{
    if(m_timeHandler != INVALID_HTIMER)
    {
        CTimerMgr::Instance()->KillTimer(m_timeHandler);
    }
}

bool GoalAwardsMgr::CheckFile()
{
    GoalAwards tmpGoalAwards;
    if (!tmpGoalAwards.LoadFile("table/GoalAwards.txt"))
    {
        LogWarn("Load GoalAwards.txt failed");
        return false;
    }

    UINT32 totalsize = tmpGoalAwards.Table.size();
    std::unordered_map<UINT32, std::vector<UINT32>> tmpGoalAwardsTableIndex;
    for (UINT32 i = 0; i < totalsize; ++i)
    {
        std::vector<UINT32>& tmpList = tmpGoalAwardsTableIndex[tmpGoalAwards.Table[i]->GoalAwardsID];
        tmpList.push_back(i);
        if(tmpList.size() != tmpGoalAwards.Table[i]->AwardsIndex)
        {
            LogWarn("GoalAwards.txt row %d AwardsIndex Error", i+1);
            return false;
        }
    }

    return true;
}

bool GoalAwardsMgr::LoadFile()
{
    if (!CheckFile())
    {
        return false;
    }
    ClearFile();

    m_GoalAwardsTable.LoadFile("table/GoalAwards.txt");
    for (UINT32 i = 0; i < m_GoalAwardsTable.Table.size(); ++i)
    {
        m_GoalAwardsTableIndex[m_GoalAwardsTable.Table[i]->GoalAwardsID].push_back(i);
    }

    return true;
}

void GoalAwardsMgr::ClearFile()
{
    m_GoalAwardsTable.Clear();
    m_GoalAwardsTableIndex.clear();
}

const static UINT32 MAX_SAVE_NUM_ONCE = 10;
void GoalAwardsMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
    UINT64 begin = TimeUtil::GetMilliSecond();
    bool isUpdate = false;
    //每1*4秒更新每一个
    {	
        UINT32 count = 0;
        while(!m_isChangedGoalAwardsInfo.empty() && count < MAX_SAVE_NUM_ONCE)
        {
            UINT64 index = m_isChangedGoalAwardsInfo.front();
            m_isChangedGoalAwardsInfo.pop();
            count++;

            SaveDBGoalAwardsInfo(index);
            isUpdate = true;
        }
        if (!m_isChangedGoalAwardsInfo.empty())
        {
            LogInfo("GoalAwards need save num:%u", m_isChangedGoalAwardsInfo.size());
        }
    }
    UINT64 end = TimeUtil::GetMilliSecond();
    if (isUpdate)
    {
        LogInfo("GoalAwards save to db use time: %llu ms", end - begin);
    }
}

GoalAwards::RowData* GoalAwardsMgr::GetTableRow(UINT32 idGoalAwards, UINT32 indexAwards)
{
    auto itorlist = m_GoalAwardsTableIndex.find(idGoalAwards);
    if(m_GoalAwardsTableIndex.end() == itorlist)
    {
        return NULL;
    }

    const std::vector<UINT32>& idlist = itorlist->second;
    if(indexAwards >= idlist.size())
    {
        return NULL;
    }

    UINT32 index = idlist[indexAwards];
    if(index >= m_GoalAwardsTable.Table.size())
    {
        return NULL;
    }
    return m_GoalAwardsTable.Table[index];
}

void GoalAwardsMgr::RpcGoalAwardsGetList(CRole* pRole, const KKSG::GoalAwardsGetList_C2M &roArg, KKSG::GoalAwardsGetList_M2C &roRes)
{
    if(NULL == pRole)
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }

    UINT64 roleid = pRole->GetID();
    std::map<UINT32, GoalAwardInfo>& roledata = InitRoleData(roleid);

    UINT32 type = roArg.type();
    for(auto itor = roledata.begin(); itor != roledata.end(); ++itor)
    {
        GoalAwardInfo& info = itor->second;
        if(type != GetTableRowType(info)) continue;
        
        FreshGoalAwardsInfo(info);
        KKSG::GoalAwardsInfo* pInfo = roRes.add_goalawardslist();
        pInfo->set_goalawardsid(info.mGoalAwardsID);
        pInfo->set_doneindex(info.mDoneIndex);
        pInfo->set_gottenawardsindex(info.mGottenAwardsIndex);

        double tmpvalue = 0.f;
        GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mGottenAwardsIndex);
        if(pRowData != NULL)
        {
            tmpvalue = info.mScore[pRowData->GKID];
        }
        pInfo->set_totalvalue(tmpvalue);
    }
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void GoalAwardsMgr::RpcGoalAwardsGetAwards(CRole* pRole, const KKSG::GoalAwardsGetAwards_C2M &roArg, KKSG::GoalAwardsGetAwards_M2C &roRes)
{
    roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
    if(NULL == pRole) return;

    UINT64 roleid = pRole->GetID();
    auto itorRoleData = m_RoleData.find(roleid);
    if(m_RoleData.end() == itorRoleData) return;

    UINT32 goalAwardsID = roArg.goalawardsid();
    auto itorRoleInfo = itorRoleData->second.find(goalAwardsID);
    if(itorRoleData->second.end() == itorRoleInfo) return;

    GoalAwardInfo& info = itorRoleInfo->second;
    FreshGoalAwardsInfo(info);
    if(!CheckHasAwards(roleid, info))return;

    GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mGottenAwardsIndex);
    if(NULL == pRowData) return;
    info.mGottenAwardsIndex++;
    roRes.set_gottenawardsindex(info.mGottenAwardsIndex);
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    m_isChangedGoalAwardsInfo.push(roleid);
    DoTLogGoalAwards(roleid, info.mGoalAwardsID, info.mGottenAwardsIndex, GetTableRowType(info), 1);

    {
        RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
        rpc->m_oArg.set_roleid(roleid);
        rpc->m_oArg.set_reason(ItemFlow_GoalAwards);
        rpc->m_oArg.set_subreason(ItemFlow_GoalAwards_GetReward);

        for(auto itor = pRowData->Awards.begin(); itor != pRowData->Awards.end(); ++itor)
        {
            KKSG::ItemBrief* item = rpc->m_oArg.add_items();
            item->set_itemid((*itor)[0]);
            item->set_itemcount((*itor)[1]);
        }
        pRole->SendMsgToGS(*rpc);
		if (pRowData->TitleID > 0)
		{
			EventMgr::Instance()->AddEvent(roleid, DESIGNATION_COM_TYPE_GOALAWARDS, pRowData->TitleID);
		}
    }
}

void GoalAwardsMgr::PtcGoalAwardsRedPoint(UINT64 roleid)
{
    CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
    if(NULL == pRole) return;

    auto itorRoleData = m_RoleData.find(roleid);
    if(m_RoleData.end() == itorRoleData) return;

    std::set<UINT32> typelist;
    for(auto itor = itorRoleData->second.begin(); itor != itorRoleData->second.end(); ++itor)
    {
        GoalAwardInfo& info = itor->second;
        FreshGoalAwardsInfo(info);
        if(!CheckHasAwards(roleid, info)) continue;
        typelist.insert(GetTableRowType(info));
    }

    PtcM2C_GoalAwardsRedPoint ptc;
    for(auto itor = typelist.begin(); itor != typelist.end(); ++itor)
    {
        if((UINT32)-1 == *itor) continue;
        ptc.m_Data.add_typelist(*itor);
    }
    pRole->Send(ptc);
}

UINT32 GoalAwardsMgr::GetTableRowType(const GoalAwardInfo& info)
{
    GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, 0);
    if(NULL == pRowData) return (UINT32)-1;
    return pRowData->Type;
}

bool GoalAwardsMgr::CheckHasAwards(UINT64 roleid, const GoalAwardInfo& info)
{
    CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
    GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mGottenAwardsIndex);
    if(pRoleSummary && pRowData)
    {
        if(pRoleSummary->GetLevel() < pRowData->ShowLevel)
        {
            return false;
        }
    }
    return info.mGottenAwardsIndex < info.mDoneIndex;
}

GOAL_AWARD_TYPE GoalAwardsMgr::GetGoalAwardType(UINT32 idGoalAwards)
{
    switch(idGoalAwards)
    {
    case GOAL_AWARD_ID_101:    //101战场单局人头
    case GOAL_AWARD_ID_102:    //102战场单局助攻
    case GOAL_AWARD_ID_103:    //103战场单局团队输出
    case GOAL_AWARD_ID_301:    //301峡谷单局人头
    case GOAL_AWARD_ID_302:    //302峡谷单局助攻
    case GOAL_AWARD_ID_303:    //303峡谷单局团队输出
    case GOAL_AWARD_ID_304:    //304峡谷单局个人输出
    case GOAL_AWARD_ID_309:    //309峡谷无队友死亡击败对手人数
    case GOAL_AWARD_ID_602:    //602所有好友度总值
    case GOAL_AWARD_ID_801:    //801家园料理等级
    case GOAL_AWARD_ID_803:    //803家园钓鱼等级
        return GOAL_AWARD_TYPE_SET_UP;

    case GOAL_AWARD_ID_1001:    //1001世界boss伤害名次
        return GOAL_AWARD_TYPE_SET_DOWN;

    case GOAL_AWARD_ID_104:    //104战场累计人头
    case GOAL_AWARD_ID_105:    //105战场累计助攻
    case GOAL_AWARD_ID_106:    //106战场累计输出
    case GOAL_AWARD_ID_107:    //107战场mvp次数
    case GOAL_AWARD_ID_108:    //108战场胜利次数
    case GOAL_AWARD_ID_200:    //200天梯总胜利场次
    case GOAL_AWARD_ID_201:    //201天梯对战战士胜利场次
    case GOAL_AWARD_ID_202:    //202天梯对战弓箭手胜利场次
    case GOAL_AWARD_ID_203:    //203天梯对战魔法师胜利场次
    case GOAL_AWARD_ID_204:    //204天梯对战牧师胜利场次
    case GOAL_AWARD_ID_205:    //205天梯对战学者胜利场次
    case GOAL_AWARD_ID_206:    //206天梯对战刺客胜利场次
    case GOAL_AWARD_ID_207:    //207天梯对战舞娘胜利场次
    case GOAL_AWARD_ID_305:    //305峡谷累计人头
    case GOAL_AWARD_ID_306:    //306峡谷累计助攻
    case GOAL_AWARD_ID_307:    //307峡谷累计输出
    case GOAL_AWARD_ID_308:    //308峡谷累计mvp
    case GOAL_AWARD_ID_310:    //310峡谷胜利场数
    case GOAL_AWARD_ID_701:    //701龙本累计助战次数
    case GOAL_AWARD_ID_702:    //702巢穴累计助战次数
    case GOAL_AWARD_ID_802:    //802家园驱赶小妖精次数
    case GOAL_AWARD_ID_804:    //804家园举办家宴次数
    case GOAL_AWARD_ID_901:    //901大乱斗累计击杀
    case GOAL_AWARD_ID_1002:   //1002世界boss累计输出
    case GOAL_AWARD_ID_1003:   //1003世界boss累计次数
    case GOAL_AWARD_ID_1401:   //1401结婚关卡胜利次数
        return GOAL_AWARD_TYPE_ADD_UP;

    case GOAL_AWARD_ID_403:    //403巢穴平均输出
    case GOAL_AWARD_ID_502:    //502龙本无伤通关人数
    case GOAL_AWARD_ID_503:    //503龙本平均输出量
        return GOAL_AWARD_TYPE_GK_UP;

    case GOAL_AWARD_ID_401:    //401巢穴通关时间
    case GOAL_AWARD_ID_402:    //402巢穴通关不死人数
    case GOAL_AWARD_ID_501:    //501龙本通关时间
    case GOAL_AWARD_ID_504:    //504龙本通关不死人数
        return GOAL_AWARD_TYPE_GK_DOWN;

    case GOAL_AWARD_ID_505:    //505龙本通关零输出不死
        return GOAL_AWARD_TYPE_GK_PASS;

    case GOAL_AWARD_ID_601:    //601好友组队通关次数
    case GOAL_AWARD_ID_603:    //603微信手Q好友组队通关次数
        return GOAL_AWARD_TYPE_GK_ADD_UP;

    default:
        break;
    }

    //11龙本关卡不死通关
    if(idGoalAwards >= GOAL_AWARD_ID_1101 && idGoalAwards <= GOAL_AWARD_ID_11xx)
    {
        return GOAL_AWARD_TYPE_GK_DOWN;
    }

    //12小龙本x分钟通关
    if(idGoalAwards >= GOAL_AWARD_ID_1201 && idGoalAwards <= GOAL_AWARD_ID_12xx)
    {
        return GOAL_AWARD_TYPE_GK_DOWN;
    }

    //13小龙本零输出不死
    if(idGoalAwards >= GOAL_AWARD_ID_1301 && idGoalAwards <= GOAL_AWARD_ID_13xx)
    {
        return GOAL_AWARD_TYPE_GK_PASS;
    }

    return GOAL_AWARD_TYPE_NONE;
}

void GoalAwardsMgr::FreshValue(UINT64 roleid, UINT32 idGoalAwards, double value, UINT32 gkid)
{
    if(GOAL_AWARD_ID_1 == idGoalAwards)
    {
        PtcGoalAwardsRedPoint(roleid);
        return;
    }

    std::map<UINT32, GoalAwardInfo>& roledate = InitRoleData(roleid);

    if(GOAL_AWARD_ID_11xx == idGoalAwards)
    {
        _FreshValueList(roledate, roleid, GOAL_AWARD_ID_1101, GOAL_AWARD_ID_11xx, value, gkid);
        return;
    }
    if(GOAL_AWARD_ID_12xx == idGoalAwards)
    {
        _FreshValueList(roledate, roleid, GOAL_AWARD_ID_1201, GOAL_AWARD_ID_12xx, value, gkid);
        return;
    }
    if(GOAL_AWARD_ID_13xx == idGoalAwards)
    {
        _FreshValueList(roledate, roleid, GOAL_AWARD_ID_1301, GOAL_AWARD_ID_13xx, value, gkid);
        return;
    }

    _FreshValue(roledate, roleid, idGoalAwards, value, gkid);
}


void GoalAwardsMgr::_FreshValueList(std::map<UINT32, GoalAwardInfo>& roledate, UINT64 roleid, UINT32 idStart, UINT32 idEnd, double value, UINT32 gkid)
{
    for(UINT32 i = idStart; i <= idEnd; i++)
    {
        _FreshValue(roledate, roleid, i, value, gkid);
    }
}

void GoalAwardsMgr::_FreshValue(std::map<UINT32, GoalAwardInfo>& roledate, UINT64 roleid, UINT32 idGoalAwards, double value, UINT32 gkid)
{
    LogDebug("GoalAwardsMgrFreshValue roleid=%llu, idGoalAwards=%d, value=%f, gkid=%d\n", roleid, idGoalAwards, value, gkid);
    auto itortable = m_GoalAwardsTableIndex.find(idGoalAwards);
    if(m_GoalAwardsTableIndex.end() == itortable) return;
    std::vector<UINT32>& idlist = itortable->second;

    GoalAwardInfo& info = roledate[idGoalAwards];
    info.mGoalAwardsID = idGoalAwards;
    UINT32 oldDoneIndex = info.mDoneIndex;

    GOAL_AWARD_TYPE goalawardtype = GetGoalAwardType(info.mGoalAwardsID);
    switch(goalawardtype)
    {
    case GOAL_AWARD_TYPE_GK_UP:
        {
            for(UINT32 i = 0; i<idlist.size(); i++)
            {
                GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mDoneIndex);
                if(NULL == pRowData) break;
                if(pRowData->GKID != gkid) break;
                if(value > info.mScore[gkid])
                {
                    info.mScore[gkid] = value;
                }
                if(value < pRowData->AwardsValue)
                {
                    break;
                }
                info.mDoneIndex++;
            }
        }
        break;
    case GOAL_AWARD_TYPE_SET_UP:
        {
            for(UINT32 i = 0; i<idlist.size(); i++)
            {
                GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mDoneIndex);
                if(NULL == pRowData) break;
                if(value > info.mScore[0])
                {
                    info.mScore[0] = value;
                }
                if(value < pRowData->AwardsValue)
                {
                    break;
                }
                info.mDoneIndex++;
            }
        }
        break;
    case GOAL_AWARD_TYPE_GK_DOWN:
        {
            for(UINT32 i = 0; i<idlist.size(); i++)
            {
                GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mDoneIndex);
                if(NULL == pRowData) break;
                if(pRowData->GKID != gkid) break;
                if(0 == info.mScore[gkid] || value < info.mScore[gkid])
                {
                    info.mScore[gkid] = value;
                }
                if(value > pRowData->AwardsValue)
                {
                    break;
                }
                info.mDoneIndex++;
            }
        }
        break;
    case GOAL_AWARD_TYPE_SET_DOWN:
        {
            for(UINT32 i = 0; i<idlist.size(); i++)
            {
                GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mDoneIndex);
                if(NULL == pRowData) break;
                if(0 == info.mScore[0] || value < info.mScore[0])
                {
                    info.mScore[0] = value;
                }
                if(value > pRowData->AwardsValue)
                {
                    break;
                }
                info.mDoneIndex++;
            }
        }
        break;
    case GOAL_AWARD_TYPE_ADD_UP:
        {
            info.mScore[0] += value;
            for(UINT32 i = 0; i<idlist.size(); i++)
            {
                GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mDoneIndex);
                if(NULL == pRowData) break;
                if(info.mScore[0] < pRowData->AwardsValue)
                {
                    break;
                }
                info.mDoneIndex++;
            }
        }
        break;
    case GOAL_AWARD_TYPE_GK_ADD_UP:
        {
            for(UINT32 i = 0; i<idlist.size(); i++)
            {
                GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mDoneIndex);
                if(NULL == pRowData) break;
                if(pRowData->GKID != gkid) break;
                if(0 == i)
                {
                    info.mScore[gkid] += value;
                }
                if(info.mScore[gkid] < pRowData->AwardsValue)
                {
                    break;
                }
                info.mDoneIndex++;
            }
        }
        break;
    case GOAL_AWARD_TYPE_GK_PASS:
        {
            GoalAwards::RowData* pRowData = GetTableRow(info.mGoalAwardsID, info.mDoneIndex);
            if(NULL == pRowData) return;
            if(pRowData->GKID != gkid) return;
            info.mDoneIndex++;
            info.mScore[0] = 0;
        }
        break;
    case GOAL_AWARD_TYPE_NONE:
    default:
        break;
    }
    FreshGoalAwardsInfo(info);
    m_isChangedGoalAwardsInfo.push(roleid);
    PtcGoalAwardsRedPoint(roleid);

    for(UINT32 i = oldDoneIndex; i < info.mDoneIndex; ++i)
    {
        DoTLogGoalAwards(roleid, info.mGoalAwardsID, i+1, GetTableRowType(info), 0);
    }
}

void GoalAwardsMgr::GMClearDB(UINT64 roleid)
{
    auto itor = m_RoleData.find(roleid);
    if(m_RoleData.end() == itor) return;
    itor->second.clear();
    m_isChangedGoalAwardsInfo.push(roleid);
}

void GoalAwardsMgr::FreshGoalAwardsInfo(GoalAwardInfo& info)
{
}

std::map<UINT32, GoalAwardInfo>& GoalAwardsMgr::InitRoleData(UINT64 roleid)
{
    std::map<UINT32, GoalAwardInfo>& roledata = m_RoleData[roleid];

    if(roledata.find(GOAL_AWARD_ID_801) == roledata.end() || 
        roledata.find(GOAL_AWARD_ID_803) == roledata.end())
    {
        //case GOAL_AWARD_ID_801:    //801家园料理等级
        //case GOAL_AWARD_ID_803:    //803家园钓鱼等级
        double tmpCookLevel = 1.f;
        double tmpFishLevel = 1.f;
        CRoleSummary* psummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
        if(NULL != psummary)
        {
            Garden* garden = GardenUtility::Instance()->GetMyGarden(roleid, psummary->GetName());
            if (NULL != garden)
            {
                FoodInfo food_info;
                garden->GetGardenCookingInfo(food_info);
                tmpCookLevel = food_info.cooking_level;

                const FishInfoStore& fish_info =garden->GetFishInfo();
                tmpFishLevel = fish_info.fish_level;
            }
        }
        _FreshValue(roledata, roleid, GOAL_AWARD_ID_801, tmpCookLevel, 0);
        _FreshValue(roledata, roleid, GOAL_AWARD_ID_803, tmpFishLevel, 0);
    }

    if(roledata.find(GOAL_AWARD_ID_602) == roledata.end())
    {
        //case GOAL_AWARD_ID_602:    //602所有好友度总值
        FriendOp op(roleid);
        const std::map<UINT64, FriendInfo>* pFriendList = op.GetFriendList();
        UINT64 nTotalFriendDegree = 0;
        if(pFriendList)
        {
            for(auto itor = pFriendList->begin(); itor != pFriendList->end(); ++itor)
            {
                nTotalFriendDegree += op.GetDegree(itor->first);
            }
        }
        _FreshValue(roledata, roleid, GOAL_AWARD_ID_602, nTotalFriendDegree, 0);
    }
    return roledata;
}

bool GoalAwardsMgr::QueryDBGoalAwardsInfo()
{
    IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
    std::stringstream ss;
    ss << "select `_id`, `data` from goalawards";
    IMysqlRecordSet* poRes = NULL;
    if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
    {
        LogError("Table goalawards Read failed, errorMsg: %s", poMySqlConn->GetError());
        return false;
    }

    KKSG::DBGoalAwardsSave dbData;

    while(poRes->FetchRow())
    {
        char* pVal = NULL;
        UINT32 dwLen = 0;

        UINT64 index;
        if(poRes->GetFieldValue(0, &pVal, dwLen))
        {
            index = convert<UINT64>(pVal);
        }
        else
        {
            LogError("Read 'index' column failed");
            return false;
        }

        if(!poRes->GetFieldValue(1, &pVal, dwLen))
        {
            LogError("Read 'data' column failed");
            return false;
        }

        if(!dbData.ParseFromArray(pVal, dwLen))
        {
            LogError("Parse data failed, index:%d\n", index);
            return false;
        }

        std::map<UINT32, GoalAwardInfo>& roledata = m_RoleData[index];
        for(int i = 0; i<dbData.infolist().size(); ++i)
        {
            const KKSG::GoalAwardsInfo& kinfo = dbData.infolist(i);
            GoalAwardInfo& info = roledata[kinfo.goalawardsid()];
            info.mGoalAwardsID = kinfo.goalawardsid();
            info.mDoneIndex = kinfo.doneindex();
            info.mGottenAwardsIndex = kinfo.gottenawardsindex();
            for(int i = 0; i < kinfo.gkidvalue().size(); ++i)
            {
                info.mScore[kinfo.gkidvalue(i).gkid()] = kinfo.gkidvalue(i).gkvalue();
            }
        }
    }
    poRes->Release();
    return true;
}

bool GoalAwardsMgr::SaveDBGoalAwardsInfo(UINT64 roleid)
{
    auto itordata = m_RoleData.find(roleid);
    if(m_RoleData.end() == itordata) return false;

    PtcM2D_DBGoalAwardsSaveNtf ptc;

    ptc.m_Data.set_roleid(roleid);
    for(auto itor = itordata->second.begin(); itor != itordata->second.end(); itor++)
    {
        auto tmp = ptc.m_Data.add_infolist();
        tmp->set_goalawardsid(itor->second.mGoalAwardsID);
        tmp->set_doneindex(itor->second.mDoneIndex);
        tmp->set_gottenawardsindex(itor->second.mGottenAwardsIndex);
        for(auto itor2 = itor->second.mScore.begin(); itor2 != itor->second.mScore.end(); itor2++)
        {
            KKSG::GoalAwardsValue* pvalue = tmp->add_gkidvalue();
            pvalue->set_gkid(itor2->first);
            pvalue->set_gkvalue(itor2->second);
        }
    }
    DBLink::Instance()->SendTo(ptc);
    return true;
}

void GoalAwardsMgr::DoTLogGoalAwards(UINT64 roleid, UINT32 awardid, UINT32 awardindex, UINT32 awardtype, UINT32 op)
{
    TGoalAwardFlow oLog(TLogMgr::Instance()->GetTagString());
    oLog.m_uRoleID = roleid;
    oLog.m_AwardID = awardid;
    oLog.m_AwardIndex = awardindex;
    oLog.m_AwardType = awardtype;
    oLog.m_op = op;
    oLog.Do();
}

