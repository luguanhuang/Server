using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/13 17:19:11

namespace XMainClient
{
	using ArgDataType = KKSG.QueryShopItemArg;
    using ResDataType = KKSG.QueryShopItemRes;

    class RpcC2G_QueryShopItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QueryShopItem()
        {
        }

        public override uint GetRpcType()
        {
            return 18079;
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
            Process_RpcC2G_QueryShopItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryShopItem.OnTimeout(oArg);
        }
    }
}
