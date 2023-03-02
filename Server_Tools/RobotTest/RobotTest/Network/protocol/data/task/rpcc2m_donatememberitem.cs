using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/19 23:28:13

namespace XMainClient
{
	using ArgDataType = KKSG.DonateMemberItemArg;
    using ResDataType = KKSG.DonateMemberItemRes;

    class RpcC2M_DonateMemberItem : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_DonateMemberItem()
        {
        }

        public override uint GetRpcType()
        {
            return 4241;
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
            Process_RpcC2M_DonateMemberItem.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DonateMemberItem.OnTimeout(oArg);
        }
    }
}
