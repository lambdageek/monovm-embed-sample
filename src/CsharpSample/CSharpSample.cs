using System;

namespace CsharpSample
{
    public class SampleClass
    {
	    public static SampleClass Create () {
		    return new SampleClass ();
	    }

	    void Hello () {
		    Console.WriteLine ("Hello");
	    }
    }
}
