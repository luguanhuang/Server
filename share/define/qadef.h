#ifndef _QA_DEF_H_
#define _QA_DEF_H_

enum QAType
{
	QATYPE_ONE		= 1,		///>���˴���
	QATYPE_MULTI	= 2,		///>��������
	QATYPE_GUILD	= 3,		///>��������
	QATYPE_MAX,
};

enum RoomStatus
{
	RoomStatus_Prepare = 1,		// ׼��
	RoomStatus_Running = 2,		// ������
	RoomStatus_Over = 3,		// ����
	RoomStatus_Close = 4,		// �ر�
};

#endif