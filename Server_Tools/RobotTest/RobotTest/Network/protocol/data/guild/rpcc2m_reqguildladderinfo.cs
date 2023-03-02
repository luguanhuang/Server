using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/30 11:18:03

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildLadderInfoAgr;
    using ResDataType = KKSG.ReqGuildLadderInfoRes;

    class RpcC2M_ReqGuildLadderInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ReqGuildLadderInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 44006;
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
            Process_RpcC2M_ReqGuildLadderInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildLadderInfo.OnTimeout(oArg);
        }
    }
}
