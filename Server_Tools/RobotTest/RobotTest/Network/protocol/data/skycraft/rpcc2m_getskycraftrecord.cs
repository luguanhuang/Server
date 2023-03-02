using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 14:47:47

namespace XMainClient
{
	using ArgDataType = KKSG.GetSkyCraftRecordArg;
    using ResDataType = KKSG.GetSkyCraftRecordRes;

    class RpcC2M_GetSkyCraftRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetSkyCraftRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 39327;
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
            Process_RpcC2M_GetSkyCraftRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetSkyCraftRecord.OnTimeout(oArg);
        }
    }
}
