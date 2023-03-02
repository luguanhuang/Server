using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 21:21:53

namespace XMainClient
{
	using ArgDataType = KKSG.GetMyApplyMasterInfoArg;
    using ResDataType = KKSG.GetMyApplyMasterInfoRes;

    class RpcC2M_GetMyApplyMasterInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetMyApplyMasterInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 61902;
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
            Process_RpcC2M_GetMyApplyMasterInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetMyApplyMasterInfo.OnTimeout(oArg);
        }
    }
}
