#ifndef __DRAGONGUILD_DEF_H__
#define __DRAGONGUILD_DEF_H__

#define INVAILD_DRAGONGUILD 0

enum DragonGuildPermission
{
	DGEM_DISMISS,			// ��ɢ
	DGEM_SET_VICELEADER,	// �������᳤
	DGEM_SETTINGS,			// ��������
	DGEM_ANNOUNCEMENT,		// ���ù���
	DGEM_APPROVAL,			// ����
	DGEM_FIREMEMBER,		// ����
	DGPEM_MAX,
};

enum DragonGuildApproval
{
	DGA_ACCEPT,				// ����
	DGA_REJECT,				// �ܾ�
	DGA_REJECT_ALL,			// һ���ܾ�
};

enum DragonGuildPosition
{
	DGPOS_LEADER,			// �ӳ�
	DGPOS_VICELEADER,		// ���ӳ�
	DGPOS_MEMBER,			// ��Ա

	DGPOS_MAX,
};


#endif