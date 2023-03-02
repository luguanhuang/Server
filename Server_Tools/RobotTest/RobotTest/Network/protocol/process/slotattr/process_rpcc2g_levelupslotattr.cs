using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2016/3/22 15:54:35

namespace XMainClient
{
    class Process_RpcC2G_LevelUpSlotAttr
    {
        public static void OnReply(LevelUpSlotAttrArg oArg, LevelUpSlotAttrRes oRes)
        {
            if (oRes.errorcode == (int)ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("levelup slotattr ok slot:"+oArg.slot + " count:"+ oArg.count +" result:"+ oRes.uplevels);
            }
        }

        public static void OnTimeout(LevelUpSlotAttrArg oArg)
        {
        }
    }
}
