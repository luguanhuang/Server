using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/16 14:26:01

namespace XMainClient
{
	using ArgDataType = KKSG.GetAchievePointRewardReq;
    using ResDataType = KKSG.GetAchievePointRewardRes;

    class RpcC2G_GetAchievePointRewardReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetAchievePointRewardReq()
        {
        }

        public override uint GetRpcType()
        {
            return 13722;
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
            Process_RpcC2G_GetAchievePointRewardReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetAchievePointRewardReq.OnTimeout(oArg);
        }
    }
}
