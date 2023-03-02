using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/25 13:15:43

namespace XMainClient
{
	using ArgDataType = KKSG.GroupChatLeaderAddRoleC2S;
    using ResDataType = KKSG.GroupChatLeaderAddRoleS2C;

    class RpcC2M_GroupChatLeaderAddRole : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GroupChatLeaderAddRole()
        {
        }

        public override uint GetRpcType()
        {
            return 44703;
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
            Process_RpcC2M_GroupChatLeaderAddRole.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GroupChatLeaderAddRole.OnTimeout(oArg);
        }
    }
}
