#include "byte_stream.hh"

#include <algorithm>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */

using namespace std;

ByteStream::ByteStream(const size_t capa) {
	_capacity = capa;
	_read = 0;
	_written = 0;
	buffer = {};
	_input_ended = false;
	_error = false;
}

size_t ByteStream::write(const string &data) {
	if (_input_ended) {
		return 0;
	}
	size_t length = data.length();
    if (length > remaining_capacity()) {
        length = remaining_capacity();
    }

	_written += length;
	for (size_t i = 0; i < length; i++) {
		buffer.push_back(data[i]);
	}
	return length;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
	size_t length = len;
	if (len > buffer.size()) {
		length = buffer.size();
	}
	
    string output(buffer.begin(), buffer.begin() + length);
    return output;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
	size_t length = len;
    if (length > buffer.size()) {
        set_error();
        return;
    }

    _read += length;
    for (size_t i = 0; i < length; i++) {
        buffer.pop_front();
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
	size_t  length = len;
	if (length > buffer.size()) {
		set_error();
		return "";				// set error and return an empty string
	}

    string output(buffer.begin(), buffer.begin() + length);
    _read += length;
    buffer.erase(buffer.begin(), buffer.begin() + length);

    return output;
}

void ByteStream::end_input() {
	_input_ended = true;
}

bool ByteStream::input_ended() const {
	return _input_ended;
}

size_t ByteStream::buffer_size() const {
	return buffer.size();
}

bool ByteStream::buffer_empty() const {
	return buffer.empty();
}

bool ByteStream::eof() const {
	return buffer.empty() && input_ended();
}

size_t ByteStream::bytes_written() const {
	return _written;
}

size_t ByteStream::bytes_read() const {
	return _read;
}

size_t ByteStream::remaining_capacity() const {
	return _capacity - buffer.size();
}