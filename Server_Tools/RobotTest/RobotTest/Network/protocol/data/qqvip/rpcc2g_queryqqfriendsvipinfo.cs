using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/4 20:26:56

namespace XMainClient
{
	using ArgDataType = KKSG.QueryQQFriendsVipInfoArg;
    using ResDataType = KKSG.QueryQQFriendsVipInfoRes;

    class RpcC2G_QueryQQFriendsVipInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_QueryQQFriendsVipInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 11531;
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
            Process_RpcC2G_QueryQQFriendsVipInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QueryQQFriendsVipInfo.OnTimeout(oArg);
        }
    }
}
