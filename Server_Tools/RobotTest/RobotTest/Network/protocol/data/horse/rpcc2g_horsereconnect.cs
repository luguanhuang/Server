using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/14 10:45:40

namespace XMainClient
{
	using ArgDataType = KKSG.HorseReConnectArg;
    using ResDataType = KKSG.HorseReConnectRes;

    class RpcC2G_HorseReConnect : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_HorseReConnect()
        {
        }

        public override uint GetRpcType()
        {
            return 7786;
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
            Process_RpcC2G_HorseReConnect.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_HorseReConnect.OnTimeout(oArg);
        }
    }
}
