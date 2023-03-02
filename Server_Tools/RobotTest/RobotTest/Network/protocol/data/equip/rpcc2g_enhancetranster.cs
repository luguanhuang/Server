using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/27 0:04:58

namespace XMainClient
{
	using ArgDataType = KKSG.EnhanceTransterArg;
    using ResDataType = KKSG.EnhanceTransterRes;

    class RpcC2G_EnhanceTranster : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_EnhanceTranster()
        {
        }

        public override uint GetRpcType()
        {
            return 25778;
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
            Process_RpcC2G_EnhanceTranster.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_EnhanceTranster.OnTimeout(oArg);
        }
    }
}
