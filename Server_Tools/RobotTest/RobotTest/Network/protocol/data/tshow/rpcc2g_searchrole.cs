﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/9 14:35:47

namespace XMainClient
{
	using ArgDataType = KKSG.SearchRoleArg;
    using ResDataType = KKSG.SearchRoleRes;

    class RpcC2G_SearchRole : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_SearchRole()
        {
        }

        public override uint GetRpcType()
        {
            return 59396;
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
            Process_RpcC2G_SearchRole.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_SearchRole.OnTimeout(oArg);
        }
    }
}
