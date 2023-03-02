using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2016/7/10 10:44:23

namespace XMainClient
{
    class Process_RpcC2M_SelectRoleNew
    {
        public static void OnReply(SelectRoleNewArg oArg, SelectRoleNewRes oRes)
        {
            if (oRes.result != ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("[" + ConnectionMgr.CurrentConnection.state.Account + "] Error: " + oRes.result.ToString());
                return;
            }

            LoginTask task = ConnectionMgr.CurrentConnection.GetTask() as LoginTask;
            if (task == null)
            {
                return;
            }

            task.OnSelectRole(oRes.result/*, oRes.roleData*/);

            //oRes.roleData = new KKSG.RoleAllInfo();
        }

        public static void OnTimeout(SelectRoleNewArg oArg)
        {
        }
    }
}
