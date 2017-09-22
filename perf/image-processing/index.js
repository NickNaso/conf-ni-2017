'use strict'

const express = require('express')
const http = require('http')
const jimp = require('jimp')
const sharp = require('sharp')

const app = express()
// Set express server port
app.set('port', process.env.PORT || 5000)

app.get('/resize-from-js', (req, res, next) => {
    jimp.read('test.png', (err, img) => {
        if(err) {
            console.error('Error happened reading the image')
            next(err)
        }
        img.resize(303, 218)
        .write('./tmp/js-'+ (new Date().getTime()) + '.png', function() {
            res.status(200).end()
        })
    })
})

app.get('/resize-from-native', (req, res, next) => {
    sharp('test.png')
    .resize(303, 218)
    .toFile('./tmp/native-'+ (new Date().getTime()) + '.png')
    .then((data => {
        res.status(200).end()
    }))
    .catch(err => {
        console.error('Error happened reading the image')
        next(err)
    })

})

http.createServer(app).listen(app.get('port'), '0.0.0.0', () => {
    console.log("Server started on http://localhost:5000")
})