using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/13 17:13:46

namespace XMainClient
{
	using ArgDataType = KKSG.ItemSellArg;
    using ResDataType = KKSG.ItemSellRes;

    class RpcC2G_ItemSell : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_ItemSell()
        {
        }

        public override uint GetRpcType()
        {
            return 34826;
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
            Process_RpcC2G_ItemSell.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ItemSell.OnTimeout(oArg);
        }
    }
}
