using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/8 19:46:33

namespace XMainClient
{
	using ArgDataType = KKSG.GetGuildCheckinRecordsArg;
    using ResDataType = KKSG.GetGuildCheckinRecordsRes;

    class RpcC2G_GetGuildCheckinRecords : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetGuildCheckinRecords()
        {
        }

        public override uint GetRpcType()
        {
            return 51906;
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
            Process_RpcC2G_GetGuildCheckinRecords.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetGuildCheckinRecords.OnTimeout(oArg);
        }
    }
}
