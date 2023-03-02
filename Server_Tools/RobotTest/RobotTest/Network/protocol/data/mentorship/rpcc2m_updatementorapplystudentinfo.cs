using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/9 22:54:06

namespace XMainClient
{
	using ArgDataType = KKSG.UpdateMentorApplyStudentInfoArg;
    using ResDataType = KKSG.UpdateMentorApplyStudentInfoRes;

    class RpcC2M_UpdateMentorApplyStudentInfo : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_UpdateMentorApplyStudentInfo()
        {
        }

        public override uint GetRpcType()
        {
            return 55126;
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
            Process_RpcC2M_UpdateMentorApplyStudentInfo.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_UpdateMentorApplyStudentInfo.OnTimeout(oArg);
        }
    }
}
