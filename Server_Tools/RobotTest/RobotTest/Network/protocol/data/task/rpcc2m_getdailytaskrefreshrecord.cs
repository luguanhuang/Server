using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/18 1:41:28

namespace XMainClient
{
	using ArgDataType = KKSG.GetDailyTaskRefreshRecordArg;
    using ResDataType = KKSG.GetDailyTaskRefreshRecordRes;

    class RpcC2M_GetDailyTaskRefreshRecord : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_GetDailyTaskRefreshRecord()
        {
        }

        public override uint GetRpcType()
        {
            return 7202;
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
            Process_RpcC2M_GetDailyTaskRefreshRecord.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_GetDailyTaskRefreshRecord.OnTimeout(oArg);
        }
    }
}
