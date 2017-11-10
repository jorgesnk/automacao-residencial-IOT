var mqtt = require('mqtt')
var express = require('express')
var client = mqtt.connect('mqtt://localhost:1883')
var app = express();

var http = require('http').Server(app);

app.get('/', function(req, res){
  res.send('<h1>Hello world</h1>');
});


http.listen(3000, function(){
  console.log('listening on *:3000');
});


client.on('connect', function () {
    client.subscribe('teste');
    client.subscribe('porta');
    client.subscribe('alarme');

})

client.on('message', function (topic, message) {
    console.log(message.toString());
    console.log(topic);
    if (topic == "alarme") {
    }

})
