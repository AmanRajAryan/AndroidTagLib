#include <jni.h>
#include <string>
#include <vector>
#include <map>

#include "fileref.h"
#include "tag.h"
#include "tpropertymap.h"
#include "tstringlist.h"
#include "tbytevector.h"
#include "tbytevectorlist.h"

#include "audioproperties.h"

#include "mpeg/id3v2/id3v2tag.h"
#include "mpeg/id3v2/frames/attachedpictureframe.h"
#include "mpeg/id3v2/frames/textidentificationframe.h" 
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
#include "apefile.h" 

extern "C" {

int getBitDepth(TagLib::File* file) {
    if (!file) return 0;

    if (auto *f = dynamic_cast<TagLib::FLAC::File*>(file)) {
        if (f->audioProperties()) return f->audioProperties()->bitsPerSample();
    }
    else if (auto *f = dynamic_cast<TagLib::RIFF::WAV::File*>(file)) {
        if (f->audioProperties()) return f->audioProperties()->bitsPerSample();
    }
    else if (auto *f = dynamic_cast<TagLib::RIFF::AIFF::File*>(file)) {
        if (f->audioProperties()) return f->audioProperties()->bitsPerSample();
    }
    else if (auto *f = dynamic_cast<TagLib::APE::File*>(file)) {
        if (f->audioProperties()) return f->audioProperties()->bitsPerSample();
    }
    else if (auto *f = dynamic_cast<TagLib::MP4::File*>(file)) {
        if (f->audioProperties()) {
            return f->audioProperties()->bitsPerSample();
        }
    }

    return 0;
}

const char* getCodecName(TagLib::File* file) {
    if (!file) return "";

    if (dynamic_cast<TagLib::MPEG::File*>(file)) return "MP3";
    if (dynamic_cast<TagLib::FLAC::File*>(file)) return "FLAC";
    if (dynamic_cast<TagLib::Ogg::Vorbis::File*>(file)) return "Vorbis";
    if (dynamic_cast<TagLib::Ogg::Opus::File*>(file)) return "Opus";
    if (dynamic_cast<TagLib::RIFF::WAV::File*>(file)) return "WAV";
    if (dynamic_cast<TagLib::RIFF::AIFF::File*>(file)) return "AIFF";
    if (dynamic_cast<TagLib::APE::File*>(file)) return "APE";
    if (dynamic_cast<TagLib::ASF::File*>(file)) return "WMA";

    if (auto *mp4 = dynamic_cast<TagLib::MP4::File*>(file)) {
        if (mp4->audioProperties()) {
            if (mp4->audioProperties()->bitsPerSample() > 0) return "ALAC";
        }
        return "AAC";
    }

    return ""; 
}

// Helper to create Java Artwork objects
jobject createArtworkObject(JNIEnv *env, jbyteArray data, const char* mimeTypeStr, const char* descStr) {
    jclass artworkClass = env->FindClass("aman/taglib/TagLib$Artwork");
    if(!artworkClass) return nullptr;
    jmethodID constructor = env->GetMethodID(artworkClass, "<init>", "([BLjava/lang/String;Ljava/lang/String;)V");
    jstring jMime = env->NewStringUTF(mimeTypeStr ? mimeTypeStr : "");
    jstring jDesc = env->NewStringUTF(descStr ? descStr : "");
    jobject obj = env->NewObject(artworkClass, constructor, data, jMime, jDesc);
    env->DeleteLocalRef(jMime);
    env->DeleteLocalRef(jDesc);
    env->DeleteLocalRef(artworkClass);
    return obj;
}

void addTag(std::vector<std::string>& vec, const std::string& key, const std::string& val) {
    if (!key.empty() && !val.empty()) {
        vec.push_back(key);
        vec.push_back(val);
    }
}

void fetchExtraTags(TagLib::File* f, std::vector<std::string>& tags) {
    if (auto *mp4 = dynamic_cast<TagLib::MP4::File*>(f)) {
        if (mp4->tag() && mp4->tag()->itemMap().contains("rtng")) {
            TagLib::MP4::Item item = mp4->tag()->itemMap()["rtng"];
            int rating = item.toInt();
            if (rating == 0) {
                TagLib::ByteVectorList dataList = item.toByteVectorList();
                if (!dataList.isEmpty() && dataList.front().size() >= 1) {
                    rating = (unsigned char)dataList.front()[0];
                }
            }
            if (rating > 0) {
                addTag(tags, "CONTENT_RATING", std::to_string(rating));
            }
        }
    }
    else if (auto *mp3 = dynamic_cast<TagLib::MPEG::File*>(f)) {
        if (mp3->ID3v2Tag()) {
            const TagLib::ID3v2::FrameList &frames = mp3->ID3v2Tag()->frameList("TXXX");
            for (auto *frame : frames) {
                auto *txxx = dynamic_cast<TagLib::ID3v2::UserTextIdentificationFrame*>(frame);
                if (txxx && txxx->description().upper() == "ITUNESADVISORY") {
                    std::string val;
                    if (txxx->fieldList().size() >= 2) val = txxx->fieldList()[1].to8Bit(true);
                    else val = txxx->fieldList().toString().to8Bit(true);
                    addTag(tags, "CONTENT_RATING", val);
                }
            }
        }
    }
}

void saveExtraTags(TagLib::File* f, const std::string& key, const std::string& value) {
    if (key != "CONTENT_RATING") return;

    int ratingVal = 0;
    try { ratingVal = std::stoi(value); } catch (...) { return; }

    if (auto *mp4 = dynamic_cast<TagLib::MP4::File*>(f)) {
        if (mp4->tag()) {
            TagLib::MP4::Item item(ratingVal);
            mp4->tag()->setItem("rtng", item);
        }
    }
    
    else if (auto *mp3 = dynamic_cast<TagLib::MPEG::File*>(f)) {
        if (mp3->ID3v2Tag()) {
            TagLib::ID3v2::Tag *tag = mp3->ID3v2Tag();
            
            TagLib::ID3v2::FrameList frames = tag->frameList("TXXX");
            for(auto *frame : frames) {
                auto *txxx = dynamic_cast<TagLib::ID3v2::UserTextIdentificationFrame*>(frame);
                if(txxx && txxx->description().upper() == "ITUNESADVISORY") {
                    tag->removeFrame(frame);
                }
            }
            
            auto *frame = new TagLib::ID3v2::UserTextIdentificationFrame();
            frame->setDescription("ITUNESADVISORY");
            frame->setText(value);
            tag->addFrame(frame);
        }
    }
}

JNIEXPORT jobjectArray JNICALL Java_aman_taglib_TagLib_getMetadataNative(JNIEnv* env, jclass /*clazz*/, jstring filePath) {
    const char* pathChars = env->GetStringUTFChars(filePath, nullptr);
    TagLib::FileRef f(pathChars);
    env->ReleaseStringUTFChars(filePath, pathChars);

    if (f.isNull() || !f.file()) return nullptr;

    std::vector<std::string> tags;
    tags.reserve(32);

    {
        TagLib::PropertyMap properties = f.file()->properties();
        for (auto it = properties.begin(); it != properties.end(); ++it) {
            std::string key = it->first.to8Bit(true);
            std::string value = it->second.toString(", ").to8Bit(true);
            for (auto &c: key) c = toupper(c);
            addTag(tags, key, value);
        }

        if (!properties.contains("TITLE") && !f.tag()->title().isEmpty())
            addTag(tags, "TITLE", f.tag()->title().to8Bit(true));
        if (!properties.contains("ARTIST") && !f.tag()->artist().isEmpty())
            addTag(tags, "ARTIST", f.tag()->artist().to8Bit(true));
        if (!properties.contains("ALBUM") && !f.tag()->album().isEmpty())
            addTag(tags, "ALBUM", f.tag()->album().to8Bit(true));

        if (f.file()->audioProperties()) {
            TagLib::AudioProperties *audioProps = f.file()->audioProperties();
            auto putInt = [&](const std::string& key, int value) {
                addTag(tags, key, std::to_string(value));
            };
            putInt("BITRATE",      audioProps->bitrate());
            putInt("SAMPLERATE",   audioProps->sampleRate());
            putInt("CHANNELS",     audioProps->channels());
            putInt("DURATION_SEC", audioProps->lengthInSeconds());

            int bitDepth = getBitDepth(f.file());
            if (bitDepth > 0) putInt("BITS_PER_SAMPLE", bitDepth);
        }

        const char* formatName = getCodecName(f.file());
        if (formatName && *formatName) addTag(tags, "FORMAT", formatName);

        fetchExtraTags(f.file(), tags);
    }

    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray result = env->NewObjectArray(tags.size(), stringClass, nullptr);
    env->DeleteLocalRef(stringClass);
    for (size_t i = 0; i < tags.size(); ++i) {
        jstring jStr = env->NewStringUTF(tags[i].c_str());
        env->SetObjectArrayElement(result, i, jStr);
        env->DeleteLocalRef(jStr);
    }
    return result;
}

JNIEXPORT jboolean JNICALL
Java_aman_taglib_TagLib_setMetadataNative(JNIEnv *env, jclass /*clazz*/, jstring jFilePath, jobjectArray jRawData) {
    if (!jFilePath || !jRawData) return JNI_FALSE;
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    if (!filePath) return JNI_FALSE;
    TagLib::FileRef f(filePath);
    
    if (f.isNull() || !f.file()) {
        env->ReleaseStringUTFChars(jFilePath, filePath);
        return JNI_FALSE;
    }

    TagLib::PropertyMap fileProps = f.file()->properties();
    jsize len = env->GetArrayLength(jRawData);

    for (jsize i = 0; i + 1 < len; i += 2) {
        jstring keyStr = (jstring)env->GetObjectArrayElement(jRawData, i);
        jstring valStr = (jstring)env->GetObjectArrayElement(jRawData, i + 1);
        if (!keyStr) continue;

        const char *keyC = env->GetStringUTFChars(keyStr, nullptr);
        const char *valC = valStr ? env->GetStringUTFChars(valStr, nullptr) : "";

        if (keyC) {
            TagLib::String tKey(keyC, TagLib::String::UTF8);
            TagLib::String tVal(valC, TagLib::String::UTF8);

            if (tVal.isEmpty()) {
                fileProps.erase(tKey);
            } else {
                fileProps.replace(tKey, tVal);
            }

            saveExtraTags(f.file(), std::string(keyC), std::string(valC));
            env->ReleaseStringUTFChars(keyStr, keyC);
        }

        if (valStr && valC && *valC) {
            env->ReleaseStringUTFChars(valStr, valC);
        }

        env->DeleteLocalRef(keyStr);
        if (valStr) env->DeleteLocalRef(valStr);
    }

    f.file()->setProperties(fileProps);
    bool ok = f.file()->save();
    
    env->ReleaseStringUTFChars(jFilePath, filePath);
    return ok ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jobjectArray JNICALL
Java_aman_taglib_TagLib_getArtwork(JNIEnv *env, jclass /*clazz*/, jstring jFilePath) {
    const char *filePath = env->GetStringUTFChars(jFilePath, nullptr);
    if (!filePath) return nullptr;
    TagLib::FileRef f(filePath);
    env->ReleaseStringUTFChars(jFilePath, filePath);
    if (f.isNull() || !f.file()) return nullptr;

    std::vector<jobject> artworks;

    if (TagLib::MP4::File *mp4File = dynamic_cast<TagLib::MP4::File*>(f.file())) {
        if (mp4File->tag() && mp4File->tag()->itemMap().contains("covr")) {
            TagLib::MP4::CoverArtList coverArtList = mp4File->tag()->itemMap()["covr"].toCoverArtList();
            for (auto &cover : coverArtList) {
                TagLib::ByteVector data = cover.data();
                jbyteArray jData = env->NewByteArray(data.size());
                env->SetByteArrayRegion(jData, 0, data.size(), reinterpret_cast<const jbyte*>(data.data()));
                const char* mime = (cover.format() == TagLib::MP4::CoverArt::PNG) ? "image/png" : "image/jpeg";
                artworks.push_back(createArtworkObject(env, jData, mime, ""));
                env->DeleteLocalRef(jData);
            }
        }
    }
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
                   env->DeleteLocalRef(jData);
                }
            }
        }
    }
    else if (TagLib::MPEG::File *mp3File = dynamic_cast<TagLib::MPEG::File*>(f.file())) {
        if (mp3File->ID3v2Tag()) {
            TagLib::ID3v2::FrameList frames = mp3File->ID3v2Tag()->frameListMap()["APIC"];
            for (auto *frame : frames) {
                auto *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);
                jbyteArray jData = env->NewByteArray(pic->picture().size());
                env->SetByteArrayRegion(jData, 0, pic->picture().size(), reinterpret_cast<const jbyte*>(pic->picture().data()));
                artworks.push_back(createArtworkObject(env, jData, pic->mimeType().toCString(true), pic->description().toCString(true)));
                env->DeleteLocalRef(jData);
            }
        }
    }
    else if (TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file())) {
        for (auto *pic : flacFile->pictureList()) {
             jbyteArray jData = env->NewByteArray(pic->data().size());
             env->SetByteArrayRegion(jData, 0, pic->data().size(), reinterpret_cast<const jbyte*>(pic->data().data()));
             artworks.push_back(createArtworkObject(env, jData, pic->mimeType().toCString(true), pic->description().toCString(true)));
             env->DeleteLocalRef(jData);
        }
    }
    else if (TagLib::Ogg::XiphComment *xiphTag = dynamic_cast<TagLib::Ogg::XiphComment*>(f.tag())) {
         for (auto *pic : xiphTag->pictureList()) {
             jbyteArray jData = env->NewByteArray(pic->data().size());
             env->SetByteArrayRegion(jData, 0, pic->data().size(), reinterpret_cast<const jbyte*>(pic->data().data()));
             artworks.push_back(createArtworkObject(env, jData, pic->mimeType().toCString(true), pic->description().toCString(true)));
             env->DeleteLocalRef(jData);
         }
    }
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
                env->DeleteLocalRef(jData);
            }
        }
    }
    
    jclass artworkClass = env->FindClass("aman/taglib/TagLib$Artwork");
    jobjectArray result = env->NewObjectArray(artworks.size(), artworkClass, nullptr);
    env->DeleteLocalRef(artworkClass);
    for (size_t i = 0; i < artworks.size(); ++i) {
        env->SetObjectArrayElement(result, i, artworks[i]);
        env->DeleteLocalRef(artworks[i]);
    }
    return result;
}

JNIEXPORT jboolean JNICALL
Java_aman_taglib_TagLib_setArtwork(JNIEnv *env, jclass /*clazz*/, jstring jFilePath, 
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

    auto cleanLegacyXiph = [](TagLib::Ogg::XiphComment *tag) {
        if (!tag) return;
        tag->removeAllPictures(); 
        tag->removeFields("COVERART");          
        tag->removeFields("COVERARTMIME");      
        tag->removeFields("ANDROID_COVER_ART"); 
    };

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
    else if (TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File*>(f.file())) {
        flacFile->removePictures();
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
    else if (TagLib::Ogg::XiphComment *xiph = dynamic_cast<TagLib::Ogg::XiphComment*>(f.tag())) {
        cleanLegacyXiph(xiph); 
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
Java_aman_taglib_TagLib_getArtworkDebugInfo(JNIEnv *env, jclass /*clazz*/, jstring jFilePath) {
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
