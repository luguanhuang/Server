using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;

// generate by ProtoGen at date: 2015/6/1 14:41:05

namespace XMainClient
{
    class Process_RpcC2G_GMCommand
    {
        public static void OnReply(GMCmdArg oArg, GMCmdRes oRes)
        {
            //Console.WriteLine("gmcmd ok ");
            Connection conn = ConnectionMgr.CurrentConnection;
            GMCommandTask task = ((GMCommandTask)conn.state.GetCurrentTask());
            Console.WriteLine(""+ conn.state.Account + " gm task : " + conn.state.GetCurrentTaskID());
            //if(oRes.result)
                task.SetFinish();
            //else
            //  Console.WriteLine(conn.state.Account + " gmcmd error " + task.GetCmd());
        }

        public static void OnTimeout(GMCmdArg oArg)
        {
        }
    }
}
