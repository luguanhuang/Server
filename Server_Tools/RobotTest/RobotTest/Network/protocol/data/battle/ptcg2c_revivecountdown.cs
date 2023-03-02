using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/22 20:28:41

namespace XMainClient
{
    using DataType = KKSG.ReviveCountdownInfo;

	class PtcG2C_ReviveCountdown : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ReviveCountdown()
		{

		}

		public override uint GetProtoType()
		{
			return 54507;
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
			Process_PtcG2C_ReviveCountdown.Process(this);
		}

	}
}
