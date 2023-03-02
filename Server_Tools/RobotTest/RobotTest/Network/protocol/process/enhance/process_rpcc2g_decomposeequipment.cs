using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/6/4 11:11:03

namespace XMainClient
{
    class Process_RpcC2G_DecomposeEquipment
    {
        public static void OnReply(DecomposeEquipmentArg oArg, DecomposeEquipmentRes oRes)
        {
            if (oRes.ErrorCode != ErrorCode.ERR_SUCCESS)
            {
            }
        }

        public static void OnTimeout(DecomposeEquipmentArg oArg)
        {
        }
    }
}
