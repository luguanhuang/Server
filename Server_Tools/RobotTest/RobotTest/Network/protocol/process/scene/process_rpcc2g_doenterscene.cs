using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using KKSG;
// generate by ProtoGen at date: 2016/7/13 19:09:40

namespace XMainClient
{
    class Process_RpcC2G_DoEnterScene
    {
        public static void OnReply(DoEnterSceneArg oArg, DoEnterSceneRes oRes)
        {
            ConnectionMgr.CurrentConnection.state.State = StateDef.EnterScene;
            if (ConnectionMgr.CurrentConnection.state.GetCurrentTask() == null)
            {
                return;
            }

            if (ConnectionMgr.CurrentConnection.state.GetCurrentTask().GetTaskType() == TaskType.TYPE_ENTERSCENE)
            {
                EnterSceneTask task = (EnterSceneTask)(ConnectionMgr.CurrentConnection.state.GetCurrentTask());
                task.Finish();
            }
            if (ConnectionMgr.CurrentConnection.state.GetCurrentTask().GetTaskType() == TaskType.TYPE_SINGLEBATTLETASK)
            {
                EnterBattleTask task = (EnterBattleTask)(ConnectionMgr.CurrentConnection.state.GetCurrentTask());
                if (task.GetBattleState() == BattleState.WAIT_LEAVE)
                    task.Finish();
            }
            if (ConnectionMgr.CurrentConnection.state.GetCurrentTask().GetTaskType() == TaskType.TYPE_TEAMTASK)
            {
                TeamBattleTask task = (TeamBattleTask)(ConnectionMgr.CurrentConnection.state.GetCurrentTask());
                if (task.GetState() == TeamState.TEAM_IN_BATTLE || task.GetState() == TeamState.TEAM_WAIT_START)
                {
                    string[] arrays = { "1_1", "whiteimmortal", "blackimmortal", "notice", "1_4xiaokulou2", "1_4xiaokulou1", "1_2", "xiaokulou", 
                                       "xiaokulou1", "tishi", "2_2cutscene", "zhaohuan", "part1", "part2", "part3", "fake1", "fake2", "2_8cutscene"};
                    PtcC2G_AddLevelScriptExtString extSend = new PtcC2G_AddLevelScriptExtString();
                    for (int i = 0; i < arrays.Length; ++i)
                    {
                        extSend.Data.extString = arrays[i];
                        ConnectionMgr.CurrentConnection.state.Send(extSend);
                    }

                }
                if (task.GetState() == TeamState.TEAM_BATTLE_END_CHANGESCENE)
                    task.Finish();
            }
        }

        public static void OnTimeout(DoEnterSceneArg oArg)
        {
        }
    }
}
