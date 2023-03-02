using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/18 16:13:54

namespace XMainClient
{
	using ArgDataType = KKSG.AddFriendArg;
    using ResDataType = KKSG.AddFriendRes;

    class RpcC2G_AddFriend : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_AddFriend()
        {
        }

        public override uint GetRpcType()
        {
            return 29531;
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
            Process_RpcC2G_AddFriend.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_AddFriend.OnTimeout(oArg);
        }
    }
}
