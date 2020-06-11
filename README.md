# BenchmarkTool

In this repository you can find the BenchmarkTool project.

BenchmarkTool tracks the time it takes a method call to run. It acts as singleton object but supports multi-threading.

After the instance is destroyed, the results are printed to a file using the specified format. If not format is defined, it will print to a regular text file.

Follow the instructions for usage:

1. Create an instance: BenchmarkTool::getInstance();
2. If you want, add a different output format by using:  BenchmarkTool::getInstance()->setOutputFormat(...).
3. If you want to retrieve the current tree view, you can do the following method call: std::string output; output << BenchmarkTool::getInstance();

