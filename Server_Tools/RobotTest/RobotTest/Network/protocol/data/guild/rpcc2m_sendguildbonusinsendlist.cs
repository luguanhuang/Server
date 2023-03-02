using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/2 10:28:20

namespace XMainClient
{
	using ArgDataType = KKSG.SendGuildBonusInSendListArg;
    using ResDataType = KKSG.SendGuildBonusInSendListRes;

    class RpcC2M_SendGuildBonusInSendList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_SendGuildBonusInSendList()
        {
        }

        public override uint GetRpcType()
        {
            return 64498;
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
            Process_RpcC2M_SendGuildBonusInSendList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_SendGuildBonusInSendList.OnTimeout(oArg);
        }
    }
}
