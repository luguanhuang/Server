using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/15 10:32:39

namespace XMainClient
{
	using ArgDataType = KKSG.QueryPkInfoReq;
    using ResDataType = KKSG.QueryPkInfoRes;

    class RpcC2G_QueryPkInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QueryPkInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 23594;
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
            Process_RpcC2G_QueryPkInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryPkInfo.OnTimeout(oArg);
        }
    }
}
