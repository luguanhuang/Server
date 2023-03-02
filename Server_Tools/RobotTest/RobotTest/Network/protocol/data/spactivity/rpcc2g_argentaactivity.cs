using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 14:48:30

namespace XMainClient
{
	using ArgDataType = KKSG.ArgentaActivityArg;
    using ResDataType = KKSG.ArgentaActivityRes;

    class RpcC2G_ArgentaActivity : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ArgentaActivity()
        {
        }

        public override uint GetRpcType()
        {
            return 838;
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
            Process_RpcC2G_ArgentaActivity.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ArgentaActivity.OnTimeout(oArg);
        }
    }
}
