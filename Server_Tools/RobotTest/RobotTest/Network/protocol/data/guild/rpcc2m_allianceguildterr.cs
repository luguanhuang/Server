using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/18 10:40:00

namespace XMainClient
{
	using ArgDataType = KKSG.AllianceGuildTerrArg;
    using ResDataType = KKSG.AllianceGuildTerrRes;

    class RpcC2M_AllianceGuildTerr : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_AllianceGuildTerr()
        {
        }

        public override uint GetRpcType()
        {
            return 10041;
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
            Process_RpcC2M_AllianceGuildTerr.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_AllianceGuildTerr.OnTimeout(oArg);
        }
    }
}
