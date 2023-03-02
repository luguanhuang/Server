using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    class RandomMoveTask : Task
    {
        private static Random random = new Random();

        private double motionInterval = 0;
        private double duration = 1;
        private DateTime lastMoveTime = DateTime.Now;
        private DateTime enterTaskTime = DateTime.Now;

        public RandomMoveTask(double duration_ = 1.0)
        {
            duration = duration_ * random.NextDouble() + 1;
            motionInterval = 6.0 + 10.0 * random.NextDouble();
        }

        public override void OnStart()
        {
            enterTaskTime = DateTime.Now;

            Console.WriteLine("[{1}] Start Random Move for {0:F2} second", duration, state.Account);
        }

        public override void OnUpdate()
        {
            if (state.State == StateDef.EnterScene)
            {
                if (state.CurrentScene == 100)
                {
                    state.LeaveScene();
                    return;
                }

                if ((DateTime.Now - lastMoveTime).TotalSeconds > motionInterval)
                {
                    double x = 88 + Math.Cos(random.NextDouble() * 2 * Math.PI) * 3;
                    double z = 64 + Math.Sin(random.NextDouble() * 2 * Math.PI) * 3;
                    state.MoveTo(x, z);
                    lastMoveTime = DateTime.Now;
                }
            }
        }

        public override bool IsFinish()
        {
            return (DateTime.Now - enterTaskTime).TotalSeconds > duration;
        }
        public override void ResetTask()
        {
        }
    }
}
