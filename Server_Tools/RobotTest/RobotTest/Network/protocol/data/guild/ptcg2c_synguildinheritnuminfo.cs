using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/26 17:08:12

namespace XMainClient
{
    using DataType = KKSG.SynGuildInheritNumInfo;

	class PtcG2C_SynGuildInheritNumInfo : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SynGuildInheritNumInfo()
		{

		}

		public override uint GetProtoType()
		{
			return 54442;
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
			Process_PtcG2C_SynGuildInheritNumInfo.Process(this);
		}

	}
}
