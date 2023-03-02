#ifndef __PLATNOTICE_DEF_H__
#define __PLATNOTICE_DEF_H__
#include "pch.h"
#include "pb/project.pb.h"

#define GAME_GUEST 3
struct PlatNoticeInfo
{
	UINT32 type;
	UINT32 noticeID;
	bool isOpen;
	UINT32 areaID;
	UINT32 platID;
	std::string content;
	UINT32 updateTime;
	bool isNew;
	std::string title;
	PlatNoticeInfo()
	{
		type = 0;
		noticeID = 0;
		isOpen = true;
		areaID = 0;
		platID = 0;
		updateTime = 0;
		isNew = true;
	}
	void FromKKSG(const KKSG::PlatNotice& info)
	{
		type = info.type();
		noticeID = info.noticeid();
		isOpen = info.isopen();
		areaID = info.areaid();
		platID = info.platid();
		content = info.content();
		updateTime = info.updatetime();
		isNew = info.isnew();
		title = info.title();
	}

	void ToKKSG(KKSG::PlatNotice& info)
	{
		info.set_type(type);
		info.set_noticeid(noticeID);
		info.set_isopen(isOpen);
		info.set_areaid(areaID);
		info.set_platid(platID);
		info.set_content(content);
		info.set_updatetime(updateTime);
		info.set_isnew(isNew);
		info.set_title(title);
	}
};
#endif