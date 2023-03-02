using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/29 11:26:20

namespace XMainClient
{
	using ArgDataType = KKSG.QueryBoxsArg;
    using ResDataType = KKSG.QueryBoxsRes;

    class RpcC2G_QueryBoxs : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_QueryBoxs()
        {
        }

        public override uint GetRpcType()
        {
            return 12558;
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
            Process_RpcC2G_QueryBoxs.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryBoxs.OnTimeout(oArg);
        }
    }
}
