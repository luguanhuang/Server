using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/14 22:14:27

namespace XMainClient
{
	using ArgDataType = KKSG.GetLeagueEleInfoArg;
    using ResDataType = KKSG.GetLeagueEleInfoRes;

    class RpcC2M_GetLeagueEleInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetLeagueEleInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 40678;
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
            Process_RpcC2M_GetLeagueEleInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetLeagueEleInfo.OnTimeout(oArg);
        }
    }
}
