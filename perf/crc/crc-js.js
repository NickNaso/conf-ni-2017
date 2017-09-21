'use strict'

const crc64js = require('crc64-ecma182.js')
const fs = require('fs')

const NS_PER_SEC = 1e9

let buf, time, retjs, diff, elapsed

buf = fs.readFileSync(__dirname + '/' + '131072.txt')
time = process.hrtime();
retjs = crc64js.crc64(buf)
diff = process.hrtime(time)
elapsed = diff[0]* NS_PER_SEC + diff[1]
console.log(elapsed)


