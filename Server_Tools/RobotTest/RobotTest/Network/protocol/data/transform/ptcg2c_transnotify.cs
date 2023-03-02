using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/18 14:21:36

namespace XMainClient
{
    using DataType = KKSG.TransNotify;

	class PtcG2C_TransNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TransNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 15935;
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
			Process_PtcG2C_TransNotify.Process(this);
		}

	}
}
