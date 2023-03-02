using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/4/11 16:35:39

namespace XMainClient
{
	using ArgDataType = KKSG.TaskOPArg;
    using ResDataType = KKSG.TaskOPRes;

    class RpcC2G_TakeOperate : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_TakeOperate()
        {
        }

        public override uint GetRpcType()
        {
            return 33554;
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
            Process_RpcC2G_TakeOperate.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TakeOperate.OnTimeout(oArg);
        }
    }
}
