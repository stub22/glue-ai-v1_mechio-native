/* 
 * File:   qpidavroUtils.h
 * Author: matt
 *
 * Created on June 30, 2011, 12:29 AM
 */

#ifndef QPIDAVROUTILS_H
#define	QPIDAVROUTILS_H

#include <avro/Specific.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include <qpid/messaging/Message.h>
#include "mechio/messaging/AvroQpidOutputStream.h"

using namespace std;
using namespace avro;
using namespace qpid::messaging;

template<typename T> T* unpackMessage(Message &message);
template<typename T> Message packMessage(T* t);

template<typename T> T* unpackMessage(Message &message){
    DecoderPtr d = binaryDecoder();
    auto_ptr<InputStream> is = memoryInputStream((const uint8_t*)message.getContentPtr(), message.getContentSize());
    d->init(*is);

    try{
        T* t = new T();
        decode(*d, *t);
        return t;
    }catch(const avro::Exception &ex){
        cout << "Error decoding avro: " << ex.what() << endl;
        return NULL;
    }
}

template<typename T> Message packMessage(T* t){
    auto_ptr<AvroQpidOutputStream> os = avroqpidOutputStream();
    EncoderPtr e = binaryEncoder();
    e->init(*os);

    avro::encode(*e, *t);
    e->flush();
    
    int count = os->byteCount();    
    char* data = new char[count];
    int i=0;
    for (std::vector<uint8_t*>::const_iterator it = os->data_.begin(); it != os->data_.end() && i<count; ++it) {
        uint8_t* chunk = *it;
        int size = os->chunkSize_;
        for(int j=0; j<size && i<count; j++, i++){
            data[i] = chunk[j];
        }
    }
    Message message;
    message.setContent(data, count);
    delete[] data;
    return message;
}

template<typename T> T* loadFromFile(const char* filename){
    DecoderPtr d = binaryDecoder();
    auto_ptr<InputStream> is = avro::fileInputStream(filename);
    d->init(*is);

    T* t = new T();
    avro::decode(*d, *t);
    return t;
}

template<typename T> void saveToFile(T* t, const char* filename){
    auto_ptr<OutputStream> os = avro::fileOutputStream(filename);
    EncoderPtr e = binaryEncoder();
    e->init(*os);

    avro::encode(*e, *t);
    e->flush();
}

#endif	/* QPIDAVROUTILS_H */

