using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/20 16:19:11

namespace XMainClient
{
	using ArgDataType = KKSG.ActivateFashionArg;
    using ResDataType = KKSG.ActivateFashionRes;

    class RpcC2G_ActivateFashionCharm : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ActivateFashionCharm()
        {
        }

        public override uint GetRpcType()
        {
            return 58036;
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
            Process_RpcC2G_ActivateFashionCharm.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ActivateFashionCharm.OnTimeout(oArg);
        }
    }
}
