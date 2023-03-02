using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/5/27 19:51:17

namespace XMainClient
{
	using ArgDataType = KKSG.QuerySceneDayCountArg;
    using ResDataType = KKSG.QuerySceneDayCountRes;

    class RpcC2G_QuerySceneDayCount : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_QuerySceneDayCount()
        {
        }

        public override uint GetRpcType()
        {
            return 1676;
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
            Process_RpcC2G_QuerySceneDayCount.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_QuerySceneDayCount.OnTimeout(oArg);
        }
    }
}
