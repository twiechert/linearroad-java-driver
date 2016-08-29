package de.twiechert.linroad.jdriver;

import com.sun.jna.Native;

/**
 * Created by tafyun on 30.05.16.
 */
public class DataDriver {

    public enum Architecture {
        X64_LINUX("datadriver"), X64_POWER_7("datadriverpower7"), X64_MACOS("macos");

        String file;

        Architecture(String file) {
            this.file = file;
        }

        public String getFile() {
            return file;
        }
    }

    // provide as arguement
    public static void main(String[] args) {
     DataDriver dataDriver = new DataDriver(args[0]);
        dataDriver.runSample();
    }

     private DataDriverLibrary instance;
     private String filePath;


    public DataDriver(String filePath, String searchPath) {
        System.setProperty("jna.library.path", searchPath);
        this.filePath = filePath;
        this.instance =  (DataDriverLibrary) Native.loadLibrary(Architecture.X64_LINUX.getFile(), DataDriverLibrary.class);

    }

    public DataDriver(String filePath, Architecture architecture) {
        this.filePath = filePath;
        this.instance =  (DataDriverLibrary) Native.loadLibrary( architecture.getFile(), DataDriverLibrary.class);
    }


    public DataDriver(String filePath, String searchPath, Architecture architecture) {
        System.setProperty("jna.library.path", searchPath);
        this.filePath = filePath;
        this.instance =  (DataDriverLibrary) Native.loadLibrary( architecture.getFile(), DataDriverLibrary.class);
    }

    public DataDriver(String searchPath) {

        System.setProperty("jna.library.path", searchPath);
        this.instance =  (DataDriverLibrary) Native.loadLibrary(Architecture.X64_LINUX.getFile(), DataDriverLibrary.class);

    }

    public DataDriver() {
        this.instance =  (DataDriverLibrary) Native.loadLibrary(Architecture.X64_LINUX.getFile(), DataDriverLibrary.class);
    }

    public void runSample() {
        DataDriverLibrary.TupleReceivedCallback tupleReceivedCallback = new DataDriverLibrary.TupleReceivedCallback() {
            public void invoke(String tuple) {
                String[] array = tuple.split(",");
                System.out.println("Received: "+tuple);
                System.out.println("Current speed is: "+array[3]);

            }
        };

        instance.startProgram(filePath,tupleReceivedCallback);
    }

    public DataDriverLibrary getLibrary() {return instance;}




}
