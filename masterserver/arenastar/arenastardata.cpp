#include "pch.h"
#include "arenastardata.h"
#include "arenastarmgr.h"

const int ArenaStarHistoryCount = 5;

ASRoleData::ASRoleData(UINT64 roleID)
{
	m_data.set_roleid(roleID);
}

ASRoleData::ASRoleData(const KKSG::ArenaStarRoleData& data)
{
	m_data = data;
}

ASRoleData::~ASRoleData()
{

}

KKSG::ArenaStarAllData* ASRoleData::GetAllData(KKSG::ArenaStarType type)
{
	for(int i = 0; i < m_data.typedata_size(); ++i)
	{
		if(m_data.typedata(i).type() == type)
		{
            return m_data.mutable_typedata(i)->mutable_alldata();
		}
	}
	// add newdata if have not this type
	auto newdata = m_data.add_typedata();
	newdata->set_type(type);
	SetChangedFlag();
	return newdata->mutable_alldata();
}

/**
 * @brief add one season data
 *
 * @param type: arena type
 * @param rank: season rank
 * @param season: season num
 */
void ASRoleData::OnSeaonEnd(KKSG::ArenaStarType type, UINT32 rank, UINT32 season)
{
    auto alldata = GetAllData(type);
    if(NULL == alldata)
    {
        return ;
    }
    UINT32 max = GetMaxSeason(type);
    if(max >= season)
    {
        SSError << "role = " << RoleID() << " type =" << type << " season max = " << max << " end season = " << season << END;
        return ;
    }
	if(max + 1 != season)
	{
        SSInfo << "role = " << RoleID() << " type =" << type << " season max = " << max << " end season = " << season << END;
	}
	alldata->clear_notice();//clear notice
    auto& histall = *alldata->mutable_historydata();
    if(1 == rank)
    {
       histall.set_rankonenum( histall.rankonenum() + 1 );
    }
    if(rank <= 10)
    {
       histall.set_ranktennum( histall.ranktennum() + 1 );
    }

    KKSG::RankOnce seasondata;
    seasondata.set_rank(rank);
    seasondata.set_season(season);
    *histall.add_rankrecent() = seasondata;
	for(int i = histall.rankrecent_size() - 1; i >= 1; --i)//move to the first
	{
		histall.mutable_rankrecent()->SwapElements(i, i-1);
	}
    if(histall.rankrecent_size() > ArenaStarHistoryCount)//to many
    {
        histall.mutable_rankrecent()->RemoveLast();
    }
	SetChangedFlag(); 
}

/**
 * @brief check add season data not delay
 *
 * @param type: arena type
 * @param season: add data to max season
 */
void ASRoleData::CheckFillSeasonData(KKSG::ArenaStarType type, UINT32 season)
{
    UINT32 max = GetMaxSeason(type);

    if(max >= season)
    {
		return;
    }

    SSInfo << " fill season data roleid = " << RoleID() << " type = " << type << " cur = " << max << " to = " << season << END;

	UINT32 from = max;
	if(season >= ArenaStarHistoryCount && (season - ArenaStarHistoryCount) > from)
	{
		from = (season - ArenaStarHistoryCount);
	}

    for(UINT32 i = from + 1; i <= season; ++i)
    {
       OnSeaonEnd(type, ArenaStarMaxRank, i);
    }
}

UINT32 ASRoleData::GetMaxSeason(KKSG::ArenaStarType type)
{
    auto data = GetAllData(type);
    if(NULL == data)
    {
        return 0;
    }
    UINT32 max = 0;
    for(int i = 0; i < data->historydata().rankrecent_size(); ++i)
    {
        const auto& rankrecent = data->historydata().rankrecent(i);
        if(rankrecent.season() > max)
        {
            max = rankrecent.season();
        }
    }
    return max;
}

void ASRoleData::InfoPrint()
{
	SSDebug << " role = " << RoleID() << END;
	for(int i = 0; i < m_data.typedata_size(); ++i)
	{
		const auto& typedata = m_data.typedata(i);
		SSDebug << " type = " << typedata.type() << END;
		const auto& histdata = typedata.alldata().historydata();
		SSDebug << " one = " << histdata.rankonenum() << " ten = " << histdata.ranktennum() << END;
		for(int j = 0; j < histdata.rankrecent_size(); ++j)
		{
			SSDebug << " season = " << histdata.rankrecent(j).season() << " rank = " << histdata.rankrecent(j).rank() << END;
		}
	}
}

int ASRoleData::GetNotice(KKSG::ArenaStarType type)
{
	auto alldata = GetAllData(type);
	if(alldata)
	{
		return alldata->notice();
	}
	return 1;//had noticed
}

void ASRoleData::SetNotice(KKSG::ArenaStarType type, int notice)
{
	auto alldata = GetAllData(type);
	if(alldata)
	{
		alldata->set_notice(notice);
		SetChangedFlag();
	}
}

void ASRoleData::FillHistoryData(KKSG::ArenaStarType type, KKSG::ArenaStarHistData& histdata)
{
	auto alldata = GetAllData(type);
	if(NULL == alldata)
	{
		return;
	}
	histdata = alldata->historydata();
}

void ASRoleData::SetChangedFlag()
{
	ArenaStarMgr::Instance()->AddChangeRole(RoleID());
}

void ASRoleData::Clear()
{
	UINT64 roleID = RoleID();
	m_data.Clear();
	m_data.set_roleid(roleID);

	SetChangedFlag();
}
