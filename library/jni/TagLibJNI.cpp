#include "flacfile.h"
#include "vorbisfile.h"
#include "opusfile.h"
#include "tpropertymap.h"
#include "mpeg/id3v2/id3v2.h"
#include "mpeg/id3v2/frames/attachedpictureframe.h"
#include "flacpicture.h"
#include "oggfile.h"
#include "oggpage.h"
#include "oggpageheader.h"

// Skeletal JNI wrapper for TagLib
#include <jni.h>
#include <string>
#include <fstream>
#include <vector>
#include "fileref.h"
#include "tag.h"
#include "mpegfile.h"
#include "id3v2tag.h"
#include "id3v2frame.h"

extern "C" {

// Helper function to extract artwork from Ogg/Opus attached picture streams
std::vector<std::pair<TagLib::ByteVector, std::string>> extractOggAttachedPictures(const char* filePath) {
    std::vector<std::pair<TagLib::ByteVector, std::string>> pictures;
    
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return pictures;
    
    // Read entire file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();
    
    // More aggressive search - look for ANY JPEG start (FF D8) followed by end (FF D9)
    for (size_t i = 0; i < buffer.size() - 2; i++) {
        // Find JPEG start marker (FF D8)
        if ((unsigned char)buffer[i] == 0xFF && 
            (unsigned char)buffer[i+1] == 0xD8) {
            
            // Look ahead to verify this looks like a real JPEG
            // Check if next byte is FF (marker)
            if (i + 2 < buffer.size() && (unsigned char)buffer[i+2] == 0xFF) {
                // Find JPEG end marker (FF D9)
                size_t endPos = i + 2;
                bool foundEnd = false;
                
                while (endPos < buffer.size() - 1 && endPos < i + 10 * 1024 * 1024) {
                    if ((unsigned char)buffer[endPos] == 0xFF && 
                        (unsigned char)buffer[endPos+1] == 0xD9) {
                        endPos += 2;
                        foundEnd = true;
                        break;
                    }
                    endPos++;
                }
                
                if (foundEnd) {
                    size_t imageSize = endPos - i;
                    // Accept images 5KB to 10MB
                    if (imageSize > 5000 && imageSize < 10 * 1024 * 1024) {
                        TagLib::ByteVector imageData(buffer.data() + i, imageSize);
                        pictures.push_back(std::make_pair(imageData, "image/jpeg"));
                        i = endPos;
                        continue;
                    }
                }
            }
        }
        
        // Check for PNG signature (89 50 4E 47 0D 0A 1A 0A)
        if (i < buffer.size() - 8 &&
            (unsigned char)buffer[i] == 0x89 && 
            (unsigned char)buffer[i+1] == 0x50 && 
            (unsigned char)buffer[i+2] == 0x4E && 
            (unsigned char)buffer[i+3] == 0x47 &&
            (unsigned char)buffer[i+4] == 0x0D &&
            (unsigned char)buffer[i+5] == 0x0A &&
            (unsigned char)buffer[i+6] == 0x1A &&
            (unsigned char)buffer[i+7] == 0x0A) {
            
            // Find PNG IEND chunk
            size_t endPos = i + 8;
            bool foundEnd = false;
            
            while (endPos < buffer.size() - 12 && endPos < i + 10 * 1024 * 1024) {
                if (buffer[endPos] == 'I' && 
                    buffer[endPos+1] == 'E' && 
                    buffer[endPos+2] == 'N' && 
                    buffer[endPos+3] == 'D') {
                    endPos += 8; // Include CRC
                    foundEnd = true;
                    break;
                }
                endPos++;
            }
            
            if (foundEnd) {
                size_t imageSize = endPos - i;
                if (imageSize > 5000 && imageSize < 10 * 1024 * 1024) {
                    TagLib::ByteVector imageData(buffer.data() + i, imageSize);
                    pictures.push_back(std::make_pair(imageData, "image/png"));
                    i = endPos;
                    continue;
                }
            }
        }
    }
    
    return pictures;
}

JNIEXPORT jobject JNICALL Java_aman_taglib_TagLib_getMetadata(JNIEnv* env, jobject thiz, jstring filePath) {
    const char* pathChars = env->GetStringUTFChars(filePath, nullptr);
    std::string path(pathChars);
    env->ReleaseStringUTFChars(filePath, pathChars);

    TagLib::FileRef f(path.c_str());
    jclass hashMapClass = env->FindClass("java/util/HashMap");
    jobject mapObj = env->NewObject(hashMapClass, env->GetMethodID(hashMapClass, "<init>", "()V"));
    jmethodID putMethod = env->GetMethodID(hashMapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    if (!f.isNull() && f.tag()) {
        TagLib::Tag* tag = f.tag();
        // Standard fields
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("title"), env->NewStringUTF(tag->title().to8Bit(true).c_str()));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("artist"), env->NewStringUTF(tag->artist().to8Bit(true).c_str()));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("album"), env->NewStringUTF(tag->album().to8Bit(true).c_str()));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("genre"), env->NewStringUTF(tag->genre().to8Bit(true).c_str()));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("trackNumber"), env->NewStringUTF(std::to_string(tag->track()).c_str()));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("date"), env->NewStringUTF(std::to_string(tag->year()).c_str()));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("comment"), env->NewStringUTF(tag->comment().to8Bit(true).c_str()));

        // MP3 (ID3v2)
        TagLib::MPEG::File* mp3File = dynamic_cast<TagLib::MPEG::File*>(f.file());
        if (mp3File && mp3File->ID3v2Tag()) {
            TagLib::ID3v2::Tag* id3v2 = mp3File->ID3v2Tag();
            TagLib::ID3v2::FrameList frames = id3v2->frameList();
            for (auto it = frames.begin(); it != frames.end(); ++it) {
                TagLib::ByteVector frameIdBV = (*it)->frameID();
                std::string frameId(frameIdBV.data(), frameIdBV.size());
                std::string frameText = (*it)->toString().to8Bit(true);
                env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF(frameId.c_str()), env->NewStringUTF(frameText.c_str()));
            }
        }
        // FLAC, OGG, OPUS, etc. (property map)
        TagLib::FLAC::File* flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file());
        if (flacFile) {
            TagLib::PropertyMap props = flacFile->properties();
            for (auto it = props.begin(); it != props.end(); ++it) {
                std::string key = it->first.to8Bit(true);
                std::string value = it->second.toString().to8Bit(true);
                env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF(key.c_str()), env->NewStringUTF(value.c_str()));
            }
        }
        TagLib::Ogg::Vorbis::File* oggFile = dynamic_cast<TagLib::Ogg::Vorbis::File*>(f.file());
        if (oggFile) {
            TagLib::PropertyMap props = oggFile->properties();
            for (auto it = props.begin(); it != props.end(); ++it) {
                std::string key = it->first.to8Bit(true);
                std::string value = it->second.toString().to8Bit(true);
                env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF(key.c_str()), env->NewStringUTF(value.c_str()));
            }
        }
        TagLib::Ogg::Opus::File* opusFile = dynamic_cast<TagLib::Ogg::Opus::File*>(f.file());
        if (opusFile) {
            TagLib::PropertyMap props = opusFile->properties();
            for (auto it = props.begin(); it != props.end(); ++it) {
                std::string key = it->first.to8Bit(true);
                std::string value = it->second.toString().to8Bit(true);
                env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF(key.c_str()), env->NewStringUTF(value.c_str()));
            }
        }
        // Add more formats as needed (MPC, APE, ASF, etc.)
    }
    return mapObj;
}

