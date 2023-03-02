using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/13 20:17:00

namespace XMainClient
{
	using ArgDataType = KKSG.DragonGuildBindReq;
    using ResDataType = KKSG.DragonGuildBindRes;

    class RpcC2M_DragonGuildBindGroupReq : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_DragonGuildBindGroupReq()
        {
        }

        public override uint GetRpcType()
        {
            return 34774;
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
            Process_RpcC2M_DragonGuildBindGroupReq.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DragonGuildBindGroupReq.OnTimeout(oArg);
        }
    }
}
