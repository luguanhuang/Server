using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/7 15:03:26

namespace XMainClient
{
	using ArgDataType = KKSG.LoginArg;
    using ResDataType = KKSG.LoginRes;

    class RpcC2T_ClientLoginRequest : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2T_ClientLoginRequest()
        {
        }

        public override uint GetRpcType()
        {
            return 10091;
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
            Process_RpcC2T_ClientLoginRequest.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2T_ClientLoginRequest.OnTimeout(oArg);
        }
    }
}
