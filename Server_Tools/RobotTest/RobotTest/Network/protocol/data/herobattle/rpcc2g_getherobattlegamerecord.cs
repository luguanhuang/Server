using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/27 15:37:44

namespace XMainClient
{
	using ArgDataType = KKSG.GetHeroBattleGameRecordArg;
    using ResDataType = KKSG.GetHeroBattleGameRecordRes;

    class RpcC2G_GetHeroBattleGameRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_GetHeroBattleGameRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 41057;
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
            Process_RpcC2G_GetHeroBattleGameRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetHeroBattleGameRecord.OnTimeout(oArg);
        }
    }
}
