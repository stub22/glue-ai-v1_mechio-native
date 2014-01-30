#include "mechio/messaging/AvroQpidOutputStream.h"

AvroQpidOutputStream::AvroQpidOutputStream(size_t chunkSize) : chunkSize_(chunkSize),
            available_(0), byteCount_(0) { }

AvroQpidOutputStream::~AvroQpidOutputStream() {
    for (std::vector<uint8_t*>::const_iterator it = data_.begin();
        it != data_.end(); ++it) {
        delete[] *it;
    }
    data_.clear();
}

bool AvroQpidOutputStream::next(uint8_t** data, size_t* len) {
    if (available_ == 0) {
        data_.push_back(new uint8_t[chunkSize_]);
        available_ = chunkSize_;
    }
    *data = &data_.back()[chunkSize_ - available_];
    *len = available_;
    byteCount_ += available_;
    available_ = 0;
    return true;
}

void AvroQpidOutputStream::backup(size_t len) {
    available_ += len;
    byteCount_ -= len;
}

uint64_t AvroQpidOutputStream::byteCount() const {
    return byteCount_;
}

void AvroQpidOutputStream::flush() { }

std::auto_ptr<AvroQpidOutputStream> avroqpidOutputStream(size_t chunkSize)
{
    return std::auto_ptr<AvroQpidOutputStream>(new AvroQpidOutputStream(chunkSize));
}

