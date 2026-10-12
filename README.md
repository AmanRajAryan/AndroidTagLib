# AndroidTagLib

[![Maven Central](https://img.shields.io/maven-central/v/io.github.amanrajaryan/TagLib)](https://central.sonatype.com/artifact/io.github.amanrajaryan/TagLib)

An Android library providing JNI bindings for TagLib — read and write audio metadata across all major formats.

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

> **Note:** Replace `<version>` with the latest release shown on the [Maven Central](https://central.sonatype.com/artifact/io.github.amanrajaryan/TagLib) badge above.

## Quick Start

```java
// Read
HashMap<String, String> metadata = TagLib.getMetadata("/sdcard/Music/song.mp3");
String title = metadata.get("TITLE");
String codec = metadata.get("FORMAT");

// Write (add, update, or delete tags)
HashMap<String, String> meta = new HashMap<>();
meta.put("TITLE", "My Song");        // Adds or updates
meta.put("ARTIST", "Artist Name");
meta.put("GENRE", "");               // Deletes tag (empty string removes it)
TagLib.setMetadata("/sdcard/Music/song.mp3", meta); // All other existing tags are preserved

// Artwork
TagLib.Artwork[] art = TagLib.getArtwork("/sdcard/Music/song.mp3");
TagLib.setArtwork("/sdcard/Music/song.mp3", imageBytes, "image/jpeg", ""); // Pass new byte[0] to remove
```

## Supported Formats
MP3, M4A (AAC/ALAC), FLAC, OGG, OPUS, WAV, AIFF, WMA, APE

## Documentation
See [DOCUMENTATION.md](DOCUMENTATION.md) for the full API reference, all metadata keys, and format-specific behavior.

## License
- TagLib: LGPL/MPL
- AndroidTagLib JNI wrapper: Free
