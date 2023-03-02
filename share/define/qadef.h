#ifndef _QA_DEF_H_
#define _QA_DEF_H_

enum QAType
{
	QATYPE_ONE		= 1,		///>单人答题
	QATYPE_MULTI	= 2,		///>多人抢答
	QATYPE_GUILD	= 3,		///>公会抢答
	QATYPE_MAX,
};

enum RoomStatus
{
	RoomStatus_Prepare = 1,		// 准备
	RoomStatus_Running = 2,		// 答题中
	RoomStatus_Over = 3,		// 结束
	RoomStatus_Close = 4,		// 关闭
};

#endif