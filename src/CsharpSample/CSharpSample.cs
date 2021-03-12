using System;

namespace CsharpSample
{
    public class SampleClass
    {
	    public static SampleClass Create () {
		    return new SampleClass ();
	    }

	    void Hello () {
		    Console.WriteLine(System.Runtime.InteropServices.RuntimeInformation.FrameworkDescription);
		    Console.WriteLine ("Hello From Managed");
	    }
    }
}
