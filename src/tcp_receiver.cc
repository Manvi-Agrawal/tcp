#include "tcp_receiver.hh"

#include <algorithm>

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();
    // ...
    bool SYN = head.syn;
    bool FIN = head.fin;
    size_t segmentLength = seg.length_in_sequence_space();
    size_t expectedIndex;

    // convert the seqno into absolute seqno
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;

    // ... 
    if (SYN && !_synReceived) {                                 // if SYN flag is received and SYN was not received previously
        _synReceived = true;
        _isn = head.seqno;
        if (FIN && !_finReceived && segmentLength == 2) {       // if the segment has both SYN and FIN flags set with no payload
            _finReceived = true;
            _reassembler.push_substring("", 0, _finReceived);
            return;
        }
    }

    if (_synReceived) {                             // check if SYN flag is received yet
        string data = seg.payload().copy();
        if (FIN) {                                  // check if FIN flag is received yet
            _finReceived = true;
        }
        _reassembler.push_substring(data, stream_idx, FIN);
        expectedIndex = _reassembler.ack_index();
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    size_t expectedIndex = _reassembler.ack_index();
    if (_synReceived == false) {                    // if SYN flag is received till now (no handshaking)
        return nullopt;
    }
    if (_finReceived != true) {                     // if FIN flag is not received yet (connection is in data transfer state)
        return wrap(expectedIndex + 1, _isn);       // tell to send next next byte with sequence number: wrapped expectedIndex + 1
    }                  
    else {
        if (!_reassembler.empty()) {
            return wrap(expectedIndex + 1, _isn);
        }
        else {
            return wrap(expectedIndex + 2, _isn);   // here it is +2 to acknowledge both the last data byte and the FIN flag
        }
    }
}

size_t TCPReceiver::window_size() const {
    return (_capacity -  _reassembler.stream_out().buffer_size());
}
