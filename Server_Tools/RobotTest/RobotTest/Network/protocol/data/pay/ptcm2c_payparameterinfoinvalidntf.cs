using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/19 22:30:58

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcM2C_PayParameterInfoInvalidNtf : Protocol
	{
		//public DataType Data = new DataType();

		public PtcM2C_PayParameterInfoInvalidNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 64504;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcM2C_PayParameterInfoInvalidNtf.Process(this);
		}

	}
}
