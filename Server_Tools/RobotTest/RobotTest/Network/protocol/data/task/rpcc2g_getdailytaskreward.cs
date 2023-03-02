using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/14 20:00:28

namespace XMainClient
{
	using ArgDataType = KKSG.GetDailyTaskRewardArg;
    using ResDataType = KKSG.GetDailyTaskRewardRes;

    class RpcC2G_GetDailyTaskReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetDailyTaskReward()
        {
        }

        public override uint GetRpcType()
        {
            return 59899;
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
            Process_RpcC2G_GetDailyTaskReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetDailyTaskReward.OnTimeout(oArg);
        }
    }
}
