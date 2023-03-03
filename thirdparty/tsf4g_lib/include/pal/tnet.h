/*
**  @file $RCSfile: tnet.h,v $
**  general description of this module
**  $Id: tnet.h,v 1.3 2009-09-28 07:52:15 hardway Exp $
**  @author $Author: hardway $
**  @date $Date: 2009-09-28 07:52:15 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef TNET_H
#define TNET_H

#include "pal/tsocket.h"

#define TNET_WAIT_INFINITE		WAIT_INFINITE

#define TNET_CHAR_ARG			'?'
#define TNET_CHAR_EQ			'='
#define TNET_CHAR_SEP			'&'
#define TNET_CHAR_PORT			':'
#define TNET_CHAR_ESC			'%'

#define TNET_STR_PROTO			"://"
#define TNET_STR_NUMBER			"1234567890."

#define TNET_PROTO_TCP			"tcp"
#define TNET_PROTO_UDP			"udp"
#define TNET_PROTO_UNIX			"unix"
#define TNET_PROTO_PIPE			"pipe"
#define TNET_PROTO_FILE			"file"
#define TNET_PROTO_SHM			"shm"
#define TNET_PROTO_THTTP			"thttp"

#define TNET_ID_TCP				1
#define TNET_ID_UDP				2
#define TNET_ID_UNIX			3
#define TNET_ID_PIPE			4
#define TNET_ID_FILE			5
#define TNET_ID_SHM				6
#define TNET_ID_THTTP				7

#define TNET_ARG_REUSE			"reuse"
#define TNET_ARG_TYPE			"type"

#define TNET_TYPE_STREAM		"stream"
#define TNET_TYPE_DGRAM			"dgram"
#define TNET_TYPE_SEQPACKET		"seqpacket"
#define TNET_TYPE_RDM			"rdm"
#define TNET_TYPE_RAW			"raw"
#define TNET_TYPE_PACKET		"packet"

#define TNET_DEF_PROTO			TNET_PROTO_TCP

#define HTONS(x)				htons((x))
#define HTONL(x)				htonl((x))
#define NTOHS(x)				ntohs((x))
#define NTOHL(x)				ntohl((x))

#define TNET_PROTO_LEN			16
#define TNET_ADDR_LEN			128
#define TNET_NAME_LEN			32
#define TNET_VALUE_LEN			256
#define TNET_MAX_ARGS			16
#define TNET_BUFF_LEN			65535

struct tagTnetArg
{
	char szName[TNET_NAME_LEN];
	char szValue[TNET_VALUE_LEN];
};

typedef struct tagTnetArg		TNETARG;

struct tagTnetOpt
{
	char szProto[TNET_PROTO_LEN];
	char szAddr[TNET_ADDR_LEN];
	int iArgs;
	TNETARG	args[TNET_MAX_ARGS];
};

typedef struct tagTnetOpt		TNETOPT;

struct tagTnetProtoDef
{
	char* pszName;
	int iID;
	int fSelectable;
	int fAvail;
};

typedef struct tagTnetProtoDef		TNETPROTODEF;

struct tagTnetTypeDef
{
	char* pszType;
	int iType;
};

typedef struct tagTnetTypeDef		TNETTYPEDEF;

union uniTnetAddr
{
	struct sockaddr_in stIn;

#if !defined(_WIN32) && !defined(_WIN64)
	struct sockaddr_un stUn;
#endif
};

typedef union uniTnetAddr			TNETADDR;

#ifdef __cplusplus
extern "C"
{
#endif


/** *******************************************************************
*	@brief judge whether the string is all composed by digit.
*	@param[in] a_pszHost the source string which need judge.
*	@return whether the string is all composed by digit.
*	@retval 1 if the string is all composed by digit.
*	@retval 0 if not.
*********************************************************************/
int tnet_is_number(IN const char* a_pszHost);


/** *******************************************************************
*	@brief convert a address string to struct sockaddr_in.
*	@param[in] a_pszAddr the address string to convert.
*	@param[out] a_pstIn the buffer to hold the struct sockaddr_in which is converted to.
*	@return whether the convert is succeeded.
*	@retval 0 if success.
*	@retval -1 if failure.
*********************************************************************/
int tnet_str2inet(IN const char* a_pszAddr, OUT struct sockaddr_in* a_pstIn);

