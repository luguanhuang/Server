using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2014/11/7 14:07:57

namespace XMainClient
{
    class Process_RpcC2G_SyncTime
    {

        public static void OnReply(SyncTimeArg oArg, SyncTimeRes oRes)
        {
            //Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] sync time reply");
            ConnectionMgr.CurrentConnection.state.SetServerTime(oRes.serverTime);
        }

        public static void OnTimeout(SyncTimeArg oArg)
        {
        }
    }
}
