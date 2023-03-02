using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/13 20:18:29

namespace XMainClient
{
	using ArgDataType = KKSG.DragonGuildJoinBindGroupArg;
    using ResDataType = KKSG.DragonGuildJoinBindGroupRes;

    class RpcC2M_DragonGuildJoinBindGroup : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_DragonGuildJoinBindGroup()
        {
        }

        public override uint GetRpcType()
        {
            return 33949;
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
            Process_RpcC2M_DragonGuildJoinBindGroup.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DragonGuildJoinBindGroup.OnTimeout(oArg);
        }
    }
}
