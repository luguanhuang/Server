using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2016/6/20 16:15:20

namespace XMainClient
{
    class Process_RpcC2A_UpLoadAudioReq
    {
        public static void OnReply(UpLoadAudioReq oArg, UpLoadAudioRes oRes)
        {
            if (oRes.result == ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine("do send audio msg :" + oRes.audioUid.ToString());
                Connection conn = ConnectionMgr.CurrentConnection;

                RpcC2M_chat chatReport = new RpcC2M_chat();
                chatReport.oArg.chatinfo = new KKSG.ChatInfo();
                chatReport.oArg.chatinfo.channel = 1;

                chatReport.oArg.chatinfo.info = "Robot chat info" + DateTime.Now.ToString();
                chatReport.oArg.chatinfo.param.Add(new KKSG.ChatParam());
                chatReport.oArg.chatinfo.issystem = false;
                chatReport.oArg.chatinfo.isRecruit = false;
                chatReport.oArg.chatinfo.audioUid = oRes.audioUid;
                chatReport.oArg.chatinfo.audioLen = 1500;
                conn.Send(chatReport);
            }
        }

        public static void OnTimeout(UpLoadAudioReq oArg)
        {
        }
    }
}
