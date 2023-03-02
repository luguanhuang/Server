using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/13 9:34:06

namespace XMainClient
{
	using ArgDataType = KKSG.GetLeagueBattleRecordArg;
    using ResDataType = KKSG.GetLeagueBattleRecordRes;

    class RpcC2M_GetLeagueBattleRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetLeagueBattleRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 51407;
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
            Process_RpcC2M_GetLeagueBattleRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetLeagueBattleRecord.OnTimeout(oArg);
        }
    }
}
