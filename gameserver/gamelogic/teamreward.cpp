#include "pch.h"
#include "teamreward.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "stagereward.h"
#include "battle/ptcg2c_teamselectnotify.h"
#include "mail/mailconfig.h"
#include "mail/mailsender.h"

BoxRewardInfo* RoleBoxRewardInfo::GetItem(UINT32 _index)
{
	if (boxs.size() < _index)
	{
		return NULL;	
	}
	return &boxs[_index - 1];
}

bool RoleBoxRewardInfo::Select(UINT32 _index)
{
	if (0 != index)
	{
		SSWarn<<"roleid:"<<roleid<<" has selected:"<<index<<END;
		return false;
	}
	index = _index;
	///> 选的和放大镜选的一样，不用再随机
	if (peer != index)
	{
		RandomOne(index);
	}
	SSInfo<<"roleid:"<<roleid<<" select:"<<index<<END;
	return true;
}

bool RoleBoxRewardInfo::CanPeer()
{
	if (0 != index)
	{
		SSWarn<<"roleid:"<<roleid<<" has selected:"<<index<<END;
		return false;
	}
	if (0 != peer)
	{
		SSWarn<<"roleid:"<<roleid<<" has peer:"<<peer<<END;
		return false;
	}
	return true;
}

bool RoleBoxRewardInfo::Peer(UINT32 _index)
{
	if (0 != index)
	{
		return false;
	}
	if (0 != peer)
	{
		return false;
	}
	peer = _index;
	RandomOne(peer);
	SSInfo<<"roleid:"<<roleid<<" peer:"<<peer<<END;
	return true;
}

void RoleBoxRewardInfo::RandomOne(UINT32 _index)
{
	if (randomcount > 2)
	{
		SSError<<"roleid:"<<roleid<<" randomcount:"<<randomcount<<END;
		return;
	}
	if (0 != boxs[_index - 1].item.itemID)
	{
		return;
	}

	// random one box
	UINT32 boxid = 1;
	bool isdefinite = false;
	if (0 == randomcount)
	{
		if (NULL != sceneinfo && !isrobot)
		{
			isdefinite = SceneConfig::Instance()->_CheckDefiniteDrop(roleid, boxid, sceneinfo->m_pConf->type, false);
		}
	}

	if (isdefinite)
	{
		for (auto j = confs.begin(); j != confs.end(); ++j)
		{
			if (boxid == j->quality)
			{
				confs.erase(j);
				break;
			}
		}
	}
	else
	{
		UINT32 all = 0;
		for (auto i = confs.begin(); i != confs.end(); ++i)
		{
			all += i->per;
		}

		UINT32 allrandom = 0;
		UINT32 random = XRandom::randInt(1, all + 1);
		for (auto j = confs.begin(); j != confs.end(); ++j)
		{
			allrandom += j->per;
			if (random <= allrandom)
			{
				boxid = j->quality;
				confs.erase(j);
				break;
			}
		}
	}

	if (0 == randomcount)
	{
		if (NULL != sceneinfo && !isrobot)
		{
			SceneConfig::Instance()->_CheckDefiniteDrop(roleid, boxid, sceneinfo->m_pConf->type, true);
		}
	}

	// fill info
	BoxRewardInfo tempbox;
	UINT32 dropid = SceneConfig::Instance()->GetBoxDropId(sceneinfo, boxid);
	SceneConfig::Instance()->FillBox(boxid, dropid, rolelevel, tempbox);

	++randomcount;
	boxs[_index - 1] = tempbox;

	return;
}

void RoleBoxRewardInfo::RandomLeft()
{
	std::deque<UINT32> types;
	for (auto j = confs.begin(); j != confs.end(); ++j)
	{
		types.push_back(j->quality);
	}
	confs.clear();
	
	// fill info
	std::random_shuffle(types.begin(), types.end());

	for (auto i = boxs.begin(); i != boxs.end(); ++i)
	{
		if (0 != i->item.itemID)
		{
			continue;
		}
		if (types.empty())
		{
			break;
		}
		UINT32 quality = types.front();
		types.pop_front();

		BoxRewardInfo tempbox;
		UINT32 dropid = SceneConfig::Instance()->GetBoxDropId(sceneinfo, quality);
		SceneConfig::Instance()->FillBox(quality, dropid, rolelevel, tempbox);

		*i = tempbox;
	}

	return;
}

void RoleBoxRewardInfo::Give()
{
	if (isgive)
	{
		return;
	}
	if (isrobot)
	{
		return;
	}
	if (boxs.size() <= index - 1)
	{
		SSWarn<<"roleid:"<<roleid<<" box size:"<<boxs.size()<<" index:"<<index<<END;
		return;
	}
	ItemDesc item = boxs[index - 1].item;
	Role* role = RoleManager::Instance()->FindByRoleID(roleid);
	if (NULL == role)
	{
		// send mail
		std::vector<ItemDesc> items;
		items.push_back(item);
		MailSender sender;
		std::string titile;
		std::string content;
		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_StageBoxReward);
		if (NULL != conf)
		{
			std::string stagename = NULL == sceneinfo ? "" : sceneinfo->m_pConf->Comment;
			titile = conf->m_title;
			content = MailConfig::Instance()->ReplaceString("%s", stagename, conf->m_content);
		}
		sender.SetContent(Mail_System,  titile,  content);
		sender.SetItems(items);
		sender.SetReason(ItemFlow_Stage_BoxGive);
		sender.Send(roleid);

		UINT32 sceneid = (NULL == sceneinfo) ? 0 : sceneinfo->m_pConf->id;
		SSWarn<<"scene:"<<sceneid<<" roleid:"<<roleid<<" not find"<<END;
		return;
	}
	else
	{
		BagGiveItemTransition give(role);
		// set reason
		give.SetReason(ItemFlow_Stage, ItemFlow_Stage_BoxGive);
   		give.GiveItem(item);
		give.NotifyClient();
	}
	UINT32 sceneid = (NULL == sceneinfo) ? 0 : sceneinfo->m_pConf->id;
	SSInfo<<"scene:"<<sceneid<<" roleid:"<<roleid<<" itemid:"<<item.itemID<<" itemcount:"<<item.itemCount<<" bind:"<<item.isbind<<END;
	isgive = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TeamSelectReward::TeamSelectReward(Scene* scene):m_scene(scene)
{
	m_isallselected = false;
}

