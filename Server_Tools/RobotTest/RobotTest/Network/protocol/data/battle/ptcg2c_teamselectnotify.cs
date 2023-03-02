using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/26 21:23:41

namespace XMainClient
{
    using DataType = KKSG.TeamSelect;

	class PtcG2C_TeamSelectNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TeamSelectNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 25174;
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
			Process_PtcG2C_TeamSelectNotify.Process(this);
		}

	}
}
