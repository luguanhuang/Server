using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/4 16:33:57

namespace XMainClient
{
	using ArgDataType = KKSG.PersonalCareerArg;
    using ResDataType = KKSG.PersonalCareerRes;

    class RpcC2G_PersonalCareer : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2G_PersonalCareer()
        {
        }

        public override uint GetRpcType()
        {
            return 64048;
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
            Process_RpcC2G_PersonalCareer.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2G_PersonalCareer.OnTimeout(oArg);
        }
    }
}
