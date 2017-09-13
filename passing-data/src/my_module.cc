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

 #include <nan.h>
 using v8::FunctionTemplate;

/**
* Sets the return value for myFunction
*/
NAN_METHOD(Hello) {
  info.GetReturnValue().Set(Nan::New("Hello World!").ToLocalChecked());
}

/**
* This code is our entry-point. We can receive up to two arguments here, the first
* is exports, the same as module.exports in a .js file and the second argument
* (omitted in this case) is module which is the same as module in a .js file.
* Normally you would attach properties to exports but you can use the module
* argument to replace its exports property so you are exporting a single thing,
* the equivalent of: module.exports = function () { ... }.
*/
NAN_MODULE_INIT(Init) {
    Nan::Set(target, Nan::New("hello").ToLocalChecked(), Nan::New<FunctionTemplate>(Hello)->GetFunction());
}

/**
* This code defines the entry-point for the Node addon, it tells Node where to go
* once the library has been loaded into active memory. The first argument must match
* the "target" in our *binding.gyp*. The second argument points to the function to invoke.
*/
NODE_MODULE(myModule, Init);