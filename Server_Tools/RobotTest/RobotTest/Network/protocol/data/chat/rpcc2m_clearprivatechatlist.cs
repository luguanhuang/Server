using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/26 10:05:34

namespace XMainClient
{
	using ArgDataType = KKSG.ClearPrivateChatListArg;
    using ResDataType = KKSG.ClearPrivateChatListRes;

    class RpcC2M_ClearPrivateChatList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ClearPrivateChatList()
        {
        }

        public override uint GetRpcType()
        {
            return 27304;
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
            Process_RpcC2M_ClearPrivateChatList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ClearPrivateChatList.OnTimeout(oArg);
        }
    }
}
