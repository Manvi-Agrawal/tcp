#include "stream_reassembler.hh"

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    :_output(capacity)
{
    _unassemble_strs = std::map<size_t, std::string>();;
    _unass_base = 0;
    _eof_idx = -1;
    _capacity = capacity;
}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t end_idx = index + data.size();
    if (_unass_base + _capacity <= index) {
        return;                                     // discard data (as outside sliding window)
    }

    size_t limit = min(end_idx, _output.bytes_read() + _capacity);
    size_t i = max(_unass_base, index);
    while (i < limit) {
        _unassemble_strs[i] = data[i - index];
        ++i;
    }
    
    string s = "";                                  // string to hold contiguous bytes
    while (_unassemble_strs.find(_unass_base) != _unassemble_strs.end()) {
        s += _unassemble_strs[_unass_base];         // add contiguous bytes to the string s
        auto it = _unassemble_strs.find(_unass_base);
        if (it != _unassemble_strs.end()) {
            _unassemble_strs.erase(it);
        }
        ++_unass_base;
    }
    _output.write(s);

    if (eof) {
        _eof_idx = end_idx;
    }
    if (_unass_base >= _eof_idx) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassemble_strs.size(); }

bool StreamReassembler::empty() const { return _unassemble_strs.empty(); }

size_t StreamReassembler::ack_index() const { return _unass_base; }