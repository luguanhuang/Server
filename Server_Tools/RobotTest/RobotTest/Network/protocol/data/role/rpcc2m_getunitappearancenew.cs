using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/25 18:49:00

namespace XMainClient
{
	using ArgDataType = KKSG.GetUnitAppearanceArg;
    using ResDataType = KKSG.GetUnitAppearanceRes;

    class RpcC2M_GetUnitAppearanceNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetUnitAppearanceNew()
        {
        }

        public override uint GetRpcType()
        {
            return 40593;
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
            Process_RpcC2M_GetUnitAppearanceNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetUnitAppearanceNew.OnTimeout(oArg);
        }
    }
}
