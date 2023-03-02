using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/2 16:33:43

namespace XMainClient
{
    using DataType = KKSG.GmfKickRes;

	class PtcG2C_GmfKickNty : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfKickNty()
		{

		}

		public override uint GetProtoType()
		{
			return 21295;
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
			Process_PtcG2C_GmfKickNty.Process(this);
		}

	}
}
