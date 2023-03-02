using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/20 18:39:10

namespace XMainClient
{
	using ArgDataType = KKSG.GetNextDayRewardArg;
    using ResDataType = KKSG.GetNextDayRewardRes;

    class RpcC2G_GetNextDayReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetNextDayReward()
        {
        }

        public override uint GetRpcType()
        {
            return 40997;
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
            Process_RpcC2G_GetNextDayReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetNextDayReward.OnTimeout(oArg);
        }
    }
}
