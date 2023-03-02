using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/10 15:53:51

namespace XMainClient
{
	using ArgDataType = KKSG.GuildBindGroupReq;
    using ResDataType = KKSG.GuildBindGroupRes;

    class RpcC2M_GuildBindGroup : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GuildBindGroup()
        {
        }

        public override uint GetRpcType()
        {
            return 16003;
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
            Process_RpcC2M_GuildBindGroup.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GuildBindGroup.OnTimeout(oArg);
        }
    }
}
