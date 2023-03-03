﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/30 16:45:47

namespace XMainClient
{
	using ArgDataType = KKSG.ItemFindBackArg;
    using ResDataType = KKSG.ItemFindBackRes;

    class RpcC2G_ItemFindBack : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ItemFindBack()
        {
        }

        public override uint GetRpcType()
        {
            return 60242;
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
            Process_RpcC2G_ItemFindBack.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ItemFindBack.OnTimeout(oArg);
        }
    }
}