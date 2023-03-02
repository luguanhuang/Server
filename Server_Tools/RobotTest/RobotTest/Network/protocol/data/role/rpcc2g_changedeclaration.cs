using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/7 14:20:22

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeDeclarationArg;
    using ResDataType = KKSG.ChangeDeclarationRes;

    class RpcC2G_ChangeDeclaration : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ChangeDeclaration()
        {
        }

        public override uint GetRpcType()
        {
            return 1588;
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
            Process_RpcC2G_ChangeDeclaration.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ChangeDeclaration.OnTimeout(oArg);
        }
    }
}
