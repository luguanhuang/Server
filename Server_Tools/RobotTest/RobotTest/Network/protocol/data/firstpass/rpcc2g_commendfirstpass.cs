using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/24 17:24:53

namespace XMainClient
{
	using ArgDataType = KKSG.CommendFirstPassArg;
    using ResDataType = KKSG.CommendFirstPassRes;

    class RpcC2G_CommendFirstPass : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_CommendFirstPass()
        {
        }

        public override uint GetRpcType()
        {
            return 8467;
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
            Process_RpcC2G_CommendFirstPass.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_CommendFirstPass.OnTimeout(oArg);
        }
    }
}
