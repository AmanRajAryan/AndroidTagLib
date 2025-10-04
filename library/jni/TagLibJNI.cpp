#include "flacfile.h"
#include "vorbisfile.h"
#include "opusfile.h"
#include "tpropertymap.h"
#include "mpeg/id3v2/id3v2.h"

// Skeletal JNI wrapper for TagLib
#include <jni.h>
#include <string>
#include "fileref.h"
#include "tag.h"
#include "mpegfile.h"
#include "id3v2tag.h"
#include "id3v2frame.h"

extern "C" {

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



extern "C" JNIEXPORT jboolean JNICALL
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

}
