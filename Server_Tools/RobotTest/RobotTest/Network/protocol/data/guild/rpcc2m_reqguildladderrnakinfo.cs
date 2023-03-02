using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/30 11:27:13

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildLadderRnakInfoArg;
    using ResDataType = KKSG.ReqGuildLadderRnakInfoRes;

    class RpcC2M_ReqGuildLadderRnakInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ReqGuildLadderRnakInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 39925;
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
            Process_RpcC2M_ReqGuildLadderRnakInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildLadderRnakInfo.OnTimeout(oArg);
        }
    }
}
