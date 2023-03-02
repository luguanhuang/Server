using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/28 22:23:17

namespace XMainClient
{
	using ArgDataType = KKSG.GMCmdArg;
    using ResDataType = KKSG.GMCmdRes;

    class RpcC2G_GMCommand : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2G_GMCommand()
        {
        }

        public override uint GetRpcType()
        {
            return 3248;
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
            Process_RpcC2G_GMCommand.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_GMCommand.OnTimeout(oArg);
        }
    }
}
