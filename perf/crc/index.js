'use strict'

const crc64js = require('crc64-ecma182.js')
const crc64c = require('crc64-ecma182')
const fs = require('fs')

let buf = fs.readFileSync(__dirname + '/http2-explained-en.pdf')

let s, e

s  = new Date()
const retjs = crc64js.crc64(buf)
e = new Date()
let jsc = e - s
console.log('JS computation executed in: ' + jsc)

s = new Date()
const retc = crc64c.crc64(buf)
e = new Date()
let cc = e - s
console.log('JS computation executed in: ' + cc)