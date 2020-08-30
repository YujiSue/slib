'use strict';
var debug = require('debug');
var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var session = require('express-session');
global.os = null;
if(process.platform==='darwin') global.os = 'macOS';
if(process.platform==='win32') global.os = 'winOS';
if(process.platform==='linux') global.os = 'linuxOS';
/* Native addon test */
var addon = null;
if(global.os==='macOS') addon = require('./native-addon/mac/NodeAppTest.node');
if(global.os==='winOS') addon = require('./native-addon/win/NodeAppTest.node');
if(global.os==='linux') addon = require('./native-addon/linux/NodeAppTest.node');
var plugin = new addon.NodeAppTest();
console.log('NodeAppTest::TestNum\t'+plugin.TestNum(100));
console.log('NodeAppTest::TestStr\t'+plugin.TestStr('abc','xyz'));
console.log('NodeAppTest::TestArrayI\t'+plugin.TestArrayI([10,3.14,'def']));
console.log('NodeAppTest::TestArrayO\t'+JSON.stringify(plugin.TestArrayO(20,2.71828,'uvw')));
console.log('NodeAppTest::TestObjI\t'+plugin.TestObjI({key1:10,key2:3.14,key3:'def'}));
console.log('NodeAppTest::TestObjO\t'+JSON.stringify(plugin.TestObjO('key4',20,'key5',2.71828,'key6','uvw')));
/*********************/
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var preprocess = require('./routes/preprocess');
var routes = require('./routes/index');

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'pug');
//app.use(favicon(__dirname + '/public/favicon.ico'));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(session({
    secret: 'slib-test',
    resave: false,
    saveUninitialized: false
}));
app.use('/', preprocess, routes);
app.use(function (req, res, next) {
	console.log(req.connection.remoteAddress);
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});
app.use(function (err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
        message: err.message,
        error: {}
    });
});

app.set('socketio', io);
app.set('port', process.env.PORT || 3000);
server.listen(app.get('port'), function () {
    console.log('Express server listening on port ' + server.address().port);
});