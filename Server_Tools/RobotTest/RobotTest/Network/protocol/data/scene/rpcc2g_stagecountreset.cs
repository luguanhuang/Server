using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/8 14:21:24

namespace XMainClient
{
	using ArgDataType = KKSG.StageCountResetArg;
    using ResDataType = KKSG.StageCountResetRes;

    class RpcC2G_StageCountReset : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_StageCountReset()
        {
        }

        public override uint GetRpcType()
        {
            return 8496;
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
            Process_RpcC2G_StageCountReset.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_StageCountReset.OnTimeout(oArg);
        }
    }
}
