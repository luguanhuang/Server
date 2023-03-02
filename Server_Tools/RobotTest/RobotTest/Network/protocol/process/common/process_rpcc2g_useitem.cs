using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;

// generate by ProtoGen at date: 2015/6/12 10:21:12

namespace XMainClient
{
    class Process_RpcC2G_UseItem
    {
        public static void OnReply(UseItemArg oArg, UseItemRes oRes)
        {
            if (oRes.ErrorCode != ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] " + oRes.ErrorCode.ToString());
            }
            else
            {
                Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] use item: {0:X} OK", oArg.uid);
            }
        }

        public static void OnTimeout(UseItemArg oArg)
        {
        }
    }
}
