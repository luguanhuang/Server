using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/15 22:33:09

namespace XMainClient
{
	using ArgDataType = KKSG.ChangeGuildSettingArg;
    using ResDataType = KKSG.ChangeGuildSettingRes;

    class RpcC2M_ChangeGuildSettingNew : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = new ResDataType();

        public RpcC2M_ChangeGuildSettingNew()
        {
        }

        public override uint GetRpcType()
        {
            return 55897;
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
            Process_RpcC2M_ChangeGuildSettingNew.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_ChangeGuildSettingNew.OnTimeout(oArg);
        }
    }
}
