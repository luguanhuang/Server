/*
**  @file $RCSfile: tlog_event.h,v $
**  general description of this module
**  $Id: tlog_event.h,v 1.3 2009-01-12 07:01:44 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-01-12 07:01:44 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_EVENT_H
#define TLOG_EVENT_H

#include "tlog/tlog_loc_info.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct tagTlogEvent
{
	int	evt_priority;
	char* evt_msg;
	int evt_msg_len;
	const tlog_loc_info_t* evt_loc;
	int evt_is_msg_binary;
	unsigned int evt_id;
	unsigned int evt_cls;
	int evt_type;
	int evt_version;
};

typedef struct tagTlogEvent TLOGEVENT;
typedef struct tagTlogEvent *LPTLOGEVENT;

#ifdef __cplusplus
}
#endif

#endif /* TLOG_EVENT_H */


