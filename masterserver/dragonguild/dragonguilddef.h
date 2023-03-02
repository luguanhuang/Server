#ifndef __DRAGONGUILD_DEF_H__
#define __DRAGONGUILD_DEF_H__

#define INVAILD_DRAGONGUILD 0

enum DragonGuildPermission
{
	DGEM_DISMISS,			// 解散
	DGEM_SET_VICELEADER,	// 任命副会长
	DGEM_SETTINGS,			// 公会设置
	DGEM_ANNOUNCEMENT,		// 设置公告
	DGEM_APPROVAL,			// 审批
	DGEM_FIREMEMBER,		// 踢人
	DGPEM_MAX,
};

enum DragonGuildApproval
{
	DGA_ACCEPT,				// 接受
	DGA_REJECT,				// 拒绝
	DGA_REJECT_ALL,			// 一键拒绝
};

enum DragonGuildPosition
{
	DGPOS_LEADER,			// 队长
	DGPOS_VICELEADER,		// 副队长
	DGPOS_MEMBER,			// 队员

	DGPOS_MAX,
};


#endif