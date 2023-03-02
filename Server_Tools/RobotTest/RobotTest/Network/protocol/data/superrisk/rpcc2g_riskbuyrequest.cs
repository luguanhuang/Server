using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/9 12:13:57

namespace XMainClient
{
	using ArgDataType = KKSG.RiskBuyRequestArg;
    using ResDataType = KKSG.RiskBuyRequestRes;

    class RpcC2G_RiskBuyRequest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_RiskBuyRequest()
        {
        }

        public override uint GetRpcType()
        {
            return 42935;
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
            Process_RpcC2G_RiskBuyRequest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_RiskBuyRequest.OnTimeout(oArg);
        }
    }
}
