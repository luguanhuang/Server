using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/5 10:03:12

namespace XMainClient
{
	using ArgDataType = KKSG.GetWeeklyTaskRewardArg;
    using ResDataType = KKSG.GetWeeklyTaskRewardRes;

    class RpcC2G_GetWeeklyTaskReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetWeeklyTaskReward()
        {
        }

        public override uint GetRpcType()
        {
            return 30588;
        }

        public override void Serialize(MemoryStream stream)
        {
            Serializer.Serialize(stream, oArg);
        }

        public override void DeSerialize(MemoryStream stream)
        {
            oRes = Serializer.Deserialize<ResDataType>(stream);
        }

        public override void Process()
        {
            base.Process();
            Process_RpcC2G_GetWeeklyTaskReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetWeeklyTaskReward.OnTimeout(oArg);
        }
    }
}
