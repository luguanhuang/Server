using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/10 11:56:09

namespace XMainClient
{
	using ArgDataType = KKSG.SetRoleAudioIdArg;
    using ResDataType = KKSG.SetRoleAudioIdRes;

    class RpcC2G_SetRoleAudioId : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SetRoleAudioId()
        {
        }

        public override uint GetRpcType()
        {
            return 64127;
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
            Process_RpcC2G_SetRoleAudioId.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SetRoleAudioId.OnTimeout(oArg);
        }
    }
}
