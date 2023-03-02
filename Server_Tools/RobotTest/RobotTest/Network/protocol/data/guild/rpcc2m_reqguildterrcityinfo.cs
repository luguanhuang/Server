using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/26 16:50:40

namespace XMainClient
{
	using ArgDataType = KKSG.ReqGuildTerrCityInfoArg;
    using ResDataType = KKSG.ReqGuildTerrCityInfo;

    class RpcC2M_ReqGuildTerrCityInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_ReqGuildTerrCityInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 47229;
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
            Process_RpcC2M_ReqGuildTerrCityInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ReqGuildTerrCityInfo.OnTimeout(oArg);
        }
    }
}
