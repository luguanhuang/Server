using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/10 14:34:41

namespace XMainClient
{
	using ArgDataType = KKSG.SmeltItemArg;
    using ResDataType = KKSG.SmeltItemRes;

    class RpcC2G_SmeltItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SmeltItem()
        {
        }

        public override uint GetRpcType()
        {
            return 10028;
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
            Process_RpcC2G_SmeltItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SmeltItem.OnTimeout(oArg);
        }
    }
}
