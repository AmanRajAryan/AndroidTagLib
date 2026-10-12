# AndroidTagLib Documentation

[![Maven Central](https://img.shields.io/maven-central/v/io.github.amanrajaryan/TagLib)](https://central.sonatype.com/artifact/io.github.amanrajaryan/TagLib)

## Overview
AndroidTagLib is an Android library that provides JNI bindings for the TagLib audio metadata library. It allows Android apps to read and write metadata (tags) in audio files using native TagLib code.

## Features
- Read and write all metadata fields, including custom fields (e.g., `LYRICS`, `CONTENT_RATING`)
- Full artwork support: read and write cover art
- Audio properties: bitrate, sample rate, channels, duration, bit depth, codec
- Full support for MP3, M4A/AAC/ALAC, FLAC, OGG, OPUS, WAV, AIFF, WMA, APE

## Installation

Add the dependency to your app module's `build.gradle`:

```groovy
// Groovy DSL
dependencies {
    implementation 'io.github.amanrajaryan:TagLib:<version>'
}
```

```kotlin
// Kotlin DSL
dependencies {
    implementation("io.github.amanrajaryan:TagLib:<version>")
}
```

*(Replace `<version>` with the latest release shown on the Maven Central badge above)*

## API Reference

### `TagLib.getMetadata(String filePath)`
Returns a `HashMap<String, String>` of all metadata for the given file.

```java
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
String codec      = metadata.get("FORMAT");        // e.g. "MP3", "FLAC", "AAC", "ALAC", "Vorbis", "Opus", "WAV"
String bitDepth   = metadata.get("BITS_PER_SAMPLE"); // e.g. 16, 24 (lossless formats)

// Custom fields
String lyrics        = metadata.get("LYRICS");
String contentRating = metadata.get("CONTENT_RATING");
```

All keys are uppercase. Only fields present in the file are included in the map.

### `TagLib.setMetadata(String filePath, Map<String, String> metadata)`
Writes metadata to the audio file. Returns `true` if saved successfully, `false` otherwise. Accepts any `Map<String, String>` (such as `HashMap`).

#### Behavior:
- **Default (Partial Update)**: Only fields present in the map are modified. All other existing tags on the file remain completely untouched and preserved.
- **Adding a tag**: Provide a key and its value. If the tag does not exist, it is created.
- **Updating a tag**: Provide an existing key with a new value to overwrite it.
- **Deleting a tag**: Set the tag's value to an empty string `""` (or `null`) to erase it from the file.
- **Key naming**: Keys are uppercase TagLib tag names (e.g. `TITLE`, `ARTIST`, `ALBUM`, `TRACKNUMBER`, `GENRE`, `DATE`, `LYRICS`, `CONTENT_RATING`).

```java
Map<String, String> meta = new HashMap<>();

// 1. Adding or updating tags
meta.put("TITLE", "My Song");
meta.put("ARTIST", "Artist Name");
meta.put("ALBUM", "Album Name");
meta.put("LYRICS", "These are the lyrics");
meta.put("CONTENT_RATING", "1"); // 0 = None, 1 = Explicit, 2 = Clean

// 2. Deleting tags (pass empty string or null)
meta.put("COMMENT", "");
meta.put("GENRE", "");

// Writes changes; all other unmentioned tags are preserved
boolean success = TagLib.setMetadata("/sdcard/Music/song.flac", meta);
```

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

### `TagLib.getArtworkDebugInfo(String filePath)`
Returns a debug string listing raw native TagLib properties found in the file. Useful for inspecting unmapped, custom, or vendor-specific tags.

```java
String debugInfo = TagLib.getArtworkDebugInfo("/sdcard/Music/song.mp3");
```

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

## Library Versions
- TagLib 2.3.2

## Compatibility
- Min SDK: 23
- Target SDK: 36
- Compile SDK: 37
- Tested with OpenJDK 17
- Compatible with Android Gradle Plugin 8+
- ABI: `arm64-v8a`, `armeabi-v7a`

## License
- TagLib: LGPL/MPL
- AndroidTagLib JNI wrapper: Free

## Support
For issues or contributions, open an issue or pull request on GitHub.
