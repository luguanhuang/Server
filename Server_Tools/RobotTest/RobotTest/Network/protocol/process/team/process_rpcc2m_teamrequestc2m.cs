using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2016/8/18 17:18:40

namespace XMainClient
{
    class Process_RpcC2M_TeamRequestC2M
    {
        public static void OnReply(TeamOPArg oArg, TeamOPRes oRes)
        {
            Connection conn = ConnectionMgr.CurrentConnection;
            TeamBattleTask task = ((TeamBattleTask)conn.state.GetCurrentTask());

            if (task == null)
                Console.WriteLine("" + conn.state.Account + " task error " + conn.state.GetCurrentTaskID());

            if (oRes.result != ErrorCode.ERR_SUCCESS)
            {
                Console.WriteLine(conn.state.Account + " team battle failed expID: " + task.expID + " state: " + task.GetState());
                Console.WriteLine(oRes.result.ToString());
                return;
            }
            if (task.GetState() == TeamState.REQ_CREATETEAM)
                task.SetState(TeamState.CREATE_TEAM);
            else if (task.GetState() == TeamState.TEAM_WAIT_START)
            {
                task.SetState(TeamState.TEAM_IN_BATTLE);

                /*PtcC2G_SceneLoadedNtf ntf = new PtcC2G_SceneLoadedNtf();
                conn.state.Send(ntf);*/

                /*string[] arrays = { "1_1", "whiteimmortal", "blackimmortal", "notice", "1_4xiaokulou2", "1_4xiaokulou1", "1_2", "xiaokulou", 
                                   "xiaokulou1", "tishi", "2_2cutscene", "zhaohuan", "part1", "part2", "part3", "fake1", "fake2", "2_8cutscene"};
                PtcC2G_AddLevelScriptExtString extSend = new PtcC2G_AddLevelScriptExtString();
                for (int i = 0; i < arrays.Length; ++i)
                {
                    extSend.Data.extString = arrays[i];
                    conn.state.Send(extSend);
                }*/
            }
        }

        public static void OnTimeout(TeamOPArg oArg)
        {
        }
    }
}
