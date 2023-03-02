using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/7 17:22:05

namespace XMainClient
{
	using ArgDataType = KKSG.DragonGuildApprovalArg;
    using ResDataType = KKSG.DragonGuildApprovalRes;

    class RpcC2M_DragonGuildApproval : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_DragonGuildApproval()
        {
        }

        public override uint GetRpcType()
        {
            return 4753;
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
            Process_RpcC2M_DragonGuildApproval.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DragonGuildApproval.OnTimeout(oArg);
        }
    }
}
