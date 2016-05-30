import com.sun.jna.Native;

import java.io.File;

/**
 * Created by tafyun on 30.05.16.
 */
public class DataDriver {

    // provide as arguement
    public static void main(String[] args) {
     DataDriver dataDriver = new DataDriver(args[0]);
        dataDriver.runSample();
    }

     private DataDriverLibrary instance = (DataDriverLibrary) Native.loadLibrary("datadriver", DataDriverLibrary.class);
     private String path;


    public DataDriver(String path) {
        System.setProperty("jna.library.path", "datadriver");
        this.path = path;

    }

    public DataDriver() {
        System.setProperty("jna.library.path", "datadriver");

    }

    public void runSample() {
        DataDriverLibrary.TupleReceivedCallback tupleReceivedCallback = new DataDriverLibrary.TupleReceivedCallback() {
            public void invoke(String tuple) {
                System.out.println("Received!!!");
            }
        };

        instance.startProgram(path,tupleReceivedCallback);
        instance.test();

    }

    public DataDriverLibrary getLibrary() {return instance;}




}
