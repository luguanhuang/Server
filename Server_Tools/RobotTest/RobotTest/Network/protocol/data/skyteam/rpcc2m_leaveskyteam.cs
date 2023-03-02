using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/31 22:15:25

namespace XMainClient
{
	using ArgDataType = KKSG.LeaveSkyTeamArg;
    using ResDataType = KKSG.LeaveSkyTeamRes;

    class RpcC2M_LeaveSkyTeam : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_LeaveSkyTeam()
        {
        }

        public override uint GetRpcType()
        {
            return 26181;
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
            Process_RpcC2M_LeaveSkyTeam.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_LeaveSkyTeam.OnTimeout(oArg);
        }
    }
}
