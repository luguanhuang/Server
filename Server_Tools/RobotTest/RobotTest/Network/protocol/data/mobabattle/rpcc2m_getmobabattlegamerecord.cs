using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/16 0:20:27

namespace XMainClient
{
	using ArgDataType = KKSG.GetMobaBattleGameRecordArg;
    using ResDataType = KKSG.GetMobaBattleGameRecordRes;

    class RpcC2M_GetMobaBattleGameRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMobaBattleGameRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 9583;
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
            Process_RpcC2M_GetMobaBattleGameRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMobaBattleGameRecord.OnTimeout(oArg);
        }
    }
}
