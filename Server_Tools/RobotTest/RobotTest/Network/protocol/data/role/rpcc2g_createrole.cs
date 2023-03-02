﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/1 14:59:01

namespace XMainClient
{
	using ArgDataType = KKSG.CreateRoleArg;
    using ResDataType = KKSG.CreateRoleRes;

    class RpcC2G_CreateRole : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_CreateRole()
        {
        }

        public override uint GetRpcType()
        {
            return 38850;
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
            Process_RpcC2G_CreateRole.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_CreateRole.OnTimeout(oArg);
        }
    }
}
