using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/30 12:07:27

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeOutLookOpArg;
    using ResDataType = KKSG.ChangeOutLookOpRes;

    class RpcC2G_ChangeOutLookOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ChangeOutLookOp()
        {
        }

        public override uint GetRpcType()
        {
            return 56978;
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
            Process_RpcC2G_ChangeOutLookOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeOutLookOp.OnTimeout(oArg);
        }
    }
}
