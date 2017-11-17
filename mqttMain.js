var mqtt = require('mqtt')
var express = require('express')
var client = mqtt.connect('mqtt://localhost:1883')
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function (req, res) {
    res.send('<h1>Hello world</h1>');
});


client.on('connect', function () {
    client.subscribe('teste');
    client.subscribe('porta');
    client.subscribe('alarme');

})

io.on('connection', (socket) => {
    socket.on('alarmestatus', (dados) => {
        var comando = {
            comando: ""
        };
        if (dados.comando == "desativar") {
            comando.comando = 'desativar';
            client.publish('alarme', JSON.stringify(comando));
        }
        else {
            comando.comando = 'ativar';
            client.publish('alarme', JSON.stringify(comando));
        }
    });

    socket.on('porta', (dados) => {
        if (dados.comando == "Abrir") {
            const comando = { comando: "Abrir" };
            client.publish('porta', JSON.stringify(comando))
        }
        else {
            const comando = { comando: "Fechar" };
            client.publish('porta', JSON.stringify(comando))
        }
    })
    client.on('message', (topic, message) => {
        console.log(message.toString());
        if (topic == "alarme") {
            if (message.toString() == "tocar") {
                const comando = {
                    status: "ativar"
                };
                socket.emit('alarmestatus', comando);
            }
        }
    })
});

http.listen(3000, function () {
    console.log('listening on *:3000');
});







