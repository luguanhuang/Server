using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/7 19:31:27

namespace XMainClient
{
	using ArgDataType = KKSG.GetPayRewardArg;
    using ResDataType = KKSG.GetPayRewardRes;

    class RpcC2G_GetPayReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetPayReward()
        {
        }

        public override uint GetRpcType()
        {
            return 63038;
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
            Process_RpcC2G_GetPayReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetPayReward.OnTimeout(oArg);
        }
    }
}
