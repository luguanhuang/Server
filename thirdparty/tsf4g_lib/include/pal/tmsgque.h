/*
**  @file $RCSfile: tmsgque.h,v $
**  general description of this module
**  $Id: tmsgque.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TMSGQUE_H

#define TMSGQUE_H

#if defined(_WIN32) || defined(_WIN64)

#ifdef __cplusplus
extern "C"
{
#endif

#define MSG_NOERROR		010000  /* no error if message is too big */
#define MSG_EXCEPT		020000  /* recv any msg except of specified type */

/* ipcs ctl commands */
# define MSG_STAT 11
# define MSG_INFO 12

/* Types used in the structure definition.  */
typedef unsigned long int msgqnum_t;
typedef unsigned long int msglen_t;

struct msqid_ds
{
	struct ipc_perm msg_perm;     /* Ownership and permissions */
	time_t          msg_stime;    /* Time of last msgsnd() */
	time_t          msg_rtime;    /* Time of last msgrcv() */
	time_t          msg_ctime;    /* Time of last change */
	unsigned long   __msg_cbytes; /* Current number of bytes in
								  queue (non-standard) */
	msgqnum_t       msg_qnum;     /* Current number of messages
								  in queue */
	msglen_t        msg_qbytes;   /* Maximum number of bytes
								  allowed in queue */
	pid_t           msg_lspid;    /* PID of last msgsnd() */
	pid_t           msg_lrpid;    /* PID of last msgrcv() */

};

struct msgbuf
{
	long mtype;     /* message type, must be > 0 */
	char mtext[1];  /* message data */
};

/* buffer for msgctl calls IPC_INFO, MSG_INFO */
struct msginfo
{
	int msgpool;
	int msgmap;
	int msgmax;
	int msgmnb;
	int msgmni;
	int msgssz;
	int msgtql;
	unsigned short int msgseg;
};


int msgget(key_t key, int msgflg);

int msgctl(int msqid, int cmd, struct msqid_ds *buf);

int msgsnd(int msqid, struct msgbuf *msgp, size_t msgsz, int msgflg);

ssize_t msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflg);


#ifdef __cplusplus
}
#endif

#endif


#endif /* TMSGQUE_H */
