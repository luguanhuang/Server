using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/21 10:43:26

namespace XMainClient
{
	using ArgDataType = KKSG.ClearGuildTerrAllianceArg;
    using ResDataType = KKSG.ClearGuildTerrAllianceRes;

    class RpcC2M_ClearGuildTerrAlliance : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ClearGuildTerrAlliance()
        {
        }

        public override uint GetRpcType()
        {
            return 38312;
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
            Process_RpcC2M_ClearGuildTerrAlliance.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ClearGuildTerrAlliance.OnTimeout(oArg);
        }
    }
}
