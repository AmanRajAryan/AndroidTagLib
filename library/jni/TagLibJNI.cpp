#include "flacfile.h"
#include "vorbisfile.h"
#include "opusfile.h"
#include "tpropertymap.h"
#include "mpeg/id3v2/id3v2.h"
#include "mpeg/id3v2/frames/attachedpictureframe.h"
#include "flacpicture.h"

// Skeletal JNI wrapper for TagLib
#include <jni.h>
#include <string>
#include "fileref.h"
#include "tag.h"
#include "mpegfile.h"
#include "id3v2tag.h"
#include "id3v2frame.h"

extern "C" {

// ------------------ GET METADATA + ARTWORK ------------------

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

        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("title"), env->NewStringUTF(tag->title().to8Bit(true)));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("artist"), env->NewStringUTF(tag->artist().to8Bit(true)));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("album"), env->NewStringUTF(tag->album().to8Bit(true)));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("genre"), env->NewStringUTF(tag->genre().to8Bit(true)));
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("comment"), env->NewStringUTF(tag->comment().to8Bit(true)));
    }

    // ------------------ Extract Artwork ------------------
    TagLib::ByteVector imageData;
    std::string mimeType;

    if (TagLib::MPEG::File mpegFile(path.c_str()); mpegFile.isValid() && mpegFile.ID3v2Tag()) {
        TagLib::ID3v2::Tag* id3v2 = mpegFile.ID3v2Tag();
        TagLib::ID3v2::FrameList frames = id3v2->frameList("APIC");
        if (!frames.isEmpty()) {
            auto* picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
            imageData = picFrame->picture();
            mimeType = picFrame->mimeType().to8Bit(true);
        }
    } 
    else if (TagLib::FLAC::File flacFile(path.c_str()); flacFile.isValid()) {
        auto picList = flacFile.pictureList();
        if (!picList.isEmpty()) {
            imageData = picList.front()->data();
            mimeType = picList.front()->mimeType().to8Bit(true);
        }
    }
    else if (TagLib::Ogg::Opus::File opusFile(path.c_str()); opusFile.isValid()) {
        auto picList = opusFile.tag()->pictureList();
        if (!picList.isEmpty()) {
            imageData = picList.front()->data();
            mimeType = picList.front()->mimeType().to8Bit(true);
        }
    }
    else if (TagLib::Ogg::Vorbis::File vorbisFile(path.c_str()); vorbisFile.isValid()) {
        auto picList = vorbisFile.tag()->pictureList();
        if (!picList.isEmpty()) {
            imageData = picList.front()->data();
            mimeType = picList.front()->mimeType().to8Bit(true);
        }
    }

    if (!imageData.isEmpty()) {
        jbyteArray artArray = env->NewByteArray(imageData.size());
        env->SetByteArrayRegion(artArray, 0, imageData.size(), (const jbyte*)imageData.data());
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("artwork"), artArray);
        env->CallObjectMethod(mapObj, putMethod, env->NewStringUTF("artwork_mime"), env->NewStringUTF(mimeType.c_str()));
    }

    return mapObj;
}

// ------------------ SET ARTWORK ------------------

JNIEXPORT jboolean JNICALL Java_aman_taglib_TagLib_setArtwork(JNIEnv* env, jobject thiz, jstring filePath, jbyteArray artData, jstring mimeTypeStr) {
    const char* pathChars = env->GetStringUTFChars(filePath, nullptr);
    std::string path(pathChars);
    env->ReleaseStringUTFChars(filePath, pathChars);

    const char* mimeChars = env->GetStringUTFChars(mimeTypeStr, nullptr);
    std::string mimeType(mimeChars);
    env->ReleaseStringUTFChars(mimeTypeStr, mimeChars);

    jsize len = env->GetArrayLength(artData);
    std::vector<unsigned char> buffer(len);
    env->GetByteArrayRegion(artData, 0, len, reinterpret_cast<jbyte*>(buffer.data()));

    TagLib::ByteVector data(reinterpret_cast<const char*>(buffer.data()), buffer.size());

    bool success = false;

    // Common picture object
    TagLib::FLAC::Picture pic;
    pic.setMimeType(mimeType);
    pic.setType(TagLib::FLAC::Picture::FrontCover);
    pic.setData(data);

    if (TagLib::MPEG::File mpegFile(path.c_str()); mpegFile.isValid() && mpegFile.ID3v2Tag()) {
        TagLib::ID3v2::Tag* id3v2 = mpegFile.ID3v2Tag();
        id3v2->removeFrames("APIC");
        auto* frame = new TagLib::ID3v2::AttachedPictureFrame;
        frame->setMimeType(mimeType);
        frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
        frame->setPicture(data);
        id3v2->addFrame(frame);
        mpegFile.save();
        success = true;
    } 
    else if (TagLib::FLAC::File flacFile(path.c_str()); flacFile.isValid()) {
        flacFile.removePictures();
        flacFile.addPicture(pic);
        flacFile.save();
        success = true;
    }
    else if (TagLib::Ogg::Opus::File opusFile(path.c_str()); opusFile.isValid()) {
        auto tag = opusFile.tag();
        tag->addPicture(pic);
        opusFile.save();
        success = true;
    }
    else if (TagLib::Ogg::Vorbis::File vorbisFile(path.c_str()); vorbisFile.isValid()) {
        auto tag = vorbisFile.tag();
        tag->addPicture(pic);
        vorbisFile.save();
        success = true;
    }

    return success ? JNI_TRUE : JNI_FALSE;
}

} // extern "C"
