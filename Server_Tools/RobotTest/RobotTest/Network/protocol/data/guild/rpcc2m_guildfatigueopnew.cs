using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/19 10:29:50

namespace XMainClient
{
	using ArgDataType = KKSG.GuildFatigueArg;
    using ResDataType = KKSG.GuildFatigueRes;

    class RpcC2M_GuildFatigueOPNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_GuildFatigueOPNew()
        {
        }

        public override uint GetRpcType()
        {
            return 10226;
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
            Process_RpcC2M_GuildFatigueOPNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GuildFatigueOPNew.OnTimeout(oArg);
        }
    }
}
