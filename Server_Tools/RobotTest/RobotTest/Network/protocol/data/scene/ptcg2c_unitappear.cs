using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/28 22:22:58

namespace XMainClient
{
    using DataType = KKSG.UnitAppearList;

	class PtcG2C_UnitAppear : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_UnitAppear()
		{

		}

		public override uint GetProtoType()
		{
			return 7458;
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
			Process_PtcG2C_UnitAppear.Process(this);
		}

	}
}
