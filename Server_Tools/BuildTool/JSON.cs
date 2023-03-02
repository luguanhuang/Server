using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;  
using System.Runtime.Serialization;  
using System.Runtime.Serialization.Json;  

namespace BuildTool
{
    public static class JSON
    {
        public static T parse<T>(string jsonString)
    	{
    	    using (var ms = new MemoryStream(Encoding.UTF8.GetBytes(jsonString)))
    	    {
    	        return (T)new DataContractJsonSerializer(typeof(T)).ReadObject(ms);
    	    }
    	}
    }
}
