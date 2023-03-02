#ifndef __NOTICE_H__
#define __NOTICE_H__

#include "define/noticedef.h"

enum NoticeParamType
{
	ParamRole,
	ParamItem,
	ParamNum,
	ParamGuild,
	ParamContent,
	ParamLink,
	ParamDragonGuild,

	ParamNoticeDummy,
};

namespace KKSG
{
	class ChatInfo;
}
class Guild;
class Role;
struct XItem;
struct ItemDesc;
class RoleSummary;

struct Notice
{
	UINT32 noticeid;
	UINT32 offset;
	bool cansend;
	std::vector<NoticeParamType>* paramtypes;
	KKSG::ChatInfo* chatinfo;

	Notice(UINT32 id);
	Notice(const std::string& info, UINT32 channel, UINT32 priority);
	~Notice();

	//void AddSrc(RoleSummary* role);
	bool AddLink();

	bool Add(Role* role);
	bool Add(RoleSummary* role);
	bool Add(XItem* item);
	bool Add(const ItemDesc& desc);
	bool Add(UINT32 num);
	bool AddGuild(UINT64 guildid);

	void Replace(const std::string& src, const std::string& dest);

	NoticeParamType GetParamType();
	bool IsValidChannel(UINT32 channel);
	bool CanSend();

	bool Send(UINT32 serverID = 0);
	bool SendInCrossGs(UINT32 serverID);
	bool SendSelf(UINT64 roleid);
	bool SendSelf(Role* role);
	bool SendTeam(int teamid);
	bool SendGuild(UINT64 guildid);
	bool SendRoleGuild(Role* role);
};

struct RawNotice
{
	std::string hint;

	RawNotice(UINT32 id);
	void Replace(const std::string& src, const std::string& dest);
	void Send(Role* role);
};

#endif