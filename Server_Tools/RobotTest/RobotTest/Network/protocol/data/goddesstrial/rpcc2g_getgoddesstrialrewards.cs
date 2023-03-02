using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/18 20:04:49

namespace XMainClient
{
	using ArgDataType = KKSG.GetGoddessTrialRewardsArg;
    using ResDataType = KKSG.GetGoddessTrialRewardsRes;

    class RpcC2G_GetGoddessTrialRewards : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetGoddessTrialRewards()
        {
        }

        public override uint GetRpcType()
        {
            return 41420;
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
            Process_RpcC2G_GetGoddessTrialRewards.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGoddessTrialRewards.OnTimeout(oArg);
        }
    }
}
