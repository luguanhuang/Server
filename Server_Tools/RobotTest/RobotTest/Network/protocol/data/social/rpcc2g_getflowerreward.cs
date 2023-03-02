﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 17:04:39

namespace XMainClient
{
	using ArgDataType = KKSG.GetFlowerRewardArg;
    using ResDataType = KKSG.GetFlowerRewardRes;

    class RpcC2G_GetFlowerReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetFlowerReward()
        {
        }

        public override uint GetRpcType()
        {
            return 65090;
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
            Process_RpcC2G_GetFlowerReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetFlowerReward.OnTimeout(oArg);
        }
    }
}
