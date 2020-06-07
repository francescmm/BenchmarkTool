# QBenchmark

In this repository you can find the QBenchmark project.

QBenchmark tracks the time it takes a method call to run. It acts as singleton object but supports multi-threading.

After the instance is destroyed, the results are printed to a file using the specified format. If not format is defined, it will print to a regular text file.

Follow the instructions for usage:

1. Create an instance: QBenchmarkRegisterer::getInstance();
2. If you want, add a different output format by using:  QBenchmarkRegisterer::getInstance()->setOutputFormat(...).
3. If you want to retrieve the current tree view, call you do: std::string output; output << QBenchmarkRegisterer::getInstance();

