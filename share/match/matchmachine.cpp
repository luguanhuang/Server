#include "pch.h"
#include "time.h"
#include "matchmachine.h"
#include "util.h"
#include "math.h"

std::unordered_map<UINT32, std::vector<std::vector<UINT32>>> MatchMachine::m_stMatchTemplate;

MatchMachine::MatchMachine()
{
	_BuildMatchTemplate();

	m_number = 1;
	m_type = MatchMachineType_Unknown;
	m_units.resize(m_number + 1);
}

MatchMachine::~MatchMachine()
{
}

void MatchMachine::Init(UINT32 num, MatchMachineType type)
{
	assert(m_stMatchTemplate.find(num) != m_stMatchTemplate.end());

	m_number = num;
	m_type = type;
	m_units.resize(m_number + 1);
}

void MatchMachine::Clear()
{
	for (UINT32 i = 1; i <= m_number; i++)
	{
		m_units[i].clear();
	}
	m_units2time.clear();
}

bool MatchMachine::AddMatchUnit(UINT64 uid, UINT32 num)
{
	if (_GetUnitSet(uid) != NULL)
	{
		LogError("MatchMachineType:%d, uid [%llu] is already in MatchMachine", m_type, uid);
		return false;
	}
	if (num == 0)
	{
		LogError("MatchMachineType:%d, uid [%llu] num = 0", m_type, uid);
		return false;
	}
	if (num > m_number)
	{
		LogWarn("MatchMachineType:%d, uid [%llu] num:%u > %u", m_type, uid, num, m_number);
		num = m_number;
	}

	if (m_units2time.find(uid) != m_units2time.end())
	{
		LogError("MatchMachineType:%d, uid [%llu] not in units but in units2time, should be code error", m_type, uid);
		assert(false);
		return false;
	}

	time_t nowTime = time(NULL);
	m_units2time[uid] = nowTime;
	m_units[num].insert(std::make_pair(nowTime, uid));

	return true;
}

bool MatchMachine::DelMatchUnit(UINT64 uid)
{
	std::set<std::pair<time_t, UINT64>>* pUnitSet = _GetUnitSet(uid);
	if (pUnitSet == NULL)
	{
		//LogError("MatchMachineType:%d, uid [%llu] is not in MatchMachine", m_type, uid);
		return false;
	}

	pUnitSet->erase(std::make_pair(m_units2time[uid], uid));
	m_units2time.erase(uid);

	return true;
}

void MatchMachine::DoMatch(std::vector<std::vector<UINT64>>& result)
{
	auto iter = m_stMatchTemplate.find(m_number);
	if (iter == m_stMatchTemplate.end())
	{
		return;
	}

	const std::vector<std::vector<UINT32>>& temp = iter->second;
	for (UINT32 i = 0; i < temp.size(); i++)
	{
		const std::vector<UINT32>& vec = temp[i];
		UINT32 matchNum = -1;	//max uint
		for (UINT32 j = 1; j <= m_number; j++)
		{
			if (vec[j] == 0) 
			{
				continue;
			}
			matchNum = (std::min)(matchNum, (UINT32)m_units[j].size() / vec[j]);
			if (matchNum == 0) 
			{
				break;
			}
		}

		for (UINT32 cnt = 0; cnt < matchNum; cnt++)
		{
			result.push_back(std::vector<UINT64>());
			std::vector<UINT64>& matchVec = result.back();

			for (UINT32 j = 1; j <= m_number; j++)
			{
				if (vec[j] == 0) 
				{
					continue;
				}
				UINT32 needNum = vec[j];
				while (needNum && m_units[j].size())
				{
					matchVec.push_back(m_units[j].begin()->second);
					
					std::map<UINT64, time_t>::size_type size = m_units2time.erase(m_units[j].begin()->second);
					if (size == 0)
					{
						LogError("MatchMachineType:%d, erase size is zero in unit2time map, should be code error", m_type);
						assert(false);
					}
					m_units[j].erase(m_units[j].begin());
					needNum--;
				}
			}
		}
	}
}

bool MatchMachine::GMChangeNum(UINT32 num)
{
	if (m_stMatchTemplate.find(num) == m_stMatchTemplate.end())
	{
		return false;
	}

	if (num >= m_number)
	{
		m_number = num;
		m_units.resize(m_number + 1);
		return true;
	}
	for (UINT32 i = num + 1; i <= m_number; i++)
	{
		for (auto j = m_units[i].begin(); j != m_units[i].end(); j++)
		{
			m_units[num].insert(*j);
		}
		m_units[i].clear();
	}

	m_number = num;

	return true;
}

void MatchMachine::PrintLog()
{
	std::string ss;
	for (UINT32 i = 1; i <= m_number; i++)
	{
		ss += "Num: " + ToString(i) + ", Count:" + ToString(m_units[i].size()) + "\n";
	}
	LogInfo("MatchMachineType:%d, MatchNumber:%u\n%s", m_type, m_number, ss.c_str());
}

std::set<std::pair<time_t, UINT64>>* MatchMachine::_GetUnitSet(UINT64 uid)
{
	auto iter = m_units2time.find(uid);
	if (iter == m_units2time.end())
	{
		return NULL;
	}

	std::pair<time_t, UINT64> key = std::make_pair(iter->second, uid);
	for (UINT32 i = 1; i <= m_number; i++)
	{
		if (m_units[i].find(key) != m_units[i].end())
		{
			return &m_units[i];
		}
	}
	return NULL;
}

void MatchMachine::_BuildMatchTemplate()
{
	if (!m_stMatchTemplate.empty())
	{
		return;
	}

	std::vector<UINT32> vec;

	for (UINT32 i = 1; i <= 10; i++)
	{
		vec.resize(i + 1);
		_DfsBuild(i, i, m_stMatchTemplate[i], vec);
		LogInfo("num:%u, MatchTemplate size:%u", i, m_stMatchTemplate[i].size());
	}
}

void MatchMachine::_DfsBuild(UINT32 dep, UINT32 val, std::vector<std::vector<UINT32>>& temp, std::vector<UINT32>& vec)
{
	if (dep == 1)
	{
		vec[1] = val;
		temp.push_back(vec);
		return;
	}
	for (UINT32 i = val / dep; (int)i >= 0; i--)
	{
		vec[dep] = i;
		_DfsBuild(dep - 1, val - i * dep, temp, vec);
	}
}