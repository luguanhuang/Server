using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/24 21:06:10

namespace XMainClient
{
	using ArgDataType = KKSG.MarriageRelationOpArg;
    using ResDataType = KKSG.MarriageRelationOpRes;

    class RpcC2M_MarriageRelationOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_MarriageRelationOp()
        {
        }

        public override uint GetRpcType()
        {
            return 24966;
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
            Process_RpcC2M_MarriageRelationOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_MarriageRelationOp.OnTimeout(oArg);
        }
    }
}
