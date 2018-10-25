var net = require('net');

var express = require('express')
  , app = express()
  , server = require('http').createServer(app)
  , path = require('path')
  , io = require('socket.io').listen(server)
  , spawn = require('child_process').spawn;


// all environments
app.set('port', process.env.TEST_PORT || 3000);
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(express.static(path.join(__dirname, 'public')));

server.listen(app.get('port'), function () {
    console.log('Fusion Pendant server is listening on port ' + app.get('port'));
});


var buffer = new Buffer([0xAA, 0x00, 0x00, 0x00, 0x00]);
var client = new net.Socket();

var HOST = '10.65.4.142'; 
var PORT = 700;

client.connect(700, '10.65.4.142', function(){
  console.log('CONNECTED TO: ' + HOST + ':' + PORT);
  // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client 
  client.write(buffer);
});

// Add a 'data' event handler for the client socket
// data is what the server sent to this socket
client.on('data', function(data) {

  console.log('DATA: ' + data);
  // Close the client socket completely
  //client.destroy();
});


// Add a 'close' event handler for the client socket
client.on('close', function() {
  console.log('Connection closed');
});


var myVar = setInterval(function(){ client.write(buffer)}, 1000);