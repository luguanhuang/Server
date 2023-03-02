using System;
using System.Collections.Generic;
using System.IO;

namespace XMainClient
{
    public class TaskChain
    {
        private int taskid;
        private List<Task> taskList;
        private ClientState state;

        public TaskChain(ClientState state_)
        {
            state = state_;
            taskList = new List<Task>();
            taskid = -1;
        }

        public void PushTask(Task task)
        {
            task.SetState(state);
            taskList.Add(task);
        }

        public Task GetCurrentTask()
        {
            if (taskid < taskList.Count)
            {
                return taskList[taskid];
            }

            return null;
        }

        public void ResetTask()
        {
            for (int i = 0; i < taskList.Count; ++i)
            {
                taskList[i].ResetTask();
            }
        }

        public int GetTaskStep()
        {
            return taskid;
        }

        public void SetTaskStep(int i)
        {
            taskid = i;
            StartTask();
        }

        public void StartTask()
        {
            Task task = GetCurrentTask();
            if (task != null)
            {
                task.OnStart();
            }
        }

        public void Update()
        {
            if (taskid == -1)
            {
                ++taskid;
                StartTask();
                return;
            }

            Task task = GetCurrentTask();
            if (task == null)
            {
                return;
            }

            task.Update();

            if (task.IsFinish())
            {
                ++taskid;
                StartTask();
            }
        }

        struct JumpLabel
        {
            public string label;
            public JumpToTask task;
        }

        public void LoadTaskScript(string scriptFile)
        {
            try
            {
                Dictionary<string, int> Labels = new Dictionary<string, int>();
                List<JumpLabel> needResolve = new List<JumpLabel>();

                string[] lines = File.ReadAllLines(scriptFile);
                for (int i = 0; i < lines.Length; ++i)
                {
                    if (lines[i].Length == 0)
                    {
                        continue;
                    }

                    if (lines[i][0] == '#' || lines[i][0] == '/')
                    {
                        continue;
                    }

                    string line = lines[i].Trim();
                    if (line.Length == 0)
                    {
                        continue;
                    }

                    string cmd = "";
                    string arg = "";
                    int st = line.IndexOf(' ');
                    if (st == -1)
                    {
                        cmd = line;
                    }
                    else
                    {
                        cmd = line.Substring(0, st);
                        arg = line.Substring(st + 1);
                    }

                    switch (cmd)
                    {
                        case "login": PushTask(new LoginTask(state.Account, arg)); PushTask(new SyncTimeTask()); break;
                        case "logout": PushTask(new LogoutTask());  break;
                        case "tutorial": PushTask(new UpdateTutorialTask(uint.Parse(arg))); break;
                        case "gmcmd": PushTask(new GMCommandTask(arg)); break;
                        case "rmove": PushTask(new RandomMoveTask(double.Parse(arg))); break;
                        case "task": PushTask(new TakeQuestTask(int.Parse(arg))); break;
                        case "taskfinish": PushTask(new FinishQuestTask(int.Parse(arg))); break;
                        case "battle": PushTask(new EnterBattleTask(uint.Parse(arg))); break;
                        case "sweep": PushTask(new SweepTask(arg)); break;
                        case "lottery": PushTask(new DrawLotteryTask(uint.Parse(arg))); break;
                        case "skillevelup": PushTask(new SkillLevelupTask(arg)); break;
                        case "skilllearnall": PushTask(new SkillLearnAllTask()); break;
                        case "skillbind": PushTask(new SkillBindTask(arg)); break;
                        case "use": PushTask(new UseItemTask(uint.Parse(arg))); break;
                        case "wearfashion": PushTask(new FashionOP(uint.Parse(arg), 2)); break;
                        case "label": Labels[arg.Trim()] = taskList.Count; break;
                        case "reinforce": PushTask(new ReinforceEquip(arg)); break;
                        case "battleresult": PushTask(new BattleResult()); break;
                        case "teambattle": PushTask(new TeamBattleTask(arg)); break;
                        case "enter": PushTask(new EnterSceneTask(arg)); break;
                        case "chat": PushTask(new ChatTask(arg)); break;
                        case "goto":
                            {
                                int gotoID;
                                arg = arg.Trim();
                                if (Labels.TryGetValue(arg, out gotoID))
                                {
                                    PushTask(new JumpToTask(gotoID));
                                }
                                else
                                {
                                    JumpLabel jl;
                                    jl.label = arg;
                                    jl.task = new JumpToTask(taskList.Count);
                                    PushTask(jl.task);
                                    needResolve.Add(jl);
                                }
                                break;
                            }
                        default: break;
                    }

                    foreach (JumpLabel jl in needResolve)
                    {
                        int gotoID;
                        if (Labels.TryGetValue(arg, out gotoID))
                        {
                            jl.task.SetJumpTo(gotoID);
                        }
                        else
                        {
                            Console.WriteLine("not found goto label:" + arg);
                        }
                    }
                }
            }
            catch(Exception e)
            {
                Console.WriteLine("open script " + scriptFile + " get exception: " + e.ToString());
            }
        }
    }
}
