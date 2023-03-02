using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/26 12:04:08

namespace XMainClient
{
	using ArgDataType = KKSG.ReqPlatFriendRankListArg;
    using ResDataType = KKSG.ReqPlatFriendRankListRes;

    class RpcC2M_ReqPlatFriendRankList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ReqPlatFriendRankList()
        {
        }

        public override uint GetRpcType()
        {
            return 43806;
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
            Process_RpcC2M_ReqPlatFriendRankList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqPlatFriendRankList.OnTimeout(oArg);
        }
    }
}
