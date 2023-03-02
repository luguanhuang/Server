﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/11 17:37:09

namespace XMainClient
{
	using ArgDataType = KKSG.AuctReqOnSalePara;
    using ResDataType = KKSG.AuctionReqResult;

    class RpcC2G_AuctonReqOnSale : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AuctonReqOnSale()
        {
        }

        public override uint GetRpcType()
        {
            return 40577;
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
            Process_RpcC2G_AuctonReqOnSale.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AuctonReqOnSale.OnTimeout(oArg);
        }
    }
}
