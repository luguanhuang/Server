using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/9 15:42:52

namespace XMainClient
{
	using ArgDataType = KKSG.GetTShowRewardArg;
    using ResDataType = KKSG.GetTShowRewardRes;

    class RpcC2G_GetTShowReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetTShowReward()
        {
        }

        public override uint GetRpcType()
        {
            return 9419;
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
            Process_RpcC2G_GetTShowReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetTShowReward.OnTimeout(oArg);
        }
    }
}
