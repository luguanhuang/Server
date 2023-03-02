#include "pch.h"
#include "social.h"
#include "pb/project.pb.h"
#include "socialmgr.h"

void SocialInfo::SocialInfo2Db(KKSG::SocialInfo2Db* socialinfo2db)
{
	flower.SaveToDb(socialinfo2db);

	if (tshow.m_ischanged)
	{
		KKSG::TShow2DB tshow2db;
		tshow2db.set_updatetime(tshow.m_updatetime);
		tshow2db.set_week(tshow.m_week);
		for (auto iter = tshow.m_rolecount.begin(); iter != tshow.m_rolecount.end(); ++iter)
		{
			KKSG::TShowRoleVoteCount* roleCount = tshow2db.add_voteinfo();
			if (roleCount)
			{
				roleCount->set_roleid(iter->roleid);
				roleCount->set_num(iter->count);
			}
		}

		socialinfo2db->set_tshow(tshow2db.SerializeAsString());

		tshow.m_ischanged = false;
	}
}

void SocialInfo::Db2SocialInfo(const KKSG::SocialInfo2Db* socialinfo2db)
{
	flower.LoadFromDb(socialinfo2db);

	if (socialinfo2db->has_tshow() && !(socialinfo2db->tshow().empty()))
	{
		KKSG::TShow2DB tshowinfo;
		tshowinfo.ParseFromString(socialinfo2db->tshow());
		tshow.m_updatetime = tshowinfo.updatetime();
		tshow.m_week = tshowinfo.week();
		for (int i = 0; i < tshowinfo.voteinfo_size(); ++i)
		{
			const KKSG::TShowRoleVoteCount& info = tshowinfo.voteinfo(i);
			TShowRoleCountRecord record;
			record.roleid = info.roleid();
			record.count = info.num();
			tshow.m_rolecount.push_back(record);
		}
		tshow.Update();
	}
}

SocialInfo::SocialInfo(UINT64 roleID):flower(roleID)
{
}