#if !defined(_WIN32) && !defined(_WIN64)
/** *******************************************************************
*	@brief convert a address string to struct sockaddr_un.
*	@param[in] a_pszAddr the address string to convert.
*	@param[out] a_pstUn the buffer to hold the struct sockaddr_un which is converted to.
*	@return whether the convert is succeeded.
*	@retval 0 if success.
*	@retval -1 if failure.
*********************************************************************/
int tnet_str2unix(IN const char* a_pszAddr, OUT struct sockaddr_un* a_pstUn);
#endif





/** *******************************************************************
*	@brief  find prototype definition from name.
*	@param[in] a_pszProto the name of the prototype.
*	@return the prototype definition.
*	@retval NULL if the named prototype not found.
*	@retval !NULL pointer to prototype definition.
*********************************************************************/
const TNETPROTODEF* tnet_find_proto(IN const char* a_pszProto);



/** *******************************************************************
*	@brief  find socket type definition from name.
*	@param[in] a_pszType the name of the socket type.
*	@return the prototype definition.
*	@retval NULL if the named socket type not found.
*	@retval !NULL pointer to socket type definition.
*********************************************************************/
const TNETTYPEDEF* tnet_find_type(IN const char* a_pszType);



/** *******************************************************************
*	@brief  find argument from the TNETOPT structure.
*	@param[in] a_pstOpt the TNETOPT struct buffer, which is the source used to find argument.
*	@param[in] a_pszName the name of the argument.
*	@return the argument definition found.
*	@retval NULL if the named argument not found.
*	@retval !NULL pointer to the argument definition found.
*********************************************************************/
const TNETARG* tnet_find_arg(IN TNETOPT* a_pstOpt, IN const char* a_pszName);



/** *******************************************************************
*	@brief  make option list from the a URI string.
*	@param[out]  a_pstOpt the buffer for contain the option list.
*	@param[in]  a_pszUri the URI string.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int tnet_get_opt(OUT TNETOPT* a_pstOpt, IN const char* a_pszUri);		/* get the option from the uri string, this is a helper, needn't use normally. */

/* some flags can set. */

/** *******************************************************************
*	@brief  set the nonblock mode for the communication descriptor.
*	@param[in]  a_iSock the communication descriptor.
*	@param[in]  a_iIsNonblocking Input, the non-blocking flags: 1 for yes, 0 for no.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int tnet_set_nonblock(IN TSOCKET a_iSock, IN int a_iIsNonblocking);			/* set the socket to be nonblock or not. */

/** *******************************************************************
*	@brief  set the send buffer for the communication descriptor.
*	@param[in]  a_iSock the communication descriptor.
*	@param[in]  a_iSize	the send buffer size.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int tnet_set_recvbuff(TSOCKET a_iSock, int a_iSize);



/** *******************************************************************
*	@brief  set the recv buffer for the communication descriptor.
*	@param[in]  a_iSock the communication descriptor.
*	@param[in]  a_iSize	the recv buffer size.
*	@return whether this operation is succeeded.
*	@retval 0 for success.
*	@retval -1 for failure.
*********************************************************************/
int tnet_set_sendbuff(TSOCKET a_iSock, int a_iSize);


/* create/destroy the socket. */

/** *******************************************************************
*	@brief  open a communication descriptor by a URI string. the URI like that: tcp://127.0.0.1:8888?reuse=1
*	@param[in]  a_pszUri the URI string used to create the communication descriptor.
*	@return the communication descriptor.
*	@retval  -1 if failed.
*	@retval  the communication descriptor if success.
*********************************************************************/
TSOCKET tnet_open(IN const char* a_pszUri);				/* create a socket use uri info. */



/** *******************************************************************
*	@brief  create a communication descriptor by a URI string and listen on it. the URI like that: tcp://127.0.0.1:8888?reuse=1
*	@param[in]  a_pszUri the URI string used to create the communication descriptor.
*	@param[in]  a_iBacklog the internal pre-finish listening queue length.
*	@return the communication descriptor.
*	@retval  -1 if failed.
*	@retval  the communication descriptor if success.
*********************************************************************/
TSOCKET tnet_listen(IN const char* a_pszUri, IN int a_iBacklog);		/* create a socket for listen. */



