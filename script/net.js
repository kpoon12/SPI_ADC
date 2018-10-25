var net = require('net');

var buffer = new Buffer([0xAA, 0x00, 0x00, 0x00, 0x00]);
var client = new net.Socket();

var HOST = '10.65.4.142'; 
var PORT = 700;
var Counter = 0;

client.connect(700, '10.65.4.142', function(){
  console.log('CONNECTED TO: ' + HOST + ':' + PORT);
  // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client 
  client.write(buffer);
});

// Add a 'data' event handler for the client socket
// data is what the server sent to this socket
client.on('data', function(data) {
  var u16Bytes = data.buffer.slice(0, 2);
  var uint = new Uint16Array(u16Bytes)[0];
  console.log(Counter.toString() + 'DATA: ' + uint.toString());
  Counter++;
  if(Counter == 500)
  {// Close the client socket completely
    client.destroy();
  }
});


// Add a 'close' event handler for the client socket
client.on('close', function() {
  clearInterval(this.myVer);
  console.log('Connection closed');
});


var myVar = setInterval(function(){ client.write(buffer); }, 1000);