TeamSelectReward::~TeamSelectReward()
{
}

void TeamSelectReward::OnSetRank(UINT64 roleid, UINT32 rolelevel, UINT32 star, bool isrobot)
{
	auto it = m_roleSelect.find(roleid);
	if(it == m_roleSelect.end())
	{
		// init
		RoleBoxRewardInfo& info = m_roleSelect[roleid];
		info.star = star;
		info.roleid = roleid;
		info.rolelevel = rolelevel;
		info.isrobot = isrobot;
		info.sceneinfo = (NULL == m_scene) ? NULL : m_scene->GetSceneInfo();

		std::vector<Sequence<UINT32, 2>>* rawconfs = SceneConfig::Instance()->GetBoxConf(star, m_scene->GetSceneInfo());
		if (NULL != rawconfs)
		{
			std::vector<BoxDropConf>& confs = m_roleSelect[roleid].confs;
			for (auto i = rawconfs->begin(); i != rawconfs->end(); ++i)
			{
				BoxDropConf conf;
				conf.quality = i->seq[0];
				conf.per = i->seq[1];
				confs.push_back(conf);
			}
		}
	}
	else
	{
		SSError<<"roleid:"<<roleid<<" is exist"<<END;
	}
}

bool TeamSelectReward::ValidIndex(UINT32 index)
{
	return index >= 1 && index <=4;
}

RoleBoxRewardInfo* TeamSelectReward::GetRoleBoxInfo(UINT64 roleID)
{
	auto it = m_roleSelect.find(roleID);
	if(it == m_roleSelect.end())
	{
		SSError<<"roleid:"<<roleID<<" invalid"<<END;
		return NULL;
	}
	return &it->second;
}

bool TeamSelectReward::Select(UINT64 roleID, UINT32 index)
{
	auto it = m_roleSelect.find(roleID);
	if(it == m_roleSelect.end())
	{
		SSError<<"roleid:"<<roleID<<" invalid"<<END;
		return false;
	}
	bool issucceed = it->second.Select(index);
	
	if(AllSelected())
	{
		Finish();
	}
	return issucceed;
}

void TeamSelectReward::Finish(bool normal/*=true*/)
{
	if(!normal)
	{
		for (auto i = m_roleSelect.begin(); i != m_roleSelect.end(); ++i)
		{
			if (i->second.index == 0)
			{
				i->second.Select(1);
			}
		}
	}
	m_isallselected = true;
}

bool TeamSelectReward::AllSelected()
{
	if (m_isallselected)
	{
		return true;
	}

	for (auto i = m_roleSelect.begin(); i != m_roleSelect.end(); ++i)
	{
		if (!m_scene->IsHelper(i->first) && !m_scene->IsSelectBoxExcept(i->first, i->second.rolelevel))
		{
			if (i->second.index == 0)
			{
				return false;
			}
		}
	}

	m_isallselected = true;
	return true;
}

void TeamSelectReward::End()
{
	if (NULL == m_scene)
	{
		return;
	}
	for (auto i = m_roleSelect.begin(); i != m_roleSelect.end(); ++i)
	{
		UINT64 roleid = i->first;
		if (!m_scene->IsHelper(roleid) && !m_scene->IsSelectBoxExcept(roleid, i->second.rolelevel))
		{
			i->second.RandomLeft();
			i->second.Give();
		}
	}
}

void TeamSelectReward::OnRoleLeave(Role* role)
{
	if (NULL == m_scene)
	{
		return;
	}
	auto i = m_roleSelect.find(role->GetID());
	if (i == m_roleSelect.end())
	{
		return;
	}
	if (i->second.isgive)
	{
		return;
	}
	if (0 == i->second.index)
	{
		i->second.Select(1);
	}
	if (!m_scene->IsHelper(i->first) && !m_scene->IsSelectBoxExcept(i->first, i->second.rolelevel))
	{
		i->second.RandomLeft();
		i->second.Give();
	}
}

void TeamSelectReward::FillRobotBox(UINT64 robotid, const RoleBoxRewardInfo& boxinfo)
{
	//RoleBoxRewardInfo tempboxinfo = boxinfo;
	//tempboxinfo.isrobot = true;
	//tempboxinfo.sceneinfo = NULL != m_scene ? m_scene->GetSceneInfo() : NULL;
}

RoleBoxRewardInfo TeamSelectReward::FillBox(SceneInfo* info, UINT32 star, UINT64 roleid, UINT32 rolelv)
{
	RoleBoxRewardInfo boxinfo;
	boxinfo.boxs.clear();
	if(NULL == info)
	{
		return boxinfo;
	}
	star = XCommon::GetOneCount(star);
	SceneConfig::Instance()->GetBox(star, info, roleid, rolelv, boxinfo);

	return boxinfo;
}
