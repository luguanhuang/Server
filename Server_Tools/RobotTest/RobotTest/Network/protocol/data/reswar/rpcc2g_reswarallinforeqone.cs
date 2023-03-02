using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/2 23:25:13

namespace XMainClient
{
	using ArgDataType = KKSG.ResWarArg;
    using ResDataType = KKSG.ResWarRes;

    class RpcC2G_ResWarAllInfoReqOne : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_ResWarAllInfoReqOne()
        {
        }

        public override uint GetRpcType()
        {
            return 8828;
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
            Process_RpcC2G_ResWarAllInfoReqOne.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_ResWarAllInfoReqOne.OnTimeout(oArg);
        }
    }
}
