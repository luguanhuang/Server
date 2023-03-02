using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/3 14:45:00

namespace XMainClient
{
	using ArgDataType = KKSG.GetUnitAppearanceArg;
    using ResDataType = KKSG.GetUnitAppearanceRes;

    class RpcC2G_GetUnitAppearance : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetUnitAppearance()
        {
        }

        public override uint GetRpcType()
        {
            return 40537;
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
            Process_RpcC2G_GetUnitAppearance.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetUnitAppearance.OnTimeout(oArg);
        }
    }
}
