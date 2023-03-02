using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/21 15:34:48

namespace XMainClient
{
	using ArgDataType = KKSG.QueryClientIpArg;
    using ResDataType = KKSG.QueryClientIpRes;

    class RpcC2G_QueryClientIp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_QueryClientIp()
        {
        }

        public override uint GetRpcType()
        {
            return 24918;
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
            Process_RpcC2G_QueryClientIp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryClientIp.OnTimeout(oArg);
        }
    }
}
