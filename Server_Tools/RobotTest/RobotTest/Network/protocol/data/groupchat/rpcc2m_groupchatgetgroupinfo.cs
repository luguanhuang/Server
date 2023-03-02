﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/13 11:46:57

namespace XMainClient
{
	using ArgDataType = KKSG.GroupChatGetGroupInfoC2S;
    using ResDataType = KKSG.GroupChatGetGroupInfoS2C;

    class RpcC2M_GroupChatGetGroupInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GroupChatGetGroupInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 64081;
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
            Process_RpcC2M_GroupChatGetGroupInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GroupChatGetGroupInfo.OnTimeout(oArg);
        }
    }
}
