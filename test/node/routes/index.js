'use strict';
var io = null;
var express = require('express');
var router = express.Router();
router.get('/', function (req, res) {
    res.render('index', { title: 'NodeAppTest'});
});
router.get('/api', function (req, res, next) {
    console.log('Called from client.');
    var obj = {msg:'From server to client.'};
    res.json(obj);
});
router.post('/api', function (req, res, next) {
    console.log('Called from client.');
    console.log('From client to server.\n'+JSON.stringify(req.body));
});
module.exports = router;


