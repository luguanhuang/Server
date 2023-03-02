﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/10 14:27:19

namespace XMainClient
{
	using ArgDataType = KKSG.dareRewardArg;
    using ResDataType = KKSG.dareRewardRes;

    class RpcC2G_dareReward : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_dareReward()
        {
        }

        public override uint GetRpcType()
        {
            return 24389;
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
            Process_RpcC2G_dareReward.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_dareReward.OnTimeout(oArg);
        }
    }
}
