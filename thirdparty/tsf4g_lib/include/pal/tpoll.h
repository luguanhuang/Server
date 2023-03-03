/*
**  @file $RCSfile: tpoll.h,v $
**  general description of this module
**  $Id: tpoll.h,v 1.2 2008-08-22 01:38:41 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008-08-22 01:38:41 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef TPOLL_H
#define TPOLL_H


#if defined(_WIN32) || defined(_WIN64)
#else
	#include <sys/epoll.h>
#endif

#include <pal/trules.h>
#include "pal/tsocket.h"

typedef intptr_t TEPOLL_HANDLE;


#if defined(_WIN32) || defined(_WIN64)

enum EPOLL_EVENTS
  {
    EPOLLIN = 0x001,
#define EPOLLIN EPOLLIN
#define TPOLLIN EPOLLIN
    EPOLLPRI = 0x002,
#define EPOLLPRI EPOLLPRI
#define TPOLLPRI EPOLLPRI
    EPOLLOUT = 0x004,
#define EPOLLOUT EPOLLOUT
#define TPOLLOUT EPOLLOUT
    EPOLLRDNORM = 0x040,
#define EPOLLRDNORM EPOLLRDNORM
#define TPOLLRDNORM EPOLLRDNORM
    EPOLLRDBAND = 0x080,
#define EPOLLRDBAND EPOLLRDBAND
#define TPOLLRDBAND EPOLLRDBAND
    EPOLLWRNORM = 0x100,
#define EPOLLWRNORM EPOLLWRNORM
#define TPOLLWRNORM EPOLLWRNORM
    EPOLLWRBAND = 0x200,
#define EPOLLWRBAND EPOLLWRBAND
#define TPOLLWRBAND EPOLLWRBAND
    EPOLLMSG = 0x400,
#define EPOLLMSG EPOLLMSG
#define TPOLLMSG EPOLLMSG
    EPOLLERR = 0x008,
#define EPOLLERR EPOLLERR
#define TPOLLERR EPOLLERR
    EPOLLHUP = 0x010,
#define EPOLLHUP EPOLLHUP
#define TPOLLHUP EPOLLHUP
    EPOLLONESHOT = (1 << 30),
#define EPOLLONESHOT EPOLLONESHOT
#define TPOLLONESHOT EPOLLONESHOT
    EPOLLET = (1 << 31)
#define EPOLLET EPOLLET
#define TPOLLET EPOLLET
  };


#define TPOLL_MAX_FD			0x10000

#define TPOLL_SEQ_MASK			0xffff

#define TPOLL_GET_IDX(key)		( ((unsigned int)(key) & ~TPOLL_SEQ_MASK)>>16 )
#define TPOLL_GET_SEQ(key)		((key) & TPOLL_SEQ_MASK)
#define TPOLL_MAKE_KEY(idx,seq)	( ((idx) << 16) | ((seq) & TPOLL_SEQ_MASK) )

/* Valid opcodes ( "op" parameter ) to issue to epoll_ctl().  */
#define EPOLL_CTL_ADD		1	/* Add a file decriptor to the interface.  */
#define EPOLL_CTL_DEL		2	/* Remove a file decriptor from the interface.  */
#define EPOLL_CTL_MOD		3	/* Change file decriptor epoll_event structure.  */

#pragma pack(1)

typedef union epoll_data
{
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;

struct epoll_event
{
	uint32_t events;		/* Epoll events */
	ULONG_PTR ulKey;		/* ulKey */
	epoll_data_t data;		/* User data variable */
};

#pragma pack()

#endif

#define TPOLLIN 			EPOLLIN
#define TPOLLPRI 			EPOLLPRI
#define TPOLLOUT 			EPOLLOUT
#define TPOLLRDNORM 		EPOLLRDNORM
#define TPOLLRDBAND 		EPOLLRDBAND
#define TPOLLWRNORM 		EPOLLWRNORM
#define TPOLLWRBAND 		EPOLLWRBAND
#define TPOLLMSG 			EPOLLMSG
#define TPOLLERR 			EPOLLERR
#define TPOLLHUP 			EPOLLHUP
#define TPOLLONESHOT 		EPOLLONESHOT
#define TPOLLET 			EPOLLET

#define TPOLL_EVENTS		EPOLL_EVENTS

#define TPOLL_CTL_ADD		EPOLL_CTL_ADD
#define TPOLL_CTL_DEL		EPOLL_CTL_DEL
#define TPOLL_CTL_MOD		EPOLL_CTL_MOD

typedef epoll_data_t		tpoll_data_t;

typedef epoll_data_t		TPOLL_DATA;
typedef epoll_data_t		*LPTPOLL_DATA;
typedef epoll_data_t		*PTPOLL_DATA;


typedef struct epoll_event	epoll_event_t;
typedef struct epoll_event	tpoll_event_t;

typedef struct epoll_event	TPOLL_EVENT;
typedef struct epoll_event	*LPTPOLL_EVENT;
typedef struct epoll_event	*PTPOLL_EVENT;





#ifdef __cplusplus
extern "C"
{
#endif


/* PA Method Start. */

#if defined(_WIN32) || defined(_WIN64)

/** *******************************************************************
*	@brief  create a tpoll port descriptor.
*	@param[in] a_iSize the max number of descriptor the tpoll descriptor can hold.
*	@return the descriptor of the tpoll port.
*	@retval -1 for failure, usally it caused by a very large a_iSize.
*	@retval <>-1 for success, the descriptor of the tpoll port.
*********************************************************************/
	TEPOLL_HANDLE tpoll_create (IN int a_iSize);

/** *******************************************************************
*	@brief  add/remove/change the descriptor attached to the tpoll descriptor.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iOp the operation to be acted, there are three operation: TPOLL_CTL_ADD/TPOLL_CTL_MOD/TPOLL_CTL_DEL.
*	@param[in] a_iFd the file descriptor which to be added/deleted/modified.
*	@param[in] a_pstEvent which event of the a_iFd descriptor the tpoll port should take care.
*	@return whether the operation is succeeded.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
	int tpoll_ctl(IN TEPOLL_HANDLE a_iTpfd, IN int a_iOp, IN TSOCKET a_iFd, IN TPOLL_EVENT* a_pstEvent);

/** *******************************************************************
*	@brief  wait the events which the tpoll port caught.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[out] a_pstEvents the events array used for hold the event returned.
*	@param[in] a_iMaxEvents the max events the a_pstEvents array can hold.
*	@param[in] a_iTimeout the milli-seconds to wait before timeout.
*	@return whether the operation is succeeded, timeout or failure.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 timeout, no event returned.
*	@retval >0 the number of events returned.
*********************************************************************/
	int tpoll_wait(IN TEPOLL_HANDLE a_iTpfd, OUT TPOLL_EVENT* a_pstEvents, IN int a_iMaxEvents, IN int a_iTimeout);

/** *******************************************************************
*	@brief  destory the tpoll port.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
	int tpoll_destroy(IN TEPOLL_HANDLE a_iTpfd);

/** *******************************************************************
*	@brief  recieve data from the specified file descriptor.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the file descriptor for receiving data.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait().
*	@param[out] a_pszBuff the buffer for hold data received.
*	@param[in] a_iBuff the max size of buffer a_pszBuff.
*	@return the bytes received.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the bytes received.
*********************************************************************/
	int tpoll_recv(IN TEPOLL_HANDLE a_iTpfd, IN TSOCKET a_iFd, IN TPOLL_EVENT* a_pstEvent, OUT char* a_pszBuff, IN int a_iBuff);

/** *******************************************************************
*	@brief  send data to the specified file descriptor.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the file descriptor for sending data.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait(), if you do not use tpoll_wait for testing writability, you can just use send().
*	@param[in] a_pszBuff the buffer for hold data to send.
*	@param[in] a_iBuff the data size.
*	@return the bytes send.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the bytes send.
*********************************************************************/
	int tpoll_send(IN TEPOLL_HANDLE a_iTpfd, IN TSOCKET a_iFd, IN TPOLL_EVENT* a_pstEvent, IN char* a_pszBuff, IN int a_iBuff);

/** *******************************************************************
*	@brief  accept network connections.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the network descriptor for accepting connections.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait().
*	@param[out] a_pszAddr the buffer for hold the remote network address of the new connection.
*	@param[in, out] a_piLen the a_pszAddr buffer size as input, the data size of a_pszAddr buffer when returned.
*	@return the descriptor of the new connection.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the descriptor of the new connection.
*********************************************************************/
	TSOCKET tpoll_accept(IN TEPOLL_HANDLE a_iTpfd, IN TSOCKET a_iFd, IN TPOLL_EVENT* a_pstEvent, OUT struct sockaddr* a_pstAddr, INOUT int* a_piLen);

#else

/** *******************************************************************
*	@brief  create a tpoll port descriptor.
*	@param[in] a_iSize the max number of descriptor the tpoll descriptor can hold.
*	@return the descriptor of the tpoll port.
*	@retval -1 for failure, usally it caused by a very large a_iSize.
*	@retval <>-1 for success, the descriptor of the tpoll port.
*********************************************************************/
	#define tpoll_create(size)						epoll_create(size)

/** *******************************************************************
*	@brief  add/remove/change the descriptor attached to the tpoll descriptor.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iOp the operation to be acted, there are three operation: TPOLL_CTL_ADD/TPOLL_CTL_MOD/TPOLL_CTL_DEL.
*	@param[in] a_iFd the file descriptor which to be added/deleted/modified.
*	@param[in] a_pstEvent which event of the a_iFd descriptor the tpoll port should take care.
*	@return whether the operation is succeeded.
*	@retval -1 for failure.
*	@retval 0 for success.
*********************************************************************/
	#define tpoll_ctl(epfd, op, fd, ev)				epoll_ctl(epfd, op, fd, ev)

/** *******************************************************************
*	@brief  wait the events which the tpoll port caught.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[out] a_pstEvents the events array used for hold the event returned.
*	@param[in] a_iMaxEvents the max events the a_pstEvents array can hold.
*	@param[in] a_iTimeout the milli-seconds to wait before timeout.
*	@return whether the operation is succeeded, timeout or failure.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 timeout, no event returned.
*	@retval >0 the number of events returned.
*********************************************************************/
	#define tpoll_wait(epfd, evs, maxevs, timeout)	epoll_wait(epfd, evs, maxevs, timeout)

/** *******************************************************************
*	@brief  destory the tpoll port.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@return whether the operation is succeeded.
*	@retval -1 for failure, use errno for detail.
*	@retval 0 for success.
*********************************************************************/
	#define tpoll_destroy(epfd)						close(epfd)


/** *******************************************************************
*	@brief  recieve data from the specified file descriptor and get the peer info.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the file descriptor for receiving data.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait().
*	@param[out] a_pszBuff the buffer for hold data received.
*	@param[in] a_iBuff the max size of buffer a_pszBuff.
*	@param[out] a_pstSrcAddr the buffer for holding peer address.
*	@param[in,out] a_piSrcLen the size of the buffer for holding peer address.
*	@return the bytes received.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the bytes received.
*********************************************************************/
	int tpoll_recvfrom(TEPOLL_HANDLE a_iTpfd, TSOCKET a_iFd, struct epoll_event* a_pstEvent, char* a_pszBuff, int a_iBuff, struct sockaddr* a_pstSrcAddr, int* a_piSrcLen);


/** *******************************************************************
*	@brief  recieve data from the specified file descriptor.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the file descriptor for receiving data.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait().
*	@param[out] a_pszBuff the buffer for hold data received.
*	@param[in] a_iBuff the max size of buffer a_pszBuff.
*	@return the bytes received.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the bytes received.
*********************************************************************/
	int tpoll_recv(IN TEPOLL_HANDLE a_iTpfd, IN TSOCKET a_iFd, IN TPOLL_EVENT* a_pstEvent, OUT char* a_pszBuff, IN int a_iBuff);


/** *******************************************************************
*	@brief  send data using the specified file descriptor to the specified peer.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the file descriptor for sending data.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait(), if you do not use tpoll_wait for testing writability, you can just use send().
*	@param[in] a_pszBuff the buffer for hold data to send.
*	@param[in] a_iBuff the data size.
*	@param[in] a_pstDstAddr the buffer for holding the peer address.
*	@param[in] a_iDstLen the size of the buffer for holding the peer address.
*	@return the bytes send.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the bytes send.
*********************************************************************/
	int tpoll_sendto(TEPOLL_HANDLE a_iTpfd, TSOCKET a_iFd, TPOLL_EVENT* a_pstEvent, char* a_pszBuff, int a_iBuff, struct sockaddr* a_pstDst, int a_iDstLen);


/** *******************************************************************
*	@brief  send data using the specified file descriptor.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the file descriptor for sending data.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait(), if you do not use tpoll_wait for testing writability, you can just use send().
*	@param[in] a_pszBuff the buffer for hold data to send.
*	@param[in] a_iBuff the data size.
*	@return the bytes send.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the bytes send.
*********************************************************************/
	int tpoll_send(IN TEPOLL_HANDLE a_iTpfd, IN TSOCKET a_iFd, IN TPOLL_EVENT* a_pstEvent, IN char* a_pszBuff, IN int a_iBuff);

/** *******************************************************************
*	@brief  accept network connections.
*	@param[in] a_iTpfd  the descriptor of the tpoll port.
*	@param[in] a_iFd  the network descriptor for accepting connections.
*	@param[in] a_pstEvent the event which bind to the a_iFd descriptor and returned by tpoll_wait().
*	@param[out] a_pszAddr the buffer for hold the remote network address of the new connection.
*	@param[in, out] a_piLen the a_pszAddr buffer size as input, the data size of a_pszAddr buffer when returned.
*	@return the descriptor of the new connection.
*	@retval -1 for failure, use errno for detail.
*	@retval >=0 the descriptor of the new connection.
*********************************************************************/
	int tpoll_accept(IN TEPOLL_HANDLE a_iTpfd, IN TSOCKET a_iFd, IN TPOLL_EVENT* a_pstEvent, OUT struct sockaddr* a_pstAddr, INOUT int* a_piLen);


#endif /* #if defined(_WIN32) || defined(_WIN64) */

/* PA Method End. */


/* PS Method Start. */

#if defined(_WIN32) || defined(_WIN64)

	#define epoll_create(size)						tpoll_create(size)
	#define epoll_ctl(epfd, op, fd, ev)				tpoll_ctl(epfd, op, fd, ev)
	#define epoll_wait(epfd, evs, maxevs, timeout)	tpoll_wait(epfd, evs, maxevs, timeout)
	#define epoll_destroy(epfd)					 	tpoll_destroy(epfd)
	#define epoll_recv(epfd, fd, ev, buf, size) 	tpoll_recv(epfd, fd, ev, buf, size)
	#define epoll_recvfrom(epfd, fd, ev, buf, size, src, psrclen) 	tpoll_recvfrom(epfd, fd, ev, buf, size, src, psrclen)
	#define epoll_send(epfd, fd, ev, buf, size) 	tpoll_send(epfd, fd, ev, buf, size)
	#define epoll_sendto(epfd, fd, ev, buf, size, dst, dstlen) 	tpoll_sendto(epfd, fd, ev, buf, size, dst, dstlen)
	#define epoll_accept(epfd, fd, ev, addr, pcnt) 	tpoll_accept(epfd, fd, ev, addr, pcnt)

#else 	/* #if defined(_WIN32) || defined(_WIN64) */

	#define epoll_recv(epfd, fd, ev, buf, size) 	tpoll_recv(epfd, fd, ev, buf, size)
	#define epoll_recvfrom(epfd, fd, ev, buf, size, src, srclen) 	tpoll_recvfrom(epfd, fd, ev, buf, size, src, srclen)
	#define epoll_send(epfd, fd, ev, buf, size)		tpoll_send(epfd, fd, ev, buf, size)
	#define epoll_sendto(epfd, fd, ev, buf, size, dst, dstlen) 	tpoll_sendto(epfd, fd, ev, buf, size, dst, dstlen)
	#define epoll_accept(epfd, fd, ev, addr, pcnt) 	tpoll_accept(epfd, fd, ev, addr, pcnt)
	#define epoll_destroy(epfd)					 	tpoll_destroy(epfd)

#endif	/* #if defined(_WIN32) || defined(_WIN64) */

/* PS Method End. */

#ifdef __cplusplus
}
#endif

#endif /* TPOLL_H */


