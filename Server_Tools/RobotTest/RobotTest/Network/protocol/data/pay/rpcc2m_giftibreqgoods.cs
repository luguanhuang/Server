using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/5 13:34:18

namespace XMainClient
{
	using ArgDataType = KKSG.GiftIbReqGoodsArg;
    using ResDataType = KKSG.GiftIbReqGoodsRes;

    class RpcC2M_GiftIbReqGoods : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GiftIbReqGoods()
        {
        }

        public override uint GetRpcType()
        {
            return 18140;
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
            Process_RpcC2M_GiftIbReqGoods.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GiftIbReqGoods.OnTimeout(oArg);
        }
    }
}
