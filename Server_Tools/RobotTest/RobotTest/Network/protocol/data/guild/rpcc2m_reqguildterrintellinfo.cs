using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/8 20:47:23

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildTerrIntellInfoArg;
    using ResDataType = KKSG.ReqGuildTerrIntellInfoRes;

    class RpcC2M_ReqGuildTerrIntellInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ReqGuildTerrIntellInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 43276;
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
            Process_RpcC2M_ReqGuildTerrIntellInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildTerrIntellInfo.OnTimeout(oArg);
        }
    }
}
