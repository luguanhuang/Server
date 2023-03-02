using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/24 11:42:17

namespace XMainClient
{
	using ArgDataType = KKSG.OrderCreateArg;
    using ResDataType = KKSG.OrderCreateRes;

    class RpcC2M_OrderCreate : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_OrderCreate()
        {
        }

        public override uint GetRpcType()
        {
            return 10957;
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
            Process_RpcC2M_OrderCreate.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_OrderCreate.OnTimeout(oArg);
        }
    }
}
