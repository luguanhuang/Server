using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/26 19:24:41

namespace XMainClient
{
	using ArgDataType = KKSG.LoginActivityArg;
    using ResDataType = KKSG.LoginActivityRes;

    class RpcC2G_LoginActivity : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_LoginActivity()
        {
        }

        public override uint GetRpcType()
        {
            return 64167;
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
            Process_RpcC2G_LoginActivity.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_LoginActivity.OnTimeout(oArg);
        }
    }
}
