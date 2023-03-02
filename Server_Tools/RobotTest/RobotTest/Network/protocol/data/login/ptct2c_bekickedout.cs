using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/13 22:28:56

namespace XMainClient
{
    using DataType = KKSG.KickAccountData;

	class PtcT2C_BeKickedOut : Protocol
	{
		public DataType Data = new DataType();

		public PtcT2C_BeKickedOut()
		{

		}

		public override uint GetProtoType()
		{
			return 38944;
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
			Process_PtcT2C_BeKickedOut.Process(this);
		}

	}
}