JNIEXPORT jboolean JNICALL
Java_aman_taglib_TagLib_setMetadata(JNIEnv *env, jobject /*thiz*/, jstring jFilePath, jobject jMetadataMap) {
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    if (!filePath) return JNI_FALSE;

    // Convert Java HashMap<String, String> to std::map
    std::map<std::string, std::string> metadata;
    jclass mapClass = env->GetObjectClass(jMetadataMap);
    jmethodID entrySet = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    jobject setObj = env->CallObjectMethod(jMetadataMap, entrySet);
    jclass setClass = env->GetObjectClass(setObj);
    jmethodID iterator = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterObj = env->CallObjectMethod(setObj, iterator);
    jclass iterClass = env->GetObjectClass(iterObj);
    jmethodID hasNext = env->GetMethodID(iterClass, "hasNext", "()Z");
    jmethodID next = env->GetMethodID(iterClass, "next", "()Ljava/lang/Object;");
    jclass entryClass = nullptr;
    jmethodID getKey = nullptr, getValue = nullptr;

    while (env->CallBooleanMethod(iterObj, hasNext)) {
        jobject entryObj = env->CallObjectMethod(iterObj, next);
        if (!entryClass) {
            entryClass = env->GetObjectClass(entryObj);
            getKey = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
            getValue = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
        }
        jstring keyStr = (jstring)env->CallObjectMethod(entryObj, getKey);
        jstring valStr = (jstring)env->CallObjectMethod(entryObj, getValue);
        const char *key = env->GetStringUTFChars(keyStr, nullptr);
        const char *val = env->GetStringUTFChars(valStr, nullptr);
        metadata[key] = val;
        env->ReleaseStringUTFChars(keyStr, key);
        env->ReleaseStringUTFChars(valStr, val);
        env->DeleteLocalRef(entryObj);
    }

    TagLib::FileRef f(filePath);
    if (f.isNull() || !f.file()) {
        env->ReleaseStringUTFChars(jFilePath, filePath);
        return JNI_FALSE;
    }

    TagLib::Tag *tag = f.tag();
    if (!tag) {
        env->ReleaseStringUTFChars(jFilePath, filePath);
        return JNI_FALSE;
    }

    // Only update fields provided by user, leave others untouched
    if (metadata.count("TITLE")) tag->setTitle(TagLib::String(metadata["TITLE"], TagLib::String::UTF8));
    if (metadata.count("ARTIST")) tag->setArtist(TagLib::String(metadata["ARTIST"], TagLib::String::UTF8));
    if (metadata.count("ALBUM")) tag->setAlbum(TagLib::String(metadata["ALBUM"], TagLib::String::UTF8));
    if (metadata.count("GENRE")) tag->setGenre(TagLib::String(metadata["GENRE"], TagLib::String::UTF8));
    if (metadata.count("COMMENT")) tag->setComment(TagLib::String(metadata["COMMENT"], TagLib::String::UTF8));
    if (metadata.count("YEAR")) tag->setYear(atoi(metadata["YEAR"].c_str()));
    if (metadata.count("TRACK")) tag->setTrack(atoi(metadata["TRACK"].c_str()));

    // For custom fields, only replace/add those provided by user
    TagLib::PropertyMap props = f.file()->properties();
    for (const auto &kv : metadata) {
        if (kv.first == "TITLE" || kv.first == "ARTIST" || kv.first == "ALBUM" ||
            kv.first == "GENRE" || kv.first == "COMMENT" || kv.first == "YEAR" || kv.first == "TRACK")
            continue;
        props.replace(kv.first, TagLib::StringList(TagLib::String(kv.second, TagLib::String::UTF8)));
    }
    f.file()->setProperties(props);

    bool ok = f.file()->save();
    env->ReleaseStringUTFChars(jFilePath, filePath);
    return ok ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobjectArray JNICALL
Java_aman_taglib_TagLib_getArtwork(JNIEnv *env, jobject /*thiz*/, jstring jFilePath) {
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    if (!filePath) return nullptr;

    TagLib::FileRef f(filePath);
    env->ReleaseStringUTFChars(jFilePath, filePath);

    if (f.isNull() || !f.file()) return nullptr;

    std::vector<jobject> artworks;
    jclass artworkClass = env->FindClass("aman/taglib/TagLib$Artwork");
    jmethodID constructor = env->GetMethodID(artworkClass, "<init>", 
        "([BLjava/lang/String;Ljava/lang/String;)V");

    // Try MP3 ID3v2 APIC frames (can have multiple)
    TagLib::MPEG::File *mp3File = dynamic_cast<TagLib::MPEG::File*>(f.file());
    if (mp3File && mp3File->ID3v2Tag()) {
        TagLib::ID3v2::FrameList frames = mp3File->ID3v2Tag()->frameListMap()["APIC"];
        for (auto it = frames.begin(); it != frames.end(); ++it) {
            TagLib::ID3v2::AttachedPictureFrame *pic = 
                static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);
            
            TagLib::ByteVector picData = pic->picture();
            jbyteArray imageData = env->NewByteArray(picData.size());
            env->SetByteArrayRegion(imageData, 0, picData.size(), 
                reinterpret_cast<const jbyte*>(picData.data()));
            
            jstring mimeType = env->NewStringUTF(pic->mimeType().toCString(true));
            jstring description = env->NewStringUTF(pic->description().toCString(true));
            
            jobject artwork = env->NewObject(artworkClass, constructor, imageData, mimeType, description);
            artworks.push_back(artwork);
        }
    }

    // Try FLAC pictures (can have multiple)
    TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file());
    if (flacFile) {
        TagLib::List<TagLib::FLAC::Picture*> pictures = flacFile->pictureList();
        for (auto it = pictures.begin(); it != pictures.end(); ++it) {
            TagLib::FLAC::Picture *pic = *it;
            
            TagLib::ByteVector picData = pic->data();
            jbyteArray imageData = env->NewByteArray(picData.size());
            env->SetByteArrayRegion(imageData, 0, picData.size(), 
                reinterpret_cast<const jbyte*>(picData.data()));
            
            jstring mimeType = env->NewStringUTF(pic->mimeType().toCString(true));
            jstring description = env->NewStringUTF(pic->description().toCString(true));
            
            jobject artwork = env->NewObject(artworkClass, constructor, imageData, mimeType, description);
            artworks.push_back(artwork);
        }
    }

    // For Ogg Vorbis, check for all METADATA_BLOCK_PICTURE entries
    TagLib::Ogg::Vorbis::File *oggFile = dynamic_cast<TagLib::Ogg::Vorbis::File*>(f.file());
    if (oggFile) {
        TagLib::Ogg::XiphComment *tag = oggFile->tag();
        if (tag && tag->fieldListMap().contains("METADATA_BLOCK_PICTURE")) {
            TagLib::StringList pictureDataList = tag->fieldListMap()["METADATA_BLOCK_PICTURE"];
            for (auto it = pictureDataList.begin(); it != pictureDataList.end(); ++it) {
                // Decode base64 first
                TagLib::ByteVector encodedData = it->data(TagLib::String::Latin1);
                TagLib::ByteVector decodedData = TagLib::ByteVector::fromBase64(encodedData);
                
                TagLib::FLAC::Picture pic;
                pic.parse(decodedData);
                
                TagLib::ByteVector picData = pic.data();
                if (picData.size() > 0) {
                    jbyteArray imageData = env->NewByteArray(picData.size());
                    env->SetByteArrayRegion(imageData, 0, picData.size(), 
                        reinterpret_cast<const jbyte*>(picData.data()));
                    
                    jstring mimeType = env->NewStringUTF(pic.mimeType().toCString(true));
                    jstring description = env->NewStringUTF(pic.description().toCString(true));
                    
                    jobject artwork = env->NewObject(artworkClass, constructor, imageData, mimeType, description);
                    artworks.push_back(artwork);
                }
            }
        }
    }

    // For Opus, check attached picture streams FIRST (higher priority), then tags
    TagLib::Ogg::Opus::File *opusFile = dynamic_cast<TagLib::Ogg::Opus::File*>(f.file());
    if (opusFile) {
        // First, try extracting from attached picture streams (yt-dlp format - PRIORITY)
        std::vector<std::pair<TagLib::ByteVector, std::string>> pictures = 
            extractOggAttachedPictures(filePath);
        
        for (const auto& pic : pictures) {
            jbyteArray imageData = env->NewByteArray(pic.first.size());
            env->SetByteArrayRegion(imageData, 0, pic.first.size(), 
                reinterpret_cast<const jbyte*>(pic.first.data()));
            
            jstring mimeType = env->NewStringUTF(pic.second.c_str());
            jstring description = env->NewStringUTF("Cover (front)");
            
            jobject artwork = env->NewObject(artworkClass, constructor, imageData, mimeType, description);
            artworks.push_back(artwork);
        }
        
        // If no attached pictures found, check Vorbis comment tags
        if (artworks.empty()) {
            TagLib::Ogg::XiphComment *tag = opusFile->tag();
            if (tag) {
                // Check for METADATA_BLOCK_PICTURE in comments
                if (tag->fieldListMap().contains("METADATA_BLOCK_PICTURE")) {
                    TagLib::StringList pictureDataList = tag->fieldListMap()["METADATA_BLOCK_PICTURE"];
                    for (auto it = pictureDataList.begin(); it != pictureDataList.end(); ++it) {
                        TagLib::ByteVector encodedData = it->data(TagLib::String::Latin1);
                        TagLib::ByteVector decodedData = TagLib::ByteVector::fromBase64(encodedData);
                        
                        TagLib::FLAC::Picture pic;
                        pic.parse(decodedData);
                        
                        TagLib::ByteVector picData = pic.data();
                        if (picData.size() > 0) {
                            jbyteArray imageData = env->NewByteArray(picData.size());
                            env->SetByteArrayRegion(imageData, 0, picData.size(), 
                                reinterpret_cast<const jbyte*>(picData.data()));
                            
                            jstring mimeType = env->NewStringUTF(pic.mimeType().toCString(true));
                            jstring description = env->NewStringUTF(pic.description().toCString(true));
                            
                            jobject artwork = env->NewObject(artworkClass, constructor, imageData, mimeType, description);
                            artworks.push_back(artwork);
                        }
                    }
                }
                
                // Also check for COVERART (alternative field name)
                if (tag->fieldListMap().contains("COVERART")) {
                    TagLib::StringList coverArtList = tag->fieldListMap()["COVERART"];
                    for (auto it = coverArtList.begin(); it != coverArtList.end(); ++it) {
                        TagLib::ByteVector encodedData = it->data(TagLib::String::Latin1);
                        TagLib::ByteVector decodedData = TagLib::ByteVector::fromBase64(encodedData);
                        
                        if (decodedData.size() > 0) {
                            jbyteArray imageData = env->NewByteArray(decodedData.size());
                            env->SetByteArrayRegion(imageData, 0, decodedData.size(), 
                                reinterpret_cast<const jbyte*>(decodedData.data()));
                            
                            std::string mimeTypeStr = "image/jpeg";
                            if (tag->fieldListMap().contains("COVERARTMIME")) {
                                mimeTypeStr = tag->fieldListMap()["COVERARTMIME"].front().to8Bit(true);
                            }
                            
                            jstring mimeType = env->NewStringUTF(mimeTypeStr.c_str());
                            jstring description = env->NewStringUTF("");
                            
                            jobject artwork = env->NewObject(artworkClass, constructor, imageData, mimeType, description);
                            artworks.push_back(artwork);
                        }
                    }
                }
            }
        }
    }

    // Convert vector to Java array
    jobjectArray result = env->NewObjectArray(artworks.size(), artworkClass, nullptr);
    for (size_t i = 0; i < artworks.size(); ++i) {
        env->SetObjectArrayElement(result, i, artworks[i]);
    }

    return result;
}

