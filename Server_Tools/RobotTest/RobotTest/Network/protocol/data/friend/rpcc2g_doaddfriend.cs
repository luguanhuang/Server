using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/23 21:11:03

namespace XMainClient
{
	using ArgDataType = KKSG.DoAddFriendArg;
    using ResDataType = KKSG.DoAddFriendRes;

    class RpcC2G_DoAddFriend : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_DoAddFriend()
        {
        }

        public override uint GetRpcType()
        {
            return 25510;
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
            Process_RpcC2G_DoAddFriend.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_DoAddFriend.OnTimeout(oArg);
        }
    }
}
