using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/13 11:42:54

namespace XMainClient
{
	using ArgDataType = KKSG.GroupChatLeaderIssueInfoC2S;
    using ResDataType = KKSG.GroupChatLeaderIssueInfoS2C;

    class RpcC2M_GroupChatLeaderIssueInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GroupChatLeaderIssueInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 9594;
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
            Process_RpcC2M_GroupChatLeaderIssueInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GroupChatLeaderIssueInfo.OnTimeout(oArg);
        }
    }
}
