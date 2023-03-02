using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    enum BattleState
    {
        START,
        IN_BATTLE,
        WAIT_LEAVE,
        FINISH,
    }

    class BattleResult : Task
    {
        public BattleResult() 
        {
        }

        public override void OnStart()
        {
            RpcC2G_ReportBattle oPtc = new RpcC2G_ReportBattle();
            oPtc.oArg.battledata.hppercent = 1;
            oPtc.oArg.battledata.timespan = 10;
            state.Send(oPtc);
        }

        public override void OnUpdate()
        {
        }

        public override bool IsFinish()
        {
            return true;
        }
        public override void ResetTask()
        {

        }
    }

    class EnterBattleTask : Task
    {
        BattleState btstate;
        uint SceneID;
        double duration;
        DateTime enterTime;

        static Random rd = new Random();

        public EnterBattleTask(uint id)
        {
            //duration = rd.NextDouble() * 1 + 0.5;
            //duration = rd.NextDouble() * 2 + 5;
            duration = rd.NextDouble() * 60 + 50;
            SceneID = id;
            taskType = TaskType.TYPE_SINGLEBATTLETASK;
        }

        public override void OnStart()
        {
            state.EnterScene(SceneID);
            btstate = BattleState.START;
            Console.WriteLine("start "+state.Account + " scene battle: " + SceneID);
        }

        public override void OnUpdate()
        {
            if (state.State == StateDef.EnterScene)
            {
                if (btstate == BattleState.FINISH)
                {

                }
                else if (btstate == BattleState.WAIT_LEAVE)
                {
                    //btstate = BattleState.FINISH;
                }
                else if (btstate == BattleState.IN_BATTLE)
                {
                    if ((DateTime.Now - enterTime).TotalSeconds > duration)
                    {
                        // LeaveScene 会把 state.State 设置为 StateDef.LeaveScene
                        state.LeaveScene();
                        btstate = BattleState.WAIT_LEAVE;
                    }
                }
                else if (btstate == BattleState.START)
                {
                    enterTime = DateTime.Now;
                    btstate = BattleState.IN_BATTLE;
                }
            }
        }

        public override bool IsFinish()
        {
            return (btstate == BattleState.FINISH);
        }
        public override void ResetTask()
        {
            btstate = BattleState.START;
        }

        public BattleState GetBattleState()
        {
            return btstate;
        }

        public void Finish()
        {
            btstate = BattleState.FINISH;
        }
    }
}
