using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/26 16:34:00

namespace XMainClient
{
	using ArgDataType = KKSG.WeddingOperatorArg;
    using ResDataType = KKSG.WeddingOperatorRes;

    class RpcC2G_WeddingOperator : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_WeddingOperator()
        {
        }

        public override uint GetRpcType()
        {
            return 38050;
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
            Process_RpcC2G_WeddingOperator.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_WeddingOperator.OnTimeout(oArg);
        }
    }
}
