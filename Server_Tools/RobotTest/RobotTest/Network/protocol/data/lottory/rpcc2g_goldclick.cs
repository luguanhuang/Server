using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/31 20:53:00

namespace XMainClient
{
	using ArgDataType = KKSG.GoldClickArg;
    using ResDataType = KKSG.GoldClickRes;

    class RpcC2G_GoldClick : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GoldClick()
        {
        }

        public override uint GetRpcType()
        {
            return 12917;
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
            Process_RpcC2G_GoldClick.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GoldClick.OnTimeout(oArg);
        }
    }
}
