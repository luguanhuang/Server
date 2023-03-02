using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/18 11:11:26

namespace XMainClient
{
	using ArgDataType = KKSG.ShowFlowerPageArg;
    using ResDataType = KKSG.ShowFlowerPageRes;

    class RpcC2M_ShowFlowerPageNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ShowFlowerPageNew()
        {
        }

        public override uint GetRpcType()
        {
            return 49446;
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
            Process_RpcC2M_ShowFlowerPageNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ShowFlowerPageNew.OnTimeout(oArg);
        }
    }
}
