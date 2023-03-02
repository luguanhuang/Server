using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/13 11:39:39

namespace XMainClient
{
	using ArgDataType = KKSG.GroupChatFindRoleInfoListC2S;
    using ResDataType = KKSG.GroupChatFindRoleInfoListS2C;

    class RpcC2M_GroupChatFindRoleInfoList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GroupChatFindRoleInfoList()
        {
        }

        public override uint GetRpcType()
        {
            return 7283;
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
            Process_RpcC2M_GroupChatFindRoleInfoList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GroupChatFindRoleInfoList.OnTimeout(oArg);
        }
    }
}
