using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/2 10:26:17

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildBonusSendListArg;
    using ResDataType = KKSG.GetGuildBonusSendListRes;

    class RpcC2M_GetGuildBonusSendList : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GetGuildBonusSendList()
        {
        }

        public override uint GetRpcType()
        {
            return 59719;
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
            Process_RpcC2M_GetGuildBonusSendList.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetGuildBonusSendList.OnTimeout(oArg);
        }
    }
}
