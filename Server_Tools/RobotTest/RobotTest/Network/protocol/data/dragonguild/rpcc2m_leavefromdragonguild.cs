using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/7 17:24:34

namespace XMainClient
{
	using ArgDataType = KKSG.LeaveDragonGuildArg;
    using ResDataType = KKSG.LeaveDragonGuildRes;

    class RpcC2M_LeaveFromDragonGuild : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_LeaveFromDragonGuild()
        {
        }

        public override uint GetRpcType()
        {
            return 9882;
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
            Process_RpcC2M_LeaveFromDragonGuild.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_LeaveFromDragonGuild.OnTimeout(oArg);
        }
    }
}
