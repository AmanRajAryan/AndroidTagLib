# AndroidTagLib Documentation

## Overview
AndroidTagLib is an Android library that provides JNI bindings for the TagLib audio metadata library. It allows Android apps to read and write metadata (tags) in audio files using native TagLib code.

## Features
- JNI wrapper for TagLib
- Buildable as an AAR for easy integration
- Uses CMake for native code
- Compatible with OpenJDK 17 and Android Gradle Plugin
- TagLib included as a git submodule
- **Read and write all metadata fields, including custom fields (e.g., LYRICS, contentRating)**
- **Full support for .opus files and all major formats**
- **Get audio properties: bitrate, sample rate, channels, length, codec, etc. via getMetadata(String filePath)**

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

## Building the AAR
### Prerequisites and Build Steps (Automated)
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
    /opt/android-sdk/cmdline-tools/latest/bin/sdkmanager --sdk_root=${ANDROID_SDK_ROOT} "platform-tools" "platforms;android-33" "build-tools;33.0.2" "ndk;25.2.9519653"
    ```
3. **Configure local.properties:**
    Set these paths in `/workspaces/AndroidTagLib/local.properties`:
    ```
    sdk.dir=/opt/android-sdk
    ndk.dir=/opt/android-sdk/ndk/25.2.9519653
    ```
4. **Initialize submodules:**
    ```bash
    git submodule update --init --recursive
    ```
5. **Build the AAR:**
    ```bash
    ./gradlew :library:assembleRelease
    ```
6. The AAR will be generated at:
    `library/build/outputs/aar/library-release.aar`

**Notes:**
- HTTP/SSL and documentation dependencies are disabled for cross-platform builds.
- All native dependencies are managed via git submodules.

## Using the AAR in Your App
- Add the AAR to your app's `libs/` directory.
- Use the Java wrapper classes to access TagLib features.

## How to Use the AAR and Its Methods

### 1. Import the AAR
Copy `library-release.aar` to your app's `libs/` directory. In your app-level `build.gradle`, add:
```gradle
dependencies {
    implementation files('libs/library-release.aar')
}
```

### 2. Load the JNI Library
The JNI library is loaded automatically by the static block in `TagLib`. Make sure the native `.so` files are included in your APK (handled by Gradle if built correctly).

### 3. Access Metadata
Use the `TagLib` class to read and write audio metadata.

#### Example: Read Metadata (All Properties)
```java
import aman.taglib.TagLib;
import java.util.HashMap;

String filePath = "/sdcard/Music/song.mp3";
HashMap<String, String> metadata = new TagLib().getMetadata(filePath);
System.out.println("Title: " + metadata.get("TITLE"));
System.out.println("Bitrate: " + metadata.get("BITRATE"));
System.out.println("Sample Rate: " + metadata.get("SAMPLERATE"));
System.out.println("Channels: " + metadata.get("CHANNELS"));
System.out.println("Length: " + metadata.get("LENGTH"));
System.out.println("Codec: " + metadata.get("CODEC"));
System.out.println("LYRICS: " + metadata.get("LYRICS"));
// ...print other fields as needed
```

#### Example: Write Metadata (Standard and Custom Fields)
```java
import aman.taglib.TagLib;
import java.util.HashMap;

String filePath = "/sdcard/Music/song.opus"; // .opus supported
HashMap<String, String> meta = new HashMap<>();
meta.put("TITLE", "My Opus Song");
meta.put("ARTIST", "Artist Name");
meta.put("LYRICS", "These are the lyrics"); // Custom field
meta.put("contentRating", "PG-13"); // Another custom field
boolean success = new TagLib().setMetadata(filePath, meta);
if (success) {
    System.out.println("Metadata updated successfully.");
} else {
    System.out.println("Failed to update metadata.");
}
```
- Only fields present in the map are changed/added; all other metadata remains untouched.
- Custom fields (like `LYRICS`) are supported for .opus, .flac, .ogg, etc.

### 4. Get All Metadata (Key-Value Map)
You can retrieve all available metadata (including custom tags, extended properties, and all fields TagLib supports) using the `getMetadata` method:

```java
import aman.taglib.TagLib;
import java.util.HashMap;

String filePath = "/sdcard/Music/song.mp3";
HashMap<String, String> allMeta = new TagLib().getMetadata(filePath);
for (String key : allMeta.keySet()) {
    System.out.println(key + ": " + allMeta.get(key));
}
```

This will print all standard and extended metadata for MP3, FLAC, OGG, OPUS, and other supported formats. The keys are tag names or frame IDs, and the values are their string representations.

#### Supported Formats
- MP3 (ID3v2 frames)
- FLAC (Vorbis comments)
- OGG (Vorbis comments)
- OPUS (Opus tags, including custom fields)
- More formats can be added as needed (MPC, APE, ASF, etc.)

#### Notes
- For MP3 files, all ID3v2 frames are included.
- For FLAC, OGG, OPUS, all Vorbis comments and properties are included.
- The returned map may contain both standard and custom fields.
- You can use this method to inspect all metadata TagLib can extract from a file.

### 5. Method Explanations
- `getMetadata(String filePath)`: Reads metadata from the specified audio file. Returns a `Metadata` object with fields populated.
- `setMetadata(String filePath, Metadata metadata)`: Writes the provided metadata to the audio file. Returns `true` if successful, `false` otherwise.

### 6. Metadata Fields
The `Metadata` class contains:
- `title`: Song title
- `artist`: Artist name
- `album`: Album name
- `genre`: Genre
- `trackNumber`: Track number
- `date`: Year or date
- `comment`: Additional comments

### 7. Notes
- Supported formats depend on TagLib (MP3, FLAC, OGG, etc.).
- File paths must be accessible and writable for metadata updates.
- JNI errors will throw exceptions if the native library is missing or incompatible.

## TagLib Submodule
- TagLib is included as a submodule in `library/taglib`.
- To initialize:
  ```bash
  git submodule update --init --recursive
  ```

## Compatibility
- Tested with OpenJDK 17
- Compatible with Android Gradle Plugin 8+

## License
- TagLib: LGPL/MPL
- AndroidTagLib JNI wrapper: Specify your license here

## Support
For issues or contributions, open an issue or pull request on GitHub.
