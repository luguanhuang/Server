using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 15:55:03

namespace XMainClient
{
	using ArgDataType = KKSG.GuildJoinBindGroupReq;
    using ResDataType = KKSG.GuildJoinBindGroupRes;

    class RpcC2M_GuildJoinBindGroup : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GuildJoinBindGroup()
        {
        }

        public override uint GetRpcType()
        {
            return 12928;
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
            Process_RpcC2M_GuildJoinBindGroup.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GuildJoinBindGroup.OnTimeout(oArg);
        }
    }
}
