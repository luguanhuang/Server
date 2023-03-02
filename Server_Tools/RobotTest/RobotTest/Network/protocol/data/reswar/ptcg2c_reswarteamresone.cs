using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/1 18:28:51

namespace XMainClient
{
    using DataType = KKSG.ResWarAllTeamBaseInfo;

	class PtcG2C_ResWarTeamResOne : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ResWarTeamResOne()
		{

		}

		public override uint GetProtoType()
		{
			return 8869;
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
			Process_PtcG2C_ResWarTeamResOne.Process(this);
		}

	}
}
