/* 
 * File:   QpidOutputStream.h
 * Author: matt
 *
 * Created on June 30, 2011, 5:41 PM
 */

#ifndef AVROQPIDOUTPUTSTREAM_H
#define	AVROQPIDOUTPUTSTREAM_H

#include <avro/Stream.hh>

using namespace std;
using namespace avro;

class AvroQpidOutputStream : public OutputStream {
    public:
        const size_t chunkSize_;
        std::vector<uint8_t*> data_;
        size_t available_;
        size_t byteCount_;

        AvroQpidOutputStream(size_t chunkSize);
        ~AvroQpidOutputStream();
        bool next(uint8_t** data, size_t* len);
        void backup(size_t len);
        uint64_t byteCount() const;
        void flush();
};

std::auto_ptr<AvroQpidOutputStream> avroqpidOutputStream(size_t chunkSize = 4 * 1024);

#endif	/* AVROQPIDOUTPUTSTREAM_H */