JNIEXPORT jboolean JNICALL
Java_aman_taglib_TagLib_setArtwork(JNIEnv *env, jobject /*thiz*/, jstring jFilePath, 
    jbyteArray jImageData, jstring jMimeType, jstring jDescription) {
    
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    if (!filePath) return JNI_FALSE;

    TagLib::FileRef f(filePath);
    env->ReleaseStringUTFChars(jFilePath, filePath);

    if (f.isNull() || !f.file()) return JNI_FALSE;

    // Get image data
    jsize imageSize = env->GetArrayLength(jImageData);
    jbyte *imageBytes = env->GetByteArrayElements(jImageData, nullptr);
    TagLib::ByteVector imageData(reinterpret_cast<const char*>(imageBytes), imageSize);
    env->ReleaseByteArrayElements(jImageData, imageBytes, JNI_ABORT);

    // Get mime type and description
    const char *mimeTypeStr = env->GetStringUTFChars(jMimeType, nullptr);
    const char *descStr = env->GetStringUTFChars(jDescription, nullptr);
    TagLib::String mimeType(mimeTypeStr, TagLib::String::UTF8);
    TagLib::String description(descStr, TagLib::String::UTF8);
    env->ReleaseStringUTFChars(jMimeType, mimeTypeStr);
    env->ReleaseStringUTFChars(jDescription, descStr);

    bool success = false;

    // Handle MP3 ID3v2
    TagLib::MPEG::File *mp3File = dynamic_cast<TagLib::MPEG::File*>(f.file());
    if (mp3File) {
        TagLib::ID3v2::Tag *tag = mp3File->ID3v2Tag(true);
        if (tag) {
            // Remove existing APIC frames
            tag->removeFrames("APIC");
            
            // Add new picture frame
            TagLib::ID3v2::AttachedPictureFrame *frame = 
                new TagLib::ID3v2::AttachedPictureFrame();
            frame->setMimeType(mimeType);
            frame->setPicture(imageData);
            frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
            frame->setDescription(description);
            
            tag->addFrame(frame);
            success = mp3File->save();
        }
    }

    // Handle FLAC
    TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file());
    if (flacFile) {
        // Remove existing pictures
        flacFile->removePictures();
        
        // Add new picture
        TagLib::FLAC::Picture *pic = new TagLib::FLAC::Picture();
        pic->setData(imageData);
        pic->setMimeType(mimeType);
        pic->setDescription(description);
        pic->setType(TagLib::FLAC::Picture::FrontCover);
        
        flacFile->addPicture(pic);
        success = flacFile->save();
    }

    // Handle Ogg Vorbis
    TagLib::Ogg::Vorbis::File *oggFile = dynamic_cast<TagLib::Ogg::Vorbis::File*>(f.file());
    if (oggFile) {
        TagLib::Ogg::XiphComment *tag = oggFile->tag();
        if (tag) {
            // Remove existing picture
            tag->removeFields("METADATA_BLOCK_PICTURE");
            
            // Create FLAC picture and encode it
            TagLib::FLAC::Picture pic;
            pic.setData(imageData);
            pic.setMimeType(mimeType);
            pic.setDescription(description);
            pic.setType(TagLib::FLAC::Picture::FrontCover);
            
            TagLib::ByteVector encodedData = pic.render();
            tag->addField("METADATA_BLOCK_PICTURE", 
                TagLib::String(encodedData.toBase64()), true);
            
            success = oggFile->save();
        }
    }

    // Handle Opus
    TagLib::Ogg::Opus::File *opusFile = dynamic_cast<TagLib::Ogg::Opus::File*>(f.file());
    if (opusFile) {
        TagLib::Ogg::XiphComment *tag = opusFile->tag();
        if (tag) {
            // Remove existing picture
            tag->removeFields("METADATA_BLOCK_PICTURE");
            
            // Create FLAC picture and encode it
            TagLib::FLAC::Picture pic;
            pic.setData(imageData);
            pic.setMimeType(mimeType);
            pic.setDescription(description);
            pic.setType(TagLib::FLAC::Picture::FrontCover);
            
            TagLib::ByteVector encodedData = pic.render();
            tag->addField("METADATA_BLOCK_PICTURE", 
                TagLib::String(encodedData.toBase64()), true);
            
            success = opusFile->save();
        }
    }

    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_aman_taglib_TagLib_getArtworkDebugInfo(JNIEnv *env, jobject /*thiz*/, jstring jFilePath) {
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    if (!filePath) return env->NewStringUTF("Error: filePath is null");

    std::string debugInfo;
    TagLib::FileRef f(filePath);
    env->ReleaseStringUTFChars(jFilePath, filePath);

    if (f.isNull() || !f.file()) {
        return env->NewStringUTF("Error: File is null or cannot be opened");
    }

    // Check file type
    TagLib::MPEG::File *mp3File = dynamic_cast<TagLib::MPEG::File*>(f.file());
    TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file());
    TagLib::Ogg::Vorbis::File *oggFile = dynamic_cast<TagLib::Ogg::Vorbis::File*>(f.file());
    TagLib::Ogg::Opus::File *opusFile = dynamic_cast<TagLib::Ogg::Opus::File*>(f.file());

    if (mp3File) debugInfo += "File type: MP3\n";
    else if (flacFile) debugInfo += "File type: FLAC\n";
    else if (oggFile) debugInfo += "File type: Ogg Vorbis\n";
    else if (opusFile) debugInfo += "File type: Opus\n";
    else debugInfo += "File type: Unknown\n";

    // For Opus/Vorbis, check all fields
    if (opusFile) {
        TagLib::Ogg::XiphComment *tag = opusFile->tag();
        if (tag) {
            debugInfo += "Opus tag found: YES\n";
            debugInfo += "Available fields:\n";
            
            TagLib::PropertyMap props = tag->properties();
            for (auto it = props.begin(); it != props.end(); ++it) {
                debugInfo += "  - " + it->first.to8Bit(true) + "\n";
            }
            
            // Check fieldListMap for all keys
            debugInfo += "All tag fields:\n";
            const TagLib::Ogg::FieldListMap &fieldMap = tag->fieldListMap();
            for (auto it = fieldMap.begin(); it != fieldMap.end(); ++it) {
                debugInfo += "  - " + it->first.upper().to8Bit(true) + "\n";
            }
            
            if (tag->fieldListMap().contains("METADATA_BLOCK_PICTURE")) {
                debugInfo += "METADATA_BLOCK_PICTURE: FOUND\n";
            } else {
                debugInfo += "METADATA_BLOCK_PICTURE: NOT FOUND\n";
            }
            
            // Check for COVERART alternative
            if (tag->fieldListMap().contains("COVERART")) {
                debugInfo += "COVERART: FOUND\n";
                TagLib::StringList coverArtList = tag->fieldListMap()["COVERART"];
                debugInfo += "COVERART count: " + std::to_string(coverArtList.size()) + "\n";
                if (!coverArtList.isEmpty()) {
                    TagLib::ByteVector encodedData = coverArtList.front().data(TagLib::String::Latin1);
                    debugInfo += "COVERART encoded size: " + std::to_string(encodedData.size()) + " bytes\n";
                    TagLib::ByteVector decodedData = TagLib::ByteVector::fromBase64(encodedData);
                    debugInfo += "COVERART decoded size: " + std::to_string(decodedData.size()) + " bytes\n";
                }
            } else {
                debugInfo += "COVERART: NOT FOUND\n";
            }
            
            if (tag->fieldListMap().contains("COVERARTMIME")) {
                debugInfo += "COVERARTMIME: " + tag->fieldListMap()["COVERARTMIME"].front().to8Bit(true) + "\n";
            }
            
            debugInfo += "\nNote: Your Opus file may have external artwork or embedded in stream.\n";
            debugInfo += "TagLib may not support all Opus artwork methods.\n";
        } else {
            debugInfo += "Opus tag found: NO\n";
        }
    }

    if (oggFile) {
        TagLib::Ogg::XiphComment *tag = oggFile->tag();
        if (tag) {
            debugInfo += "Vorbis tag found: YES\n";
            debugInfo += "Available fields:\n";
            
            TagLib::PropertyMap props = tag->properties();
            for (auto it = props.begin(); it != props.end(); ++it) {
                debugInfo += "  - " + it->first.to8Bit(true) + "\n";
            }
            
            if (tag->fieldListMap().contains("METADATA_BLOCK_PICTURE")) {
                debugInfo += "METADATA_BLOCK_PICTURE: FOUND\n";
            } else {
                debugInfo += "METADATA_BLOCK_PICTURE: NOT FOUND\n";
            }
        }
    }

    if (mp3File && mp3File->ID3v2Tag()) {
        TagLib::ID3v2::FrameList frames = mp3File->ID3v2Tag()->frameListMap()["APIC"];
        debugInfo += "APIC frames found: " + std::to_string(frames.size()) + "\n";
    }

    if (flacFile) {
        debugInfo += "FLAC pictures: " + std::to_string(flacFile->pictureList().size()) + "\n";
    }

    return env->NewStringUTF(debugInfo.c_str());
}

}
