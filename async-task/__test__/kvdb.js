/*******************************************************************************
 * Copyright (c) Nicola Del Gobbo
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the license at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS
 * OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY
 * IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
 * MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 *
 * Contributors - initial API implementation:
 * Nicola Del Gobbo <nicoladelgobbo@gmail.com>
 ******************************************************************************/

'use strict'

const KVDB = require('../')

process.chdir(__dirname)

const db = new KVDB.Database('test')

describe("Test kvdb module", function () {

    it('Should KVDB.Database to be a function', function () {
        expect(typeof KVDB.Database).toEqual('function')
    })

    it('Should set a key value on database', function () {
        expect(db.putKeySync('username', 'NickNaso')).toEqual(undefined)
    })

    it('Should retrieve a value from database', function () {
        expect(db.getKeySync('username')).toEqual('NickNaso')
    })

})
