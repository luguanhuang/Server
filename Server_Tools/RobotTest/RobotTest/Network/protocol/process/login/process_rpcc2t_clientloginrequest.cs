using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2015/10/7 15:03:26

namespace XMainClient
{
    class Process_RpcC2T_ClientLoginRequest
    {
        public static void OnReply(LoginArg oArg, LoginRes oRes)
        {
            LoginTask task = ConnectionMgr.CurrentConnection.GetTask() as LoginTask;
            if (task != null)
            {
                if (oRes.accountData == null)
                {
                    Console.WriteLine("Process_RpcC2T_ClientLoginRequest load account data error");
                    return;
                }
                task.OnAccountData(oRes.accountData);
            }
        }

        public static void OnTimeout(LoginArg oArg)
        {
        }
    }
}
