using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/3 15:56:53

namespace XMainClient
{
	using ArgDataType = KKSG.ResWarPVEArg;
    using ResDataType = KKSG.ResWarPVERes;

    class RpcC2M_StartResWarPVE : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_StartResWarPVE()
        {
        }

        public override uint GetRpcType()
        {
            return 35398;
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
            Process_RpcC2M_StartResWarPVE.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_StartResWarPVE.OnTimeout(oArg);
        }
    }
}
