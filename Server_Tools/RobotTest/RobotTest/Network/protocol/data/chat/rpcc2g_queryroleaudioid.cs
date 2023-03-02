using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/10 11:55:06

namespace XMainClient
{
	using ArgDataType = KKSG.QueryRoleAudioIdArg;
    using ResDataType = KKSG.QueryRoleAudioIdRes;

    class RpcC2G_QueryRoleAudioId : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QueryRoleAudioId()
        {
        }

        public override uint GetRpcType()
        {
            return 64026;
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
            Process_RpcC2G_QueryRoleAudioId.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryRoleAudioId.OnTimeout(oArg);
        }
    }
}
