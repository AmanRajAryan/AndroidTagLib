# AndroidTagLib

An Android library providing JNI bindings for TagLib — read and write audio metadata across all major formats.

## Installation

```gradle
dependencies {
    implementation("io.github.amanrajaryan:TagLib:1.0.1")
}
```

## Quick Start

```java
// Read
HashMap<String, String> metadata = TagLib.getMetadata("/sdcard/Music/song.mp3");
String title = metadata.get("TITLE");
String codec = metadata.get("FORMAT");

// Write
HashMap<String, String> meta = new HashMap<>();
meta.put("TITLE", "My Song");
meta.put("ARTIST", "Artist Name");
TagLib.setMetadata("/sdcard/Music/song.mp3", meta);

// Artwork
TagLib.Artwork[] art = TagLib.getArtwork("/sdcard/Music/song.mp3");
TagLib.setArtwork("/sdcard/Music/song.mp3", imageBytes, "image/jpeg", "");
```

## Supported Formats
MP3, M4A (AAC/ALAC), FLAC, OGG, OPUS, WAV, AIFF, WMA, APE

## Documentation
See [DOCUMENTATION.md](DOCUMENTATION.md) for the full API reference, all metadata keys, format-specific behavior, and build instructions.

## License
- TagLib: LGPL/MPL
- AndroidTagLib JNI wrapper: Free
