var PORT = 1700;
var HOST = "10.65.4.147";

var dgram = require('dgram');
var message = new Buffer('My KungFu is Good!');
var AXISON = new Buffer([0x04, 0x00, 0x10, 0x01, 0x02, 0x17]);
var AXISOFF = new Buffer([0x04, 0x00, 0x10, 0x00, 0x02, 0x16]);

var client = dgram.createSocket('udp4');

client.send(AXISOFF, 0, AXISOFF.length, PORT, HOST, function (err, bytes) {
    if (err) throw err;
    console.log('UDP message sent to ' + HOST + ':' + PORT);
    client.close();
});

