using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/17 18:07:20

namespace XMainClient
{
	using ArgDataType = KKSG.HoldWeddingReq;
    using ResDataType = KKSG.HoldWeddingRes;

    class RpcC2M_HoldWedding : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_HoldWedding()
        {
        }

        public override uint GetRpcType()
        {
            return 51875;
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
            Process_RpcC2M_HoldWedding.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_HoldWedding.OnTimeout(oArg);
        }
    }
}
