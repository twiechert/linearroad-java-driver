## Description

This tool allows you to directly process the data returned by the Linear Road Benchmark data driver in Java.
It integrates the provided C-libraries using JNA. This project already contains a native build of these libraries compatible with Linux X64 operating systems.


In order to process the single tuples, you can use the following code snippet:

```java
de.twiechert.linroad.jdriver.DataDriver dataDriver = new de.twiechert.linroad.jdriver.DataDriver()

de.twiechert.linroad.jdriver.DataDriverLibrary.TupleReceivedCallback tupleReceivedCallback = new de.twiechert.linroad.jdriver.DataDriverLibrary.TupleReceivedCallback() {
            public void invoke(String tuple) {
                String[] array = tuple.split(",");
                System.out.println("Received: "+tuple);
                System.out.println("Current speed is: "+array[3]);            }
        };

dataDriver.getLibrary().startProgram("path to raw file", tupleReceivedCallback);
```

Whereas the callback is responsible to further process the data (e.g. sending it to stream processing tool).
You can include this project as a maven dependency, adding the following to you pom.xml:

    <dependency>
        <groupId>com.github.twiechert</groupId>
        <artifactId>linearroad-java-driver</artifactId>
        <version>-SNAPSHOT</version>
     </dependency>

     <repositories>
     <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
     </repository>
     </repositories>



## Compile for different target platforms

You can compile the native libraries for different target platforms. The folder 'linroad-modified' contains a cmake file that allows you to easiliy build.


    cmake -H. -Bbuild

    cmake --build build -- -j3
