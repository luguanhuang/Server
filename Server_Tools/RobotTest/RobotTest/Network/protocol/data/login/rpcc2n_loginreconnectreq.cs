using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/21 17:36:53

namespace XMainClient
{
	using ArgDataType = KKSG.LoginReconnectReqArg;
    using ResDataType = KKSG.LoginReconnectReqRes;

    class RpcC2N_LoginReconnectReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2N_LoginReconnectReq()
        {
        }

        public override uint GetRpcType()
        {
            return 25422;
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
            Process_RpcC2N_LoginReconnectReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2N_LoginReconnectReq.OnTimeout(oArg);
        }
    }
}
