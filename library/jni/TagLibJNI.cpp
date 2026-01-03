#include <jni.h>
#include <string>
#include <vector>
#include <map>

// TagLib Core
#include "fileref.h"
#include "tag.h"
#include "tpropertymap.h"
#include "tstringlist.h"
#include "tbytevector.h"

// Format Specific Headers
#include "mpeg/id3v2/id3v2tag.h"
#include "mpeg/id3v2/frames/attachedpictureframe.h"
#include "mpegfile.h"
#include "flacfile.h"
#include "flacpicture.h"
#include "vorbisfile.h"
#include "opusfile.h"

#include "mp4file.h"
#include "mp4tag.h"
#include "mp4item.h"
#include "mp4coverart.h"
#include "asffile.h"
#include "asftag.h"
#include "asfattribute.h"
#include "asfpicture.h"
#include "wavfile.h"
#include "aifffile.h"

extern "C" {

jobject createArtworkObject(JNIEnv *env, jbyteArray data, const char* mimeTypeStr, const char* descStr) {
    jclass artworkClass = env->FindClass("aman/taglib/TagLib$Artwork");
    if(!artworkClass) return nullptr;
    jmethodID constructor = env->GetMethodID(artworkClass, "<init>", "([BLjava/lang/String;Ljava/lang/String;)V");
    jstring jMime = env->NewStringUTF(mimeTypeStr ? mimeTypeStr : "");
    jstring jDesc = env->NewStringUTF(descStr ? descStr : "");
    return env->NewObject(artworkClass, constructor, data, jMime, jDesc);
}

JNIEXPORT jobject JNICALL Java_aman_taglib_TagLib_getMetadata(JNIEnv* env, jobject thiz, jstring filePath) {
    const char* pathChars = env->GetStringUTFChars(filePath, nullptr);
    TagLib::FileRef f(pathChars);
    env->ReleaseStringUTFChars(filePath, pathChars);

    jclass hashMapClass = env->FindClass("java/util/HashMap");
    jobject mapObj = env->NewObject(hashMapClass, env->GetMethodID(hashMapClass, "<init>", "()V"));
    jmethodID putMethod = env->GetMethodID(hashMapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    if (!f.isNull() && f.file()) {
        TagLib::PropertyMap properties = f.file()->properties();
        for (auto it = properties.begin(); it != properties.end(); ++it) {
            std::string key = it->first.to8Bit(true);
            std::string value = it->second.toString(", ").to8Bit(true);
            env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF(key.c_str()), env->NewStringUTF(value.c_str()));
        }
        if(!properties.contains("TITLE") && !f.tag()->title().isEmpty())
             env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("TITLE"), env->NewStringUTF(f.tag()->title().to8Bit(true).c_str()));
        if(!properties.contains("ARTIST") && !f.tag()->artist().isEmpty())
             env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("ARTIST"), env->NewStringUTF(f.tag()->artist().to8Bit(true).c_str()));
        if(!properties.contains("ALBUM") && !f.tag()->album().isEmpty())
             env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("ALBUM"), env->NewStringUTF(f.tag()->album().to8Bit(true).c_str()));
    }
    return mapObj;
}

