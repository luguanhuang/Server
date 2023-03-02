﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/18 16:13:55

namespace XMainClient
{
	using ArgDataType = KKSG.ShowFriendListArg;
    using ResDataType = KKSG.ShowFriendListRes;

    class RpcC2G_ShowFriendList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ShowFriendList()
        {
        }

        public override uint GetRpcType()
        {
            return 31560;
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
            Process_RpcC2G_ShowFriendList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ShowFriendList.OnTimeout(oArg);
        }
    }
}
