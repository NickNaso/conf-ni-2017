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

#include "kvdb.h"
#include <iostream>
#include <sstream>

using namespace v8;


class GetKeyWorker: public Nan::AsyncWorker {
  public:
    GetKeyWorker(Nan::Callback *callback, vedis *db, std::string cmd )
      :AsyncWorker(callback), db(db), cmd(cmd) {}
    ~GetKeyWorker() {}

    void Execute() {
      int rc;
      rc = vedis_exec(db, cmd.c_str(), -1);
      if(rc != VEDIS_OK) { 
        // Handle error
      } 
      /* Extract the return value of the last executed command (i.e. 'GET test') " */
      vedis_value *result;
      vedis_exec_result(db, &result);
      /* Cast the vedis object to a string */
      value = vedis_value_to_string(result, 0);
    }

    void HandleOKCallback() {
      Nan::HandleScope();
      int argc = 2;
      Local<Value> argv[2];
      argv[0] = Nan::Null();
      argv[1] = Nan::New(value).ToLocalChecked();
      callback->Call(argc, argv);
    }

  private:
    vedis *db; 
    std::string cmd;
    const char *value;
};

class PutKeyWorker: public Nan::AsyncWorker {
  public:
    PutKeyWorker(Nan::Callback *callback, vedis *db, std::string cmd)
      :AsyncWorker(callback), db(db), cmd(cmd) {}
    ~PutKeyWorker() {}

    void Execute() {
      int rc;
      rc = vedis_exec(db, cmd.c_str(), -1);
      if(rc != VEDIS_OK) { 
        // Handle error
      }
    }

    void HandleOKCallback() {
      Nan::HandleScope();
      int argc = 1;
      Local<Value> argv[1];
      argv[0] = Nan::Null();
      callback->Call(argc, argv);
    }

  private:
    vedis *db; 
    std::string cmd; 
};


namespace KVDB {

  NAN_MODULE_INIT(Database::Init) {
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Database").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    Nan::SetPrototypeMethod(tpl, "getKey", GetKey);
    Nan::SetPrototypeMethod(tpl, "getKeySync", GetKeySync);
    Nan::SetPrototypeMethod(tpl, "putKey", PutKey);
    Nan::SetPrototypeMethod(tpl, "putKeySync", PutKeySync);
    // Only if you have accessor method
    Local<ObjectTemplate> itpl = tpl->InstanceTemplate();
    Nan::SetAccessor(itpl, Nan::New("db_name").ToLocalChecked(), DbName);
    constructor().Reset(v8::Isolate::GetCurrent(), Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("Database").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
  }

  NAN_METHOD(Database::New) {
    // Here we need some control 
    String::Utf8Value tmpDbName(info[0]->ToString());
    std::string dbName(*tmpDbName);
    if (info.IsConstructCall()) {
      KVDB::Database *database = new KVDB::Database(dbName);
      database->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      const int argc = 1;
      Local<Value> argv[argc] = {info[0]};
      Local<Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }   
  }

  NAN_METHOD(Database::GetKey) {
      // Here we need some control
      String::Utf8Value tmpKey(info[0]->ToString());
      Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());
      std::string key(*tmpKey);
      std::stringstream cmd;
      cmd << "GET " + key;
      KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
      AsyncQueueWorker(new GetKeyWorker(callback, database->db, cmd.str()));
      info.GetReturnValue().SetUndefined();
  }

  NAN_METHOD(Database::GetKeySync) {
    // Here we need some control
    String::Utf8Value tmpKey(info[0]->ToString());
    std::string key(*tmpKey);
    std::stringstream cmd;
    cmd << "GET " + key;
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    int rc;
    rc = vedis_exec(database->db, (cmd.str()).c_str(), -1);
    if(rc != VEDIS_OK) { 
      // Handle error
    } 
    /* Extract the return value of the last executed command (i.e. 'GET test') " */
    vedis_value *result;
    vedis_exec_result(database->db, &result);
    const char *value;
    /* Cast the vedis object to a string */
    value = vedis_value_to_string(result, 0);
    info.GetReturnValue().Set(Nan::New(value).ToLocalChecked());
  }

  NAN_METHOD(Database::PutKey) {
     // Here we need some control
     String::Utf8Value tmpKey(info[0]->ToString());
     std::string key(*tmpKey);
     String::Utf8Value tmpValue(info[1]->ToString());
     std::string value(*tmpValue);
     std::stringstream cmd;
     cmd << "SET " + key + " '" + value + "'";
     Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());
     KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
     AsyncQueueWorker(new GetKeyWorker(callback, database->db, cmd.str()));
     info.GetReturnValue().SetUndefined();
  }

  NAN_METHOD(Database::PutKeySync) {
    // Here we need some control
    String::Utf8Value tmpKey(info[0]->ToString());
    std::string key(*tmpKey);
    String::Utf8Value tmpValue(info[1]->ToString());
    std::string value(*tmpValue);
    std::stringstream cmd;
    cmd << "SET " + key + " '" + value + "'";
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    int rc;
    rc = vedis_exec(database->db, (cmd.str()).c_str(), -1);
    if (rc != VEDIS_OK) {
      // Hanlde  error
    }
    info.GetReturnValue().SetUndefined();
  }

  NAN_GETTER(Database::DbName) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    info.GetReturnValue().Set(Nan::New(database->db_name).ToLocalChecked());
  }
  
  inline Persistent<v8::Function> & Database::constructor() {
    static Persistent<v8::Function> kvdb_constructor;
    return kvdb_constructor;
  }

  Database::Database(std::string db_name) {
    this->db_name = db_name;
    std::stringstream db_path;
    db_path << root_path << "/" << db_name << db_extension;
    int rc;
    rc = vedis_open(&(this->db), (db_path.str()).c_str());
    //rc = vedis_open(&(this->db), NULL);
    if (rc != VEDIS_OK) {
      // Hanlde the initialization error
    }
  }

  Database::~Database() {
    // NOOP
  }

}