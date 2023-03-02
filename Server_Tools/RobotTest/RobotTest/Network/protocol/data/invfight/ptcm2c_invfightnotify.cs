using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/13 23:25:40

namespace XMainClient
{
    using DataType = KKSG.InvFightPara;

	class PtcM2C_InvFightNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_InvFightNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 38172;
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
			Process_PtcM2C_InvFightNotify.Process(this);
		}

	}
}
