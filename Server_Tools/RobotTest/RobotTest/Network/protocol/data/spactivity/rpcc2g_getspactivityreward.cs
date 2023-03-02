using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/28 17:04:32

namespace XMainClient
{
	using ArgDataType = KKSG.GetSpActivityRewardArg;
    using ResDataType = KKSG.GetSpActivityRewardRes;

    class RpcC2G_GetSpActivityReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetSpActivityReward()
        {
        }

        public override uint GetRpcType()
        {
            return 7905;
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
            Process_RpcC2G_GetSpActivityReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetSpActivityReward.OnTimeout(oArg);
        }
    }
}
