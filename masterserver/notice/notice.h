#ifndef __NOTICE_H__
#define __NOTICE_H__

#include "pb/project.pb.h"
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

class CRoleSummary;
class Guild;
class DragonGuild;
struct Notice
{
	bool cansend;
	UINT32 offset;
	UINT32 noticeid;
	UINT32 type;
	UINT32 level;
	UINT64 uid; ///> roleid, teamid, guildid
	KKSG::ChatInfo* chatinfo;
	std::vector<NoticeParamType>* paramtypes;

	Notice(UINT32 id);
	Notice(KKSG::NoticeForwardNtf& ntf);
	Notice(UINT32 uChannel, UINT32 uPro, std::string content);
	~Notice();

	bool AddLink(); // no need to fill the param
	bool AddLink(Guild* guild); // fill the guild param
	bool AddLink(DragonGuild* dragonguild);
	bool Add(UINT32 num);
	bool AddRole(CRoleSummary* summary);
	bool AddItem(UINT32 itemid, UINT32 itemcount);
	bool AddGuild(UINT64 guildid);
	void Replace(const std::string& src, const std::string& dest);

	bool AddDragonGuild(UINT64 dragonguildId);

	bool Send();
	bool SendRole(UINT64 roleid);
	bool SendGuild(UINT64 guildid);
	bool SendPartner(UINT64 partnerId);
	bool SendServerID(UINT32 serverID);

	bool SendPlays(std::vector<UINT64>& roles);
	void FillGuildName();
	NoticeParamType GetParamType();
	void GetForwardSession(UINT32 type, UINT64 uid, std::vector<SessionType>& sessions);
};


#endif