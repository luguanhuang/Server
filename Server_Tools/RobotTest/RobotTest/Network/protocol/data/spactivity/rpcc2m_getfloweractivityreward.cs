using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/7 15:14:13

namespace XMainClient
{
	using ArgDataType = KKSG.GetFlowerActivityRewardArg;
    using ResDataType = KKSG.GetFlowerActivityRewardRes;

    class RpcC2M_GetFlowerActivityReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetFlowerActivityReward()
        {
        }

        public override uint GetRpcType()
        {
            return 36979;
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
            Process_RpcC2M_GetFlowerActivityReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetFlowerActivityReward.OnTimeout(oArg);
        }
    }
}
