using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 17:23:35

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildRankInfoArg;
    using ResDataType = KKSG.ReqGuildRankInfoRes;

    class RpcC2M_ReqGuildRankInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ReqGuildRankInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 48521;
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
            Process_RpcC2M_ReqGuildRankInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildRankInfo.OnTimeout(oArg);
        }
    }
}
