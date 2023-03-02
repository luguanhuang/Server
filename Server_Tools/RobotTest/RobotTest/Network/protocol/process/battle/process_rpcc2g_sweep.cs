using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;

// generate by ProtoGen at date: 2015/7/13 14:28:32

namespace XMainClient
{
    class Process_RpcC2G_Sweep
    {
        public static void OnReply(SweepArg oArg, SweepRes oRes)
        {
            if (oRes.result != ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "]sweep failed: " + oRes.result.ToString());
            }
        }

        public static void OnTimeout(SweepArg oArg)
        {
        }
    }
}