JNIEXPORT jboolean JNICALL
Java_aman_taglib_TagLib_setMetadata(JNIEnv *env, jobject /*thiz*/, jstring jFilePath, jobject jMetadataMap) {
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    if (!filePath) return JNI_FALSE;
    TagLib::FileRef f(filePath);
    
    if (f.isNull() || !f.file()) {
        env->ReleaseStringUTFChars(jFilePath, filePath);
        return JNI_FALSE;
    }

    TagLib::PropertyMap fileProps = f.file()->properties();
    jclass mapClass = env->GetObjectClass(jMetadataMap);
    jmethodID entrySet = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    jobject setObj = env->CallObjectMethod(jMetadataMap, entrySet);
    jclass setClass = env->GetObjectClass(setObj);
    jmethodID iterator = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterObj = env->CallObjectMethod(setObj, iterator);
    jclass iterClass = env->GetObjectClass(iterObj);
    jmethodID hasNext = env->GetMethodID(iterClass, "hasNext", "()Z");
    jmethodID next = env->GetMethodID(iterClass, "next", "()Ljava/lang/Object;");

    while (env->CallBooleanMethod(iterObj, hasNext)) {
        jobject entryObj = env->CallObjectMethod(iterObj, next);
        jclass entryClass = env->GetObjectClass(entryObj);
        jmethodID getKey = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
        jmethodID getValue = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

        jstring keyStr = (jstring)env->CallObjectMethod(entryObj, getKey);
        jstring valStr = (jstring)env->CallObjectMethod(entryObj, getValue);
        
        const char *keyC = env->GetStringUTFChars(keyStr, nullptr);
        const char *valC = env->GetStringUTFChars(valStr, nullptr);

        TagLib::String tKey(keyC, TagLib::String::UTF8);
        TagLib::String tVal(valC, TagLib::String::UTF8);

        if (tVal.isEmpty()) {
            fileProps.erase(tKey);
        } else {
            fileProps.replace(tKey, tVal);
        }

        env->ReleaseStringUTFChars(keyStr, keyC);
        env->ReleaseStringUTFChars(valStr, valC);
        env->DeleteLocalRef(entryObj);
    }

    f.file()->setProperties(fileProps);
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

    // MP4
    if (TagLib::MP4::File *mp4File = dynamic_cast<TagLib::MP4::File*>(f.file())) {
        if (mp4File->tag() && mp4File->tag()->itemMap().contains("covr")) {
            TagLib::MP4::CoverArtList coverArtList = mp4File->tag()->itemMap()["covr"].toCoverArtList();
            for (auto &cover : coverArtList) {
                TagLib::ByteVector data = cover.data();
                jbyteArray jData = env->NewByteArray(data.size());
                env->SetByteArrayRegion(jData, 0, data.size(), reinterpret_cast<const jbyte*>(data.data()));
                const char* mime = (cover.format() == TagLib::MP4::CoverArt::PNG) ? "image/png" : "image/jpeg";
                artworks.push_back(createArtworkObject(env, jData, mime, ""));
            }
        }
    }
    // ASF
    else if (TagLib::ASF::File *asfFile = dynamic_cast<TagLib::ASF::File*>(f.file())) {
        if (asfFile->tag() && asfFile->tag()->attributeListMap().contains("WM/Picture")) {
            TagLib::ASF::AttributeList attrs = asfFile->tag()->attributeListMap()["WM/Picture"];
            for (auto &attr : attrs) {
                TagLib::ASF::Picture pic = attr.toPicture();
                if(pic.isValid()) {
                   TagLib::ByteVector data = pic.picture();
                   jbyteArray jData = env->NewByteArray(data.size());
                   env->SetByteArrayRegion(jData, 0, data.size(), reinterpret_cast<const jbyte*>(data.data()));
                   artworks.push_back(createArtworkObject(env, jData, pic.mimeType().toCString(true), pic.description().toCString(true)));
                }
            }
        }
    }
    // MP3
    else if (TagLib::MPEG::File *mp3File = dynamic_cast<TagLib::MPEG::File*>(f.file())) {
        if (mp3File->ID3v2Tag()) {
            TagLib::ID3v2::FrameList frames = mp3File->ID3v2Tag()->frameListMap()["APIC"];
            for (auto *frame : frames) {
                auto *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);
                jbyteArray jData = env->NewByteArray(pic->picture().size());
                env->SetByteArrayRegion(jData, 0, pic->picture().size(), reinterpret_cast<const jbyte*>(pic->picture().data()));
                artworks.push_back(createArtworkObject(env, jData, pic->mimeType().toCString(true), pic->description().toCString(true)));
            }
        }
    }
    // FLAC
    else if (TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file())) {
        for (auto *pic : flacFile->pictureList()) {
             jbyteArray jData = env->NewByteArray(pic->data().size());
             env->SetByteArrayRegion(jData, 0, pic->data().size(), reinterpret_cast<const jbyte*>(pic->data().data()));
             artworks.push_back(createArtworkObject(env, jData, pic->mimeType().toCString(true), pic->description().toCString(true)));
        }
    }
    // OGG / OPUS
    else if (TagLib::Ogg::XiphComment *xiphTag = dynamic_cast<TagLib::Ogg::XiphComment*>(f.tag())) {
         for (auto *pic : xiphTag->pictureList()) {
             jbyteArray jData = env->NewByteArray(pic->data().size());
             env->SetByteArrayRegion(jData, 0, pic->data().size(), reinterpret_cast<const jbyte*>(pic->data().data()));
             artworks.push_back(createArtworkObject(env, jData, pic->mimeType().toCString(true), pic->description().toCString(true)));
         }
    }
    // RIFF (WAV/AIFF)
    else {
        TagLib::ID3v2::Tag *id3v2 = nullptr;
        if (auto *wavFile = dynamic_cast<TagLib::RIFF::WAV::File*>(f.file())) id3v2 = wavFile->ID3v2Tag();
        else if (auto *aiffFile = dynamic_cast<TagLib::RIFF::AIFF::File*>(f.file())) id3v2 = dynamic_cast<TagLib::ID3v2::Tag*>(aiffFile->tag());
        if (id3v2) {
             TagLib::ID3v2::FrameList frames = id3v2->frameListMap()["APIC"];
             for (auto *frame : frames) {
                auto *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);
                jbyteArray jData = env->NewByteArray(pic->picture().size());
                env->SetByteArrayRegion(jData, 0, pic->picture().size(), reinterpret_cast<const jbyte*>(pic->picture().data()));
                artworks.push_back(createArtworkObject(env, jData, pic->mimeType().toCString(true), pic->description().toCString(true)));
            }
        }
    }
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

    jsize imageSize = env->GetArrayLength(jImageData);
    bool isDelete = (imageSize == 0);

    TagLib::ByteVector imageData;
    if (!isDelete) {
        jbyte *imageBytes = env->GetByteArrayElements(jImageData, nullptr);
        imageData = TagLib::ByteVector(reinterpret_cast<const char*>(imageBytes), imageSize);
        env->ReleaseByteArrayElements(jImageData, imageBytes, JNI_ABORT);
    }

    const char *mimeC = env->GetStringUTFChars(jMimeType, nullptr);
    const char *descC = env->GetStringUTFChars(jDescription, nullptr);
    TagLib::String mimeType(mimeC, TagLib::String::UTF8);
    TagLib::String description(descC, TagLib::String::UTF8);
    env->ReleaseStringUTFChars(jMimeType, mimeC);
    env->ReleaseStringUTFChars(jDescription, descC);

    bool success = false;

    // --- HELPER LAMBDA FOR XIPH CLEANUP (FLAC/OGG/OPUS) ---
    // FIXED: removeField -> removeFields
    auto cleanLegacyXiph = [](TagLib::Ogg::XiphComment *tag) {
        if (!tag) return;
        tag->removeAllPictures(); 
        tag->removeFields("COVERART");          // Fixed Plural
        tag->removeFields("COVERARTMIME");      // Fixed Plural
        tag->removeFields("ANDROID_COVER_ART"); // Fixed Plural
    };

    // 1. MP4 / M4A
    if (TagLib::MP4::File *mp4File = dynamic_cast<TagLib::MP4::File*>(f.file())) {
        TagLib::MP4::Tag *tag = mp4File->tag();
        if(tag) {
            if (isDelete) {
                tag->removeItem("covr");
            } else {
                TagLib::MP4::CoverArt::Format fmt = TagLib::MP4::CoverArt::JPEG;
                if(mimeType == "image/png") fmt = TagLib::MP4::CoverArt::PNG;
                TagLib::MP4::CoverArt coverArt(fmt, imageData);
                TagLib::MP4::CoverArtList coverArtList;
                coverArtList.append(coverArt);
                TagLib::MP4::Item coverItem(coverArtList);
                tag->setItem("covr", coverItem); 
            }
            success = mp4File->save();
        }
    }
    // 2. WMA / ASF
    else if (TagLib::ASF::File *asfFile = dynamic_cast<TagLib::ASF::File*>(f.file())) {
         TagLib::ASF::Tag *tag = asfFile->tag();
         if(tag) {
             tag->removeItem("WM/Picture");
             if (!isDelete) {
                 TagLib::ASF::Picture pic;
                 pic.setPicture(imageData);
                 pic.setMimeType(mimeType);
                 pic.setDescription(description);
                 pic.setType(TagLib::ASF::Picture::FrontCover);
                 TagLib::ASF::Attribute attr(pic);
                 tag->setAttribute("WM/Picture", attr);
             }
             success = asfFile->save();
         }
    }
    // 3. FLAC
    else if (TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file())) {
        flacFile->removePictures();
        // FLAC also has a XiphComment where legacy tags might hide
        if (flacFile->xiphComment()) cleanLegacyXiph(flacFile->xiphComment());
        
        if (!isDelete) {
            TagLib::FLAC::Picture *pic = new TagLib::FLAC::Picture();
            pic->setData(imageData);
            pic->setMimeType(mimeType);
            pic->setDescription(description);
            pic->setType(TagLib::FLAC::Picture::FrontCover);
            flacFile->addPicture(pic);
        }
        success = flacFile->save();
    }
    // 4. OGG / OPUS (Generic Xiph)
    else if (TagLib::Ogg::XiphComment *xiph = dynamic_cast<TagLib::Ogg::XiphComment*>(f.tag())) {
        cleanLegacyXiph(xiph); // NUKE EVERYTHING
        if (!isDelete) {
            TagLib::FLAC::Picture *pic = new TagLib::FLAC::Picture();
            pic->setData(imageData);
            pic->setMimeType(mimeType);
            pic->setDescription(description);
            pic->setType(TagLib::FLAC::Picture::FrontCover);
            xiph->addPicture(pic);
        }
        success = f.save(); 
    }
    // 5. MP3 / WAV / AIFF
    else {
        TagLib::ID3v2::Tag *id3v2 = nullptr;
        TagLib::File *targetFile = f.file();
        if (auto *mp3 = dynamic_cast<TagLib::MPEG::File*>(f.file())) id3v2 = mp3->ID3v2Tag(true);
        else if (auto *wav = dynamic_cast<TagLib::RIFF::WAV::File*>(f.file())) id3v2 = wav->ID3v2Tag();
        else if (auto *aiff = dynamic_cast<TagLib::RIFF::AIFF::File*>(f.file())) id3v2 = dynamic_cast<TagLib::ID3v2::Tag*>(aiff->tag());

        if (id3v2 && targetFile) {
            id3v2->removeFrames("APIC");
            if (!isDelete) {
                auto *frame = new TagLib::ID3v2::AttachedPictureFrame();
                frame->setMimeType(mimeType);
                frame->setPicture(imageData);
                frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
                frame->setDescription(description);
                id3v2->addFrame(frame);
            }
            success = targetFile->save();
        }
    }
    return success ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_aman_taglib_TagLib_getArtworkDebugInfo(JNIEnv *env, jobject /*thiz*/, jstring jFilePath) {
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    TagLib::FileRef f(filePath);
    env->ReleaseStringUTFChars(jFilePath, filePath);
    if (f.isNull() || !f.file()) return env->NewStringUTF("Error: Invalid file");
    std::string debug = "Properties found:\n";
    TagLib::PropertyMap props = f.file()->properties();
    for(auto it : props) {
        debug += it.first.to8Bit(true) + " : " + it.second.toString().to8Bit(true) + "\n";
    }
    return env->NewStringUTF(debug.c_str());
}
} // extern "C"
