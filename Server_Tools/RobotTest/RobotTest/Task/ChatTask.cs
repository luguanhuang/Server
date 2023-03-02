using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using XUtliPoolLib;
using System.IO;

namespace XMainClient
{
    class ChatTask : Task
    {
        float frequence = 1.0f;
        uint timertoken = 0;

        public ChatTask(string freq)
        {
            float.TryParse(freq, out frequence);
        }

        public override void OnStart()
        {
            OnSendVoiceMsg(null);
        }

        public void OnSendVoiceMsg(object obj)
        {
            timertoken = XTimerMgr.singleton.SetTimer(frequence, OnSendVoiceMsg, null);

            RpcC2A_UpLoadAudioReq msg = new RpcC2A_UpLoadAudioReq();
            msg.oArg.audio = File.ReadAllBytes("adelin1.mp3");
            msg.oArg.text = Encoding.UTF8.GetBytes("Auto send voice");

            state.Send(msg);
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
}