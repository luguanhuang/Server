using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/5/30 12:44:55

namespace XMainClient
{
	using ArgDataType = KKSG.GetAchieveBrifInfoReq;
    using ResDataType = KKSG.GetAchieveBrifInfoRes;

    class RpcC2G_GetAchieveBrifInfoReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GetAchieveBrifInfoReq()
        {
        }

        public override uint GetRpcType()
        {
            return 25095;
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
            Process_RpcC2G_GetAchieveBrifInfoReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GetAchieveBrifInfoReq.OnTimeout(oArg);
        }
    }
}
