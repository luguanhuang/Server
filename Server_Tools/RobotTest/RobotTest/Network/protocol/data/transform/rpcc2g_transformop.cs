using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/9 14:10:51

namespace XMainClient
{
	using ArgDataType = KKSG.TransformOpArg;
    using ResDataType = KKSG.TransformOpRes;

    class RpcC2G_TransformOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_TransformOp()
        {
        }

        public override uint GetRpcType()
        {
            return 7373;
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
            Process_RpcC2G_TransformOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TransformOp.OnTimeout(oArg);
        }
    }
}
