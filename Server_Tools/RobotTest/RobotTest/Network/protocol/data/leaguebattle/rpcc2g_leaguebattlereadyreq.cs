using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/11 16:51:08

namespace XMainClient
{
	using ArgDataType = KKSG.LeagueBattleReadyReqArg;
    using ResDataType = KKSG.LeagueBattleReadyReqRes;

    class RpcC2G_LeagueBattleReadyReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_LeagueBattleReadyReq()
        {
        }

        public override uint GetRpcType()
        {
            return 15873;
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
            Process_RpcC2G_LeagueBattleReadyReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_LeagueBattleReadyReq.OnTimeout(oArg);
        }
    }
}
