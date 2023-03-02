using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/27 22:19:52

namespace XMainClient
{
	using ArgDataType = KKSG.TaskOPArg;
    using ResDataType = KKSG.TaskOPRes;

    class RpcC2G_TaskOperate : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_TaskOperate()
        {
        }

        public override uint GetRpcType()
        {
            return 20029;
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
            Process_RpcC2G_TaskOperate.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_TaskOperate.OnTimeout(oArg);
        }
    }
}
