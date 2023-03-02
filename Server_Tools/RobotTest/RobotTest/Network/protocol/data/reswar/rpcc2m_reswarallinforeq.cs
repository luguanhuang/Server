using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/1 17:21:37

namespace XMainClient
{
	using ArgDataType = KKSG.ResWarArg;
    using ResDataType = KKSG.ResWarRes;

    class RpcC2M_ResWarAllInfoReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ResWarAllInfoReq()
        {
        }

        public override uint GetRpcType()
        {
            return 23709;
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
            Process_RpcC2M_ResWarAllInfoReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ResWarAllInfoReq.OnTimeout(oArg);
        }
    }
}
