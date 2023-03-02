#include "pch.h"
#include "ChannelCloseSys.h"
#include "config.h"
#include "util/XCommon.h"
#include "unit/role.h"

INSTANCE_SINGLETON(ChannelCloseSysMgr)

ChannelCloseSysMgr::ChannelCloseSysMgr()
{
}

ChannelCloseSysMgr::~ChannelCloseSysMgr()
{
}

bool ChannelCloseSysMgr::Init()
{
	if (GSConfig::Instance()->IsCrossGS())
		return true;
	return LoadFile();
}

void ChannelCloseSysMgr::Uninit()
{
	ClearFile();
}

bool ChannelCloseSysMgr::CheckFile()
{
	return true;
}

bool ChannelCloseSysMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();
	_LoadCloseSystem();
	return true;
}

void ChannelCloseSysMgr::ClearFile()
{
	m_mapCloseSys.clear();
}



bool ChannelCloseSysMgr::IsSysOpen(Role *pRole,UINT32 nSysID, const std::string &channel)
{
	auto iter = m_mapCloseSys.find(channel);
	if (iter!=m_mapCloseSys.end())
	{
		if (GSConfig::Instance()->GetAppType() == KKSG::GAME_APP_QQ)
		{
			for(auto siter =  (iter->second).qq.begin(); siter != (iter->second).qq.end(); siter++)
			{
				if (nSysID==*siter)
					return false;
			}
		}else if (GSConfig::Instance()->GetAppType() == KKSG::GAME_APP_WECHAT)
		{
			for(auto siter =  (iter->second).weixin.begin(); siter != (iter->second).weixin.end(); siter++)
			{
				if (nSysID==*siter)
					return false;
			}
		}
	}
	return true;
}

void ChannelCloseSysMgr::_LoadCloseSystem()
{
	std::string closeInfo = GSConfig::Instance()->ReadFileContent("serveronly/channelclosesystem.txt");
	std::vector<std::string> Lines = Split(closeInfo, "\r\n");
	for (UINT32 i = 0; i < Lines.size(); ++i)
	{
		std::string temp = Trim(Lines[i]);
		if (!temp.empty())
		{
			const std::vector<std::string> svec = Split(temp , "=");
			std::string apptype;
			if (svec.size()==2)
				apptype = svec[0];
			else
				continue;
			const std::vector<std::string> vecsys = Split(svec[1] , ":");
			UINT32 nType = 0;
			for (auto iter = vecsys.begin(); iter!=vecsys.end(); iter++)
			{
				const std::vector<std::string>& vecid = Split(*iter , "|");
				for(auto titer = vecid.begin(); titer!=vecid.end(); titer++)
				{
					if (nType==0)
						m_mapCloseSys[apptype].qq.push_back(atoi((*titer).c_str()));
					else
						m_mapCloseSys[apptype].weixin.push_back(atoi((*titer).c_str()));
				}
				nType++;
			}
		}
	}
}