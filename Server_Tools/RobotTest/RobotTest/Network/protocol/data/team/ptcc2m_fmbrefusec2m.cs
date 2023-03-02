using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/19 15:52:06

namespace XMainClient
{
    using DataType = KKSG.FMBRes;

	class PtcC2M_FMBRefuseC2M : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_FMBRefuseC2M()
		{

		}

		public override uint GetProtoType()
		{
			return 44407;
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
