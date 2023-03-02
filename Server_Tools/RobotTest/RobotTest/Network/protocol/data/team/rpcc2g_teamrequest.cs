using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/15 17:05:14

namespace XMainClient
{
	using ArgDataType = KKSG.TeamOPArg;
    using ResDataType = KKSG.TeamOPRes;

    class RpcC2G_TeamRequest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_TeamRequest()
        {
        }

        public override uint GetRpcType()
        {
            return 44555;
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
            Process_RpcC2G_TeamRequest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TeamRequest.OnTimeout(oArg);
        }
    }
}
