﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/17 16:36:23

namespace XMainClient
{
	using ArgDataType = KKSG.GetWeddingInviteInfoArg;
    using ResDataType = KKSG.GetWeddingInviteInfoRes;

    class RpcC2M_GetWeddingInviteInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetWeddingInviteInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 2804;
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
            Process_RpcC2M_GetWeddingInviteInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetWeddingInviteInfo.OnTimeout(oArg);
        }
    }
}
