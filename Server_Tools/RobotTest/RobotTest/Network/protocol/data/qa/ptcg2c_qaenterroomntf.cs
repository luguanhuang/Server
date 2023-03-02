using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/12 10:12:23

namespace XMainClient
{
    using DataType = KKSG.QAEnterRoomNtf;

	class PtcG2C_QAEnterRoomNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_QAEnterRoomNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 28671;
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
			Process_PtcG2C_QAEnterRoomNtf.Process(this);
		}

	}
}
