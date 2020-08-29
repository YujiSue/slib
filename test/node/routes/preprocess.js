'use strict';
var express = require('express');
var router = express.Router();
var io = null;
function initSocketIO() {
    io.on('connection', function(socket){
        console.log('socket connected.');
        socket.on('sender', function(msg){
            console.log(msg);
            io.emit('notify', 'socket received.');
        });
    });
}
module.exports = function(req, res, next) {
    var app = req.app;
    if (req.session) console.log(req.session.id);
    if(!io) {
        io = app.get('socketio');
        initSocketIO();
    }
    next();
};