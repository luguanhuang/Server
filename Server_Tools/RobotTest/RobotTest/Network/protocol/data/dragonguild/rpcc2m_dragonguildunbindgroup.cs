using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/13 20:19:53

namespace XMainClient
{
	using ArgDataType = KKSG.DragonGuildUnBindGroupArg;
    using ResDataType = KKSG.DragonGuildUnBindGroupRes;

    class RpcC2M_DragonGuildUnBindGroup : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_DragonGuildUnBindGroup()
        {
        }

        public override uint GetRpcType()
        {
            return 56553;
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
            Process_RpcC2M_DragonGuildUnBindGroup.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DragonGuildUnBindGroup.OnTimeout(oArg);
        }
    }
}
