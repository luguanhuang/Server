using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/6 23:18:28

namespace XMainClient
{
    using DataType = KKSG.LoadAccountData;

	class PtcG2C_NotifyAccountData : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NotifyAccountData()
		{

		}

		public override uint GetProtoType()
		{
			return 29137;
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
			Process_PtcG2C_NotifyAccountData.Process(this);
		}

	}
}
