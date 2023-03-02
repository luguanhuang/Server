using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 15:20:32

namespace XMainClient
{
	using ArgDataType = KKSG.GetSkyCraftTeamInfoArg;
    using ResDataType = KKSG.GetSkyCraftTeamInfoRes;

    class RpcC2M_GetSkyCraftTeamInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetSkyCraftTeamInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 25015;
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
            Process_RpcC2M_GetSkyCraftTeamInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetSkyCraftTeamInfo.OnTimeout(oArg);
        }
    }
}
