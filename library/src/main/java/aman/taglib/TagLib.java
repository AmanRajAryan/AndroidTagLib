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

    private static native String[] getMetadataNative(String filePath);

    /**
     * Returns all available metadata as a key-value map.
     * Supports: MP3, M4A, FLAC, WMA, WAV, AIFF, OGG, OPUS, APE.
     * Keys are standard TagLib uppercase tags (e.g., "TITLE", "ARTIST", "ALBUM").
     * Format-specific keys (like "ENCODEDBY") are also available if present.
     */
    public static HashMap<String, String> getMetadata(String filePath) {
        HashMap<String, String> map = new HashMap<>();
        String[] rawData = getMetadataNative(filePath);
        if (rawData != null) {
            for (int i = 0; i < rawData.length - 1; i += 2) {
                map.put(rawData[i], rawData[i + 1]);
            }
        }
        return map;
    }

    private static native boolean setMetadataNative(String filePath, String[] rawData);

    /**
     * Updates metadata for the file.
     * Keys not present in the input map are preserved. Pass an empty string to delete a tag.
     */
    public static boolean setMetadata(String filePath, java.util.Map<String, String> metadata) {
        if (filePath == null || metadata == null) return false;
        String[] flat = new String[metadata.size() * 2];
        int idx = 0;
        for (java.util.Map.Entry<String, String> entry : metadata.entrySet()) {
            flat[idx++] = entry.getKey();
            flat[idx++] = entry.getValue() != null ? entry.getValue() : "";
        }
        return setMetadataNative(filePath, flat);
    }
    
    public static boolean setMetadata(String filePath, HashMap<String, String> metadata) {
        return setMetadata(filePath, (java.util.Map<String, String>) metadata);
    }
    
    /**
     * Returns all artworks/cover art found in the audio file.
     * @param filePath Path to the audio file
     * @return Array of Artwork objects. Empty if none found.
     */
    public static native Artwork[] getArtwork(String filePath);
    
    /**
     * Sets or replaces the Front Cover artwork.
     * Supported formats:
     * - MP3/WAV/AIFF: Replaces 'APIC' ID3v2 frame.
     * - M4A: Replaces 'covr' atom.
     * - FLAC/OGG: Replaces METADATA_BLOCK_PICTURE.
     * - WMA: Replaces 'WM/Picture' attribute.
     * @param filePath Path to the audio file
     * @param imageData Raw image data
     * @param mimeType MIME type (e.g. "image/jpeg", "image/png")
     * @param description Optional description
     * @return true if successful
     */
    public static native boolean setArtwork(String filePath, byte[] imageData, String mimeType, String description);
    
    public static native String getArtworkDebugInfo(String filePath);
}
