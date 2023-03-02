using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/11 15:13:37

namespace XMainClient
{
    using DataType = KKSG.LoginRewardRet;

	class PtcG2C_LoginReward2CNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LoginReward2CNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51966;
		}

		public override void Serialize(MemoryStream stream)
		{
			Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcG2C_LoginReward2CNtf.Process(this);
		}

	}
}
