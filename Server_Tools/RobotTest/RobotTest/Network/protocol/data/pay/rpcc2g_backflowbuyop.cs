﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/4 15:47:17

namespace XMainClient
{
	using ArgDataType = KKSG.BackFlowBuyOpArg;
    using ResDataType = KKSG.BackFlowBuyOpRes;

    class RpcC2G_BackFlowBuyOp : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_BackFlowBuyOp()
        {
        }

        public override uint GetRpcType()
        {
            return 16261;
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
            Process_RpcC2G_BackFlowBuyOp.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_BackFlowBuyOp.OnTimeout(oArg);
        }
    }
}
