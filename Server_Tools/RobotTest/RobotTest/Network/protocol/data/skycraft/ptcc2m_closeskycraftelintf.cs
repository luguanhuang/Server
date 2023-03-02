using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/11 17:59:34

namespace XMainClient
{
    using DataType = KKSG.CloseSkyCraftEliNtf;

	class PtcC2M_CloseSkyCraftEliNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_CloseSkyCraftEliNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 46239;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
