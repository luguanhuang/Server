using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;

// generate by ProtoGen at date: 2015/7/20 10:15:31

namespace XMainClient
{
    class Process_RpcC2G_BuyGoldAndFatigue
    {
        public static void OnReply(BuyGoldAndFatigueArg oArg, BuyGoldAndFatigueRes oRes)
        {
            if (oRes.result != ErrorCode.ERR_SUCCESS)
            {
            }
            else
            {
            }
        }

        public static void OnTimeout(BuyGoldAndFatigueArg oArg)
        {
        }
    }
}
