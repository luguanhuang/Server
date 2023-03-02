using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/26 16:45:31

namespace XMainClient
{
	using ArgDataType = KKSG.SendGift2PlatFriendArg;
    using ResDataType = KKSG.SendGift2PlatFriendRes;

    class RpcC2M_SendGift2PlatFriend : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_SendGift2PlatFriend()
        {
        }

        public override uint GetRpcType()
        {
            return 57764;
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
            Process_RpcC2M_SendGift2PlatFriend.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_SendGift2PlatFriend.OnTimeout(oArg);
        }
    }
}
