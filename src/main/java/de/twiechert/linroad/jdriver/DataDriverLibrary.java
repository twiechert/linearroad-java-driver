package de.twiechert.linroad.jdriver;

import com.sun.jna.Callback;
import com.sun.jna.Library;

/**
 * Created by tafyun on 30.05.16.
 */
public interface DataDriverLibrary extends Library {


     interface TupleReceivedCallback extends Callback {
          void invoke(String tuple);
     }
     int startProgram(String argv,  TupleReceivedCallback tupleReceivedCallback);
     int  test();
}
