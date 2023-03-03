/*
**  @file $RCSfile: tmodid.h,v $
**  general description of this module
**  $Id: tmodid.h,v 1.3 2009-09-14 10:53:41 jacky Exp $
**  @author $Author: jacky $
**  @date $Date: 2009-09-14 10:53:41 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TMODID_H
#define TMODID_H


//basic components id

// 模块ID 0x03 这个值没有使用,
// 这个值或许是给 BUS 模块预留的，
// 但是 BUS 头文件定义的ID是 0x01,
// 这个值与 DR 模块的ID值相同，冲突，
// 为了保持对历史版本的兼容，只能保持

#define TMODID_NON          0x00
#define TMODID_DR			0x01
#define TMODID_LOG          0x02
#define TMODID_MNG			0x04
#define TMODID_SEC			0x05

/* the least identfier app can use. */
#define TMODID_APP			0x10
#define TMODID_ORM			0x11


/* the largest identfier, only for TAPP */
#define TMODID_TAPP         0x80


#endif
