using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/28 13:15:08

namespace XMainClient
{
	using ArgDataType = KKSG.DragonGroupRecordC2S;
    using ResDataType = KKSG.DragonGroupRecordS2C;

    class RpcC2G_DragonGroupRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_DragonGroupRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 62181;
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
            Process_RpcC2G_DragonGroupRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_DragonGroupRecord.OnTimeout(oArg);
        }
    }
}
