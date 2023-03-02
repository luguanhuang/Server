using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/15 20:38:00

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildTerrAllianceInfoArg;
    using ResDataType = KKSG.ReqGuildTerrAllianceInfoRes;

    class RpcC2M_ReqGuildTerrAllianceInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ReqGuildTerrAllianceInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 63044;
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
            Process_RpcC2M_ReqGuildTerrAllianceInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildTerrAllianceInfo.OnTimeout(oArg);
        }
    }
}
