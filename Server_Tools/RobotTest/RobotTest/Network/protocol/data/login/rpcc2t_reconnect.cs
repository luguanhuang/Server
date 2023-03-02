using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/4 15:41:43

namespace XMainClient
{
	using ArgDataType = KKSG.ReconnArg;
    using ResDataType = KKSG.ReconnRes;

    class RpcC2T_Reconnect : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2T_Reconnect()
        {
        }

        public override uint GetRpcType()
        {
            return 28358;
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
            Process_RpcC2T_Reconnect.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2T_Reconnect.OnTimeout(oArg);
        }
    }
}
