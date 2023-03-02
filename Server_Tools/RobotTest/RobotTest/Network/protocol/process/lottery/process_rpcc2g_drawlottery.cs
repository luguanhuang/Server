using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/5/13 11:42:37


namespace XMainClient
{
    class Process_RpcC2G_DrawLottery
    {
        public static void OnReply(DrawLotteryArg oArg, DrawLotteryRes oRes)
        {
            foreach (ItemBrief brief in oRes.Items)
            {
                Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] lottery item:" + brief.itemID);
            }
        }

        public static void OnTimeout(DrawLotteryArg oArg)
        {
        }
    }
}
