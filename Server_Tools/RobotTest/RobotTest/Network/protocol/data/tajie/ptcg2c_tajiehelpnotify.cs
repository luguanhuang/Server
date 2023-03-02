using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/11 19:43:21

namespace XMainClient
{
    using DataType = KKSG.TajieHelpData;

	class PtcG2C_TajieHelpNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TajieHelpNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 36521;
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
			Process_PtcG2C_TajieHelpNotify.Process(this);
		}

	}
}
