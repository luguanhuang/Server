﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/7 19:45:56

namespace XMainClient
{
	using ArgDataType = KKSG.AtlasUpStarArg;
    using ResDataType = KKSG.AtlasUpStarRes;

    class RpcC2G_AtlasUpStar : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_AtlasUpStar()
        {
        }

        public override uint GetRpcType()
        {
            return 41051;
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
            Process_RpcC2G_AtlasUpStar.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AtlasUpStar.OnTimeout(oArg);
        }
    }
}
