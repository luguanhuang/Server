using System;
using KKSG;
// generate by ProtoGen at date: 2015/1/21 10:44:49

namespace XMainClient
{
    class Process_RpcC2G_SkillLevelup
    {
        public static void OnReply(SkillLevelupArg oArg, SkillLevelupRes oRes)
        {
            if (oRes.errorcode == (int)ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("skill level up ok :" + oArg.skillHash);
            }
            else
            {
            }
        }

        public static void OnTimeout(SkillLevelupArg oArg)
        {
        }
    }
}
