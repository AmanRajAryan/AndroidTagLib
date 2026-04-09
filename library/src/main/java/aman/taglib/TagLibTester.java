package aman.taglib;

import android.content.Context;
import android.util.Log;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.HashMap;

public class TagLibTester {
    private static final String TAG = "TagLibTester";
    private final TagLib tagLib;
    private final Context context;

    public TagLibTester(Context context) {
        this.context = context;
        this.tagLib = new TagLib();
    }

    public void runAllTests() {
        new Thread(() -> {
            Log.d(TAG, "=== STARTING TAGLIB EXTENSIVE CHECKS ===");
            
            String[] formats = {"sample.mp3", "sample.m4a", "sample.flac", "sample.wav", "sample.ogg"};
            
            for (String filename : formats) {
                testFormat(filename);
            }
            
            Log.d(TAG, "=== ALL CHECKS COMPLETED ===");
        }).start();
    }

    private void testFormat(String assetName) {
        Log.d(TAG, "--------------------------------------------------");
        Log.d(TAG, "Testing Format: " + assetName);
        
        File file = copyAssetToCache(assetName);
        if (file == null) {
            Log.e(TAG, "Skipping " + assetName + " (File not found in assets)");
            return;
        }

        String path = file.getAbsolutePath();

        HashMap<String, String> meta = tagLib.getMetadata(path);
        Log.d(TAG, "[READ] Found " + (meta != null ? meta.size() : "null") + " tags.");
        if (meta != null) {
            Log.d(TAG, "   Title: " + meta.get("TITLE"));
            Log.d(TAG, "   Type: " + meta.get("ARTIST"));
        }

        HashMap<String, String> newMeta = new HashMap<>();
        String testTitle = "Test Title " + System.currentTimeMillis();
        newMeta.put("TITLE", testTitle);
        newMeta.put("ARTIST", "TagLib Android");
        
        boolean writeSuccess = tagLib.setMetadata(path, newMeta);
        Log.d(TAG, "[WRITE] Result: " + (writeSuccess ? "SUCCESS" : "FAILED"));

        HashMap<String, String> verifyMeta = tagLib.getMetadata(path);
        String readBackTitle = verifyMeta.get("TITLE");
        if (testTitle.equals(readBackTitle)) {
             Log.i(TAG, "[VERIFY] PASS: Title updated correctly.");
        } else {
             Log.e(TAG, "[VERIFY] FAIL: Expected '" + testTitle + "' but got '" + readBackTitle + "'");
        }

        TagLib.Artwork[] art = tagLib.getArtwork(path);
        Log.d(TAG, "[ARTWORK] Found " + (art != null ? art.length : "0") + " images.");
        
        String debug = tagLib.getArtworkDebugInfo(path);

        file.delete();
    }

    private File copyAssetToCache(String fileName) {
        try {
            File file = new File(context.getCacheDir(), fileName);
            try (InputStream is = context.getAssets().open(fileName);
                 FileOutputStream fos = new FileOutputStream(file)) {
                byte[] buffer = new byte[1024];
                int read;
                while ((read = is.read(buffer)) != -1) {
                    fos.write(buffer, 0, read);
                }
                return file;
            }
        } catch (Exception e) {
            return null;
        }
    }
}
