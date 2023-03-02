using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/5 17:28:48

namespace XMainClient
{
	using ArgDataType = KKSG.CreateOrJoinDragonGuildArg;
    using ResDataType = KKSG.CreateOrJoinDragonGuildRes;

    class RpcC2M_CreateOrJoinDragonGuild : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_CreateOrJoinDragonGuild()
        {
        }

        public override uint GetRpcType()
        {
            return 8623;
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
            Process_RpcC2M_CreateOrJoinDragonGuild.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_CreateOrJoinDragonGuild.OnTimeout(oArg);
        }
    }
}
