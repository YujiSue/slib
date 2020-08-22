'use strict';
var express = require('express');
var router = express.Router();
router.get('/', function (req, res) {
	res.render('index', { title: 'NodeAppTest'});
});
router.get('/api', function (req, res, next) {
    var obj = {msg:'From server to client.'};
    res.json(obj);
});
router.post('/api', function (req, res, next) {
    var rejson =console.log('From client to server.\n'+JSON.stringify(req.body));
});
module.exports = router;


