using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/22 15:23:29

namespace XMainClient
{
	using ArgDataType = KKSG.HeroBattleReqArg;
    using ResDataType = KKSG.HeroBattleReqRes;

    class RpcC2M_HeroBattleReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_HeroBattleReq()
        {
        }

        public override uint GetRpcType()
        {
            return 47628;
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
            Process_RpcC2M_HeroBattleReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_HeroBattleReq.OnTimeout(oArg);
        }
    }
}
