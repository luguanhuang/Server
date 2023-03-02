using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/9/16 15:23:58

namespace XMainClient
{
    class Process_RpcC2I_QueryGateIP
    {
        public static void OnReply(QueryGateArg oArg, QueryGateRes oRes)
        {
            LoginTask task = ConnectionMgr.CurrentConnection.GetTask() as LoginTask;
            if (task != null)
            {
                task.OnQueryGateIP(oRes);
            }
        }

        public static void OnTimeout(QueryGateArg oArg)
        {
        }
    }
}
