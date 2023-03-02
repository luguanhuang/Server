using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/16 19:30:02

namespace XMainClient
{
	using ArgDataType = KKSG.FetchAchiveArg;
    using ResDataType = KKSG.FetchAchiveRes;

    class RpcC2G_FetchAchivementReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_FetchAchivementReward()
        {
        }

        public override uint GetRpcType()
        {
            return 47094;
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
            Process_RpcC2G_FetchAchivementReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_FetchAchivementReward.OnTimeout(oArg);
        }
    }
}
