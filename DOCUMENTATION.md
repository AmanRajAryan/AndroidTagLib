# AndroidTagLib Documentation

## Overview
AndroidTagLib is an Android library that provides JNI bindings for the TagLib audio metadata library. It allows Android apps to read and write metadata (tags) in audio files using native TagLib code.

## Features
- Read and write all metadata fields, including custom fields (e.g., `LYRICS`, `CONTENT_RATING`)
- Full artwork support: read and write cover art
- Audio properties: bitrate, sample rate, channels, duration, bit depth, codec
- Full support for MP3, M4A/AAC/ALAC, FLAC, OGG, OPUS, WAV, AIFF, WMA, APE

## Installation

### Via Maven (Recommended)
Add to your app-level `build.gradle`:
```gradle
dependencies {
    implementation("io.github.amanrajaryan:TagLib:1.0.2")
}
```

### Via AAR (Manual)
Copy `library-release.aar` to your app's `libs/` directory and add:
```gradle
dependencies {
    implementation files('libs/library-release.aar')
}
```

## API Reference

### `TagLib.getMetadata(String filePath)`
Returns a `HashMap<String, String>` of all metadata for the given file.

```java
import aman.taglib.TagLib;
import java.util.HashMap;

HashMap<String, String> metadata = TagLib.getMetadata("/sdcard/Music/song.mp3");

// Standard tags
String title  = metadata.get("TITLE");
String artist = metadata.get("ARTIST");
String album  = metadata.get("ALBUM");
String genre  = metadata.get("GENRE");
String track  = metadata.get("TRACKNUMBER");
String date   = metadata.get("DATE");

// Audio properties
String bitrate    = metadata.get("BITRATE");       // kbps
String sampleRate = metadata.get("SAMPLERATE");    // Hz
String channels   = metadata.get("CHANNELS");
String duration   = metadata.get("DURATION_SEC");  // seconds
String codec      = metadata.get("FORMAT");        // e.g. "MP3", "FLAC", "AAC", "ALAC"
String bitDepth   = metadata.get("BITS_PER_SAMPLE"); // e.g. 16, 24 (lossless formats)

// Custom fields
String lyrics        = metadata.get("LYRICS");
String contentRating = metadata.get("CONTENT_RATING");
```

All keys are uppercase. Only fields present in the file are included in the map.

### `TagLib.setMetadata(String filePath, HashMap<String, String> metadata)`
Writes metadata to the given file. Returns `true` if successful.

```java
HashMap<String, String> meta = new HashMap<>();
meta.put("TITLE", "My Song");
meta.put("ARTIST", "Artist Name");
meta.put("ALBUM", "Album Name");
meta.put("LYRICS", "These are the lyrics");
meta.put("CONTENT_RATING", "1");

boolean success = TagLib.setMetadata("/sdcard/Music/song.flac", meta);
```

Only fields present in the map are updated — all other existing tags are preserved. Pass an empty string for a key to delete that tag.

### `TagLib.getArtwork(String filePath)`
Returns an array of `TagLib.Artwork` objects found in the file.

```java
TagLib.Artwork[] artworks = TagLib.getArtwork("/sdcard/Music/song.mp3");
if (artworks != null && artworks.length > 0) {
    byte[] imageData = artworks[0].data;
    String mimeType  = artworks[0].mimeType;       // e.g. "image/jpeg"
    String desc      = artworks[0].description;
}
```

### `TagLib.setArtwork(String filePath, byte[] imageData, String mimeType, String description)`
Sets or replaces the front cover artwork. Returns `true` if successful.

```java
byte[] imageData = ...; // raw image bytes
boolean success = TagLib.setArtwork("/sdcard/Music/song.mp3", imageData, "image/jpeg", "");
```

To remove artwork, pass an empty byte array:
```java
TagLib.setArtwork("/sdcard/Music/song.mp3", new byte[0], "", "");
```

Format-specific behavior:
- MP3 / WAV / AIFF: writes an `APIC` ID3v2 frame
- M4A: writes the `covr` atom
- FLAC / OGG / OPUS: writes `METADATA_BLOCK_PICTURE`
- WMA: writes the `WM/Picture` attribute

## Supported Formats

| Format | Read | Write | Artwork |
|--------|------|-------|---------|
| MP3    | ✓    | ✓     | ✓       |
| M4A (AAC/ALAC) | ✓ | ✓  | ✓       |
| FLAC   | ✓    | ✓     | ✓       |
| OGG    | ✓    | ✓     | ✓       |
| OPUS   | ✓    | ✓     | ✓       |
| WAV    | ✓    | ✓     | ✓       |
| AIFF   | ✓    | ✓     | ✓       |
| WMA    | ✓    | ✓     | ✓       |
| APE    | ✓    | ✓     | ✗       |

## Project Structure
```
AndroidTagLib/
├── build.gradle           # Top-level Gradle build file
├── settings.gradle        # Gradle settings
├── local.properties       # Android SDK location
├── library/               # Android library module
│   ├── build.gradle       # Module build file
│   ├── CMakeLists.txt     # JNI and native build config
│   ├── jni/               # JNI C++ sources
│   ├── src/main/java/     # Java wrapper classes
│   └── taglib/            # TagLib submodule
```

## Building from Source

1. **Install OpenJDK 17:**
    ```bash
    sudo apt-get update && sudo apt-get install -y openjdk-17-jdk
    export JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
    ```

2. **Install Android SDK & NDK:**
    ```bash
    wget https://dl.google.com/android/repository/commandlinetools-linux-10406996_latest.zip -O /tmp/cmdline-tools.zip
    mkdir -p /opt/android-sdk/cmdline-tools
    unzip /tmp/cmdline-tools.zip -d /opt/android-sdk/cmdline-tools
    mv /opt/android-sdk/cmdline-tools/cmdline-tools /opt/android-sdk/cmdline-tools/latest
    export ANDROID_SDK_ROOT=/opt/android-sdk
    /opt/android-sdk/cmdline-tools/latest/bin/sdkmanager --sdk_root=${ANDROID_SDK_ROOT} "platform-tools" "platforms;android-33" "build-tools;33.0.2" "ndk;29.0.14033849"
    ```

3. **Configure `local.properties`:**
    ```
    sdk.dir=/opt/android-sdk
    ndk.dir=/opt/android-sdk/ndk/29.0.14033849
    ```

4. **Initialize submodules:**
    ```bash
    git submodule update --init --recursive
    ```

5. **Build the AAR:**
    ```bash
    ./gradlew :library:assembleRelease
    ```
    Output: `library/build/outputs/aar/library-release.aar`

## Library Versions
- TagLib 2.1.1
- libogg 1.3.5
- libvorbis 1.3.7
- opus 1.3.1
- opusfile 0.12

## Compatibility
- Min SDK: 23
- Tested with OpenJDK 17
- Compatible with Android Gradle Plugin 8+
- ABI: arm64-v8a

## License
- TagLib: LGPL/MPL
- AndroidTagLib JNI wrapper: Free

## Support
For issues or contributions, open an issue or pull request on GitHub.
