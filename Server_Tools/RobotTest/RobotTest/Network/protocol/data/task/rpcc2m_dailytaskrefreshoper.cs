using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/18 1:45:18

namespace XMainClient
{
	using ArgDataType = KKSG.DailyTaskRefreshOperArg;
    using ResDataType = KKSG.DailyTaskRefreshOperRes;

    class RpcC2M_DailyTaskRefreshOper : Rpc
    {
        public ArgDataType oArg = new ArgDataType();
        public ResDataType oRes = null;

        public RpcC2M_DailyTaskRefreshOper()
        {
        }

        public override uint GetRpcType()
        {
            return 31675;
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
            Process_RpcC2M_DailyTaskRefreshOper.OnReply(oArg, oRes);
        }

        public override void OnTimeout(object args)
        {
            Process_RpcC2M_DailyTaskRefreshOper.OnTimeout(oArg);
        }
    }
}
