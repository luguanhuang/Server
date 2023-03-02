using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/1 19:46:26

namespace XMainClient
{
	using ArgDataType = KKSG.GoalAwardsGetAwards_C2M;
    using ResDataType = KKSG.GoalAwardsGetAwards_M2C;

    class RpcC2M_GoalAwardsGetAwards : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GoalAwardsGetAwards()
        {
        }

        public override uint GetRpcType()
        {
            return 4985;
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
            Process_RpcC2M_GoalAwardsGetAwards.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GoalAwardsGetAwards.OnTimeout(oArg);
        }
    }
}
