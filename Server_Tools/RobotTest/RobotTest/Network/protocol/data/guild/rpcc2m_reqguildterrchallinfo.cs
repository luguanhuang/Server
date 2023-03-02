using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/16 20:22:56

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildTerrChallInfoArg;
    using ResDataType = KKSG.ReqGuildTerrChallInfoRes;

    class RpcC2M_ReqGuildTerrChallInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ReqGuildTerrChallInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 9791;
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
            Process_RpcC2M_ReqGuildTerrChallInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildTerrChallInfo.OnTimeout(oArg);
        }
    }
}
