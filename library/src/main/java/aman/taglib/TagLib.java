package aman.taglib;

import java.util.HashMap;

public class TagLib {
    static { System.loadLibrary("taglib_jni"); }

    public static class Artwork {
        public byte[] data;
        public String mimeType;
        public String description;
        
        public Artwork(byte[] data, String mimeType, String description) {
            this.data = data;
            this.mimeType = mimeType;
            this.description = description;
        }
    }

    /**
     * Returns all available metadata as a key-value map.
     * Keys are tag names, values are their string representations.
     */
    public native HashMap<String, String> getMetadata(String filePath);
    public native boolean setMetadata(String filePath, HashMap<String, String> metadata);
    
    /**
     * Returns all artworks/cover art found in the audio file.
     * @param filePath Path to the audio file
     * @return Array of Artwork objects, or empty array if no artwork found
     */
    public native Artwork[] getArtwork(String filePath);
    
    /**
     * Sets or replaces the artwork/cover art in the audio file.
     * @param filePath Path to the audio file
     * @param imageData Raw image data (JPEG, PNG, etc.)
     * @param mimeType MIME type of the image (e.g., "image/jpeg", "image/png")
     * @param description Optional description of the image (can be empty string)
     * @return true if artwork was successfully set, false otherwise
     */
    public native boolean setArtwork(String filePath, byte[] imageData, String mimeType, String description);
    
    /**
     * Debug method to get information about artwork extraction.
     * @param filePath Path to the audio file
     * @return Debug information as a string
     */
    public native String getArtworkDebugInfo(String filePath);
}
