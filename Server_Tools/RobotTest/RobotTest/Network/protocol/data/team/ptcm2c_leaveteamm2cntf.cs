using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/16 11:51:45

namespace XMainClient
{
    using DataType = KKSG.ErrorInfo;

	class PtcM2C_LeaveTeamM2CNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LeaveTeamM2CNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 23306;
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
			Process_PtcM2C_LeaveTeamM2CNtf.Process(this);
		}

	}
}
