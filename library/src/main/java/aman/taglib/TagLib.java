package aman.taglib;

import java.util.HashMap;

public class TagLib {
    static { System.loadLibrary("taglib_jni"); }

    public static class Metadata {
        public String title = "";
        public String artist = "";
        public String album = "";
        public String genre = "";
        public String trackNumber = "";
        public String date = "";
        public String comment = "";
    }

    /**
     * Returns all available metadata as a key-value map.
     * Keys are tag names, values are their string representations.
     */
    public native HashMap<String, String> getMetadata(String filePath);
    public native boolean setMetadata(String filePath, HashMap<String, String> metadata);
}
