using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/27 10:34:46

namespace XMainClient
{
	using ArgDataType = KKSG.LeaveGuildArg;
    using ResDataType = KKSG.LeaveGuildRes;

    class RpcC2M_LeaveFromGuild : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_LeaveFromGuild()
        {
        }

        public override uint GetRpcType()
        {
            return 2565;
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
            //Process_RpcC2M_LeaveFromGuild.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            //Process_RpcC2M_LeaveFromGuild.OnTimeout(oArg);
        }
    }
}
