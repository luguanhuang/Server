using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/4/14 16:22:17


namespace XMainClient
{
    class Process_RpcC2G_BindSkill
    {
        public static void OnReply(BingSkillArg oArg, BindSkillRes oRes)
        {
            if (oRes.errorcode == ErrorCode.ERR_SUCCESS)
            {
               
            }
        }

        public static void OnTimeout(BingSkillArg oArg)
        {
        }
    }
}
