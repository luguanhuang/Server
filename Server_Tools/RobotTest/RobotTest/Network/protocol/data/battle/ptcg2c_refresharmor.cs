using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/2/19 14:17:10

namespace XMainClient
{
    using DataType = KKSG.ArmorDesc;

	class PtcG2C_RefreshArmor : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_RefreshArmor()
		{

		}

		public override uint GetProtoType()
		{
			return 25110;
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
			Process_PtcG2C_RefreshArmor.Process(this);
		}

	}
}
