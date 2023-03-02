using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/20 16:34:42

namespace XMainClient
{
	using ArgDataType = KKSG.QueryMayhemRankArg;
    using ResDataType = KKSG.QueryMayhemRankRes;

    class RpcC2M_QueryBigMeleeRank : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_QueryBigMeleeRank()
        {
        }

        public override uint GetRpcType()
        {
            return 33332;
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
            Process_RpcC2M_QueryBigMeleeRank.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_QueryBigMeleeRank.OnTimeout(oArg);
        }
    }
}
