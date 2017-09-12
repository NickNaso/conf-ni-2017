'use strict'

const bcryptjs = require('bcryptjs')
const bcrypt = require('bcrypt')

let s, e

s  = new Date()
let saltjs = bcryptjs.genSaltSync(20)
let hashjs = bcryptjs.hashSync('ilovenodejs', saltjs)
e = new Date()
let jsc = e - s
console.log('JS computation executed in: ' + jsc)

s = new Date()
let saltc = bcrypt.genSaltSync(20)
let hashc = bcrypt.hashSync('ilovenodejs', saltc)
e = new Date()
let cc = e - s
console.log('JS computation executed in: ' + cc)