/** *******************************************************************
*	@brief  open a communication descriptor by a URI string and do connect. the URI like that: tcp://127.0.0.1:8888?reuse=1
*	@param[in]  a_pszUri the URI string used to create the communication descriptor.
*	@param[in]  a_iTimeout the max milliseconds can be used to wait for the finsishing of connection.
*	@return the communication descriptor.
*	@retval  -1 if failed.
*	@retval  the communication descriptor if success.
*********************************************************************/
TSOCKET tnet_connect(IN const char* a_pszUri, IN int a_iTimeout);		/* create a socket, and connect to the uri specified. */



/** *******************************************************************
*	@brief  accept a pre-finished connection on a listening descriptor.
*	@param[in] a_iSock the listening descriptor which the connection is created on.
*	@param[out] a_pstAddr the buffer used to hold the connection's source address.
*	@param[in,out] a_piLen it hold the buffer Length as input, actually data length on return.
*	@param[in] a_iTimeout  the max milliseconds can be used to accept the pre-finished connection.
*	@return the accepted communication descriptor.
*	@retval  -1 if failed.
*	@retval  the accepted communication descriptor if success.
*********************************************************************/
TSOCKET tnet_accept(IN TSOCKET a_iSock, OUT struct sockaddr* a_pstAddr, INOUT int* a_piLen, IN int a_iTimeout); /* accept a income connection. */



/** *******************************************************************
*	@brief  close a communication descriptor.
*	@param[in] a_iSock the communication descriptor which need to be closed.
*	@return whether this operation is succeeded.
*	@retval 0 if success.
*	@retval -1 if failure.
*********************************************************************/
int tnet_close(IN TSOCKET a_iSock);						/* close a socket. */


/* send/recv socket operations. */

/** *******************************************************************
*	@brief  send data through the specified communication descriptor.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[in]  a_pszBuff the buffer which used to hold data to send.
*	@param[in]  a_iLen the length of the data to send.
*	@param[in]  a_iTimeout the max milliseconds can be used to send the data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data sent, by bytes.
*********************************************************************/
int tnet_send(IN TSOCKET a_iSock, IN const char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);



/** *******************************************************************
*	@brief  receive data from the specified communication descriptor.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[out]  a_pszBuff the buffer which used to hold data received.
*	@param[in]  a_iLen the length of the buffer.
*	@param[in]  a_iTimeout the max milliseconds can be used to receivethe data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data received, by bytes.
*********************************************************************/
int tnet_recv(IN TSOCKET a_iSock, OUT char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);


/* sendall/recvall socket operations. */

/** *******************************************************************
*	@brief  send data through the specified communication descriptor as more as possible in the specified timeout period.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[in]  a_pszBuff the buffer which used to hold data to send.
*	@param[in]  a_iLen the length of the data to send.
*	@param[in]  a_iTimeout the max milliseconds can be used to send the data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data sent, by bytes.
*********************************************************************/
int tnet_sendall(IN TSOCKET a_iSock, IN const char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);



/** *******************************************************************
*	@brief  receive data from the specified communication descriptor as more as possible in the specified timeout period.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[out]  a_pszBuff the buffer which used to hold data received.
*	@param[in]  a_iLen the length of the buffer.
*	@param[in]  a_iTimeout the max milliseconds can be used to receivethe data.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data received, by bytes.
*********************************************************************/
int tnet_recvall(IN TSOCKET a_iSock, OUT char* a_pszBuff, IN int a_iLen, IN int a_iTimeout);

/* read/write operations. */


/** *******************************************************************
*	@brief  write data to the specified descriptor, it is a blocking operation.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[in]  a_pszBuff the buffer which used to hold data.
*	@param[in]  a_iLen the length of the data to write.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data written, by bytes.
*********************************************************************/
int tnet_write(IN HANDLE a_iSock, IN const char* a_pszBuff, IN int a_iLen);


/** ******************************************************************
*	@brief  read data from the specified descriptor, it is a blocking operation.
*	@param[in]  a_iSock the communcation descriptor.
*	@param[out]  a_pszBuff the buffer which used to hold data read.
*	@param[in]  a_iLen the length of the buffer.
*	@return whether this operation is succeeded.
*	@retval -1 if failure.
*	@retval >=0 the data read, by bytes.
*********************************************************************/
int tnet_read(IN HANDLE a_iSock, OUT char* a_pszBuff, IN int a_iLen);

#ifdef __cplusplus
}
#endif

#endif /* TNET_H */

