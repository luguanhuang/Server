using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/10 18:14:08

namespace XMainClient
{
	using ArgDataType = KKSG.LeagueBattleReqArg;
    using ResDataType = KKSG.LeagueBattleReqRes;

    class RpcC2M_LeagueBattleReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_LeagueBattleReq()
        {
        }

        public override uint GetRpcType()
        {
            return 8012;
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
            Process_RpcC2M_LeagueBattleReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_LeagueBattleReq.OnTimeout(oArg);
        }
    }
}
