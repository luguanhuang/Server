using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/17 16:33:38

namespace XMainClient
{
	using ArgDataType = KKSG.JadeOperationArg;
    using ResDataType = KKSG.JadeOperationRes;

    class RpcC2G_JadeOperation : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_JadeOperation()
        {
        }

        public override uint GetRpcType()
        {
            return 55991;
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
            Process_RpcC2G_JadeOperation.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_JadeOperation.OnTimeout(oArg);
        }
    }
}
