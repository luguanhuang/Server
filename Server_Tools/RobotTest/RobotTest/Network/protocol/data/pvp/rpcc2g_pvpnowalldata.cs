using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/27 10:43:57

namespace XMainClient
{
	using ArgDataType = KKSG.roArg;
    using ResDataType = KKSG.PvpNowGameData;

    class RpcC2G_PvpNowAllData : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_PvpNowAllData()
        {
        }

        public override uint GetRpcType()
        {
            return 58355;
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
            Process_RpcC2G_PvpNowAllData.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PvpNowAllData.OnTimeout(oArg);
        }
    }
}
