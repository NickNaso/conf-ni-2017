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

using namespace Napi;

class GetKeyWorker: public Napi::AsyncWorker {
  public:
    GetKeyWorker(Napi::FunctionReference *callback, vedis *db, std::string cmd )
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

    void OnOK() {
      Napi::HandleScope();
      int argc = 2;
      Napi::Value argv[2];
      argv[0] = env.Null();
      argv[1] = Napi::New(env, value);
      callback->Call(argc, argv);
    }

  private:
    vedis *db; 
    std::string cmd;
    const char *value;
};

class PutKeyWorker: public Napi::AsyncWorker {
  public:
    PutKeyWorker(Napi::FunctionReference *callback, vedis *db, std::string cmd)
      :AsyncWorker(callback), db(db), cmd(cmd) {}
    ~PutKeyWorker() {}

    void Execute() {
      int rc;
      rc = vedis_exec(db, cmd.c_str(), -1);
      if(rc != VEDIS_OK) { 
        // Handle error
      }
    }

    void OnOK() {
      Napi::HandleScope();
      int argc = 1;
      Napi::Value argv[1];
      argv[0] = env.Null();
      callback->Call(argc, argv);
    }

  private:
    vedis *db; 
    std::string cmd; 
};

void buffer_delete_callback(char* data, void* hint) {
  free(data);
}

class GetKeyBufferWorker: public Napi::AsyncWorker {
  public:
    GetKeyBufferWorker(Napi::FunctionReference *callback, vedis *db, std::string key)
      :AsyncWorker(callback), db(db), key(key) {}
    ~GetKeyBufferWorker() {}

    void Execute() {
      int rc;

      std::stringstream strlenCmd;
      strlenCmd << "STRLEN " + key;
      rc = vedis_exec(db, (strlenCmd.str()).c_str(), -1);
      if(rc != VEDIS_OK) { 
        // Handle error
      }
      /* Extract the return value of the last executed command (i.e. 'STRLEN test') " */
      vedis_value *strlen_result;
      vedis_exec_result(db, &strlen_result);
      /* Cast the vedis object to a string */
      this->buffer_length = vedis_value_to_int(strlen_result); 

      std::stringstream get_cmd;
      get_cmd << "GET " + key;
      rc = vedis_exec(db, (get_cmd.str()).c_str(), -1);
      if(rc != VEDIS_OK) { 
        // Handle error
      } 
      /* Extract the return value of the last executed command (i.e. 'GET test') " */
      vedis_value *get_result;
      vedis_exec_result(db, &get_result);
      /* Cast the vedis object to a string */
      this->buffer = static_cast<char *>(malloc(this->buffer_length));
      this->buffer = const_cast<char *>(vedis_value_to_string(get_result, 0));
      
    }

    void OnOK() {
      Napi::HandleScope();
      int argc = 2;
      Napi::Value argv[2];
      argv[0] = env.Null();
      // Error violation memory
      //argv[1] = Napi::Buffer<char>::New(env, this->buffer, this->buffer_length);
      // Not efficient solution
      //argv[1] = Napi::Buffer::Copy(env, this->buffer, this->buffer_length);
      Napi::Object data = 
      Napi::Buffer<char>::New(env, this->buffer, this->buffer_length, buffer_delete_callback, this->buffer);
      argv[1] = data;
      callback->Call(argc, argv);
    }
  private:
    vedis *db; 
    std::string key;
    char *buffer;
    int buffer_length;
};

class PutKeyBufferWorker: public Napi::AsyncWorker {
  public:
    PutKeyBufferWorker(Napi::FunctionReference *callback, vedis *db, std::string key, Napi::Object &value)
      :AsyncWorker(callback), db(db), key(key) {
        this->buffer = value.As<Napi::Buffer<char>>().Data();
        this->buffer_length = value.As<Napi::Buffer<char>>().Length();
      }
    ~PutKeyBufferWorker() {}

    void Execute() {
      int rc;
      //rc = vedis_exec(db, cmd.c_str(), -1);
      rc = vedis_kv_store(db, key.c_str(), -1, this->buffer, this->buffer_length);
      if(rc != VEDIS_OK) { 
        // Handle error
        std::cout << "Error: " << rc;
      }
    }

    void OnOK() {
      Napi::HandleScope();
      int argc = 1;
      Napi::Value argv[1];
      argv[0] = env.Null();
      callback->Call(argc, argv);
    }

  private:
    vedis *db; 
    std::string key; 
    char *buffer;
    int buffer_length;
};

namespace KVDB {

  void Database::Init(Napi::Env env, Napi::Object exports, Napi::Object module) {
    Napi::FunctionReference tpl = Napi::Function::New(env, New);
    tpl->SetClassName(Napi::String::New(env, "Database"));

      InstanceMethod("getKey", &GetKey),
      InstanceMethod("getKeyBuffer", &GetKeyBuffer),
      InstanceMethod("getKeySync", &GetKeySync),
      InstanceMethod("putKey", &PutKey),
      InstanceMethod("putKeyBuffer", &PutKeyBuffer),
      InstanceMethod("putKeySync", &PutKeySync),
    // Only if you have accessor method
    Local<ObjectTemplate> itpl = tpl->InstanceTemplate();
    Napi::SetAccessor(itpl, Napi::String::New(env, "db_name"), DbName);
    constructor().Reset(v8::Isolate::GetCurrent(), Napi::GetFunction(tpl));
    (target).Set(Napi::String::New(env, "Database"), Napi::GetFunction(tpl));
  }

  Napi::Value Database::New(const Napi::CallbackInfo& info) {
    // Here we need some control 
    Napi::String tmpDbName(env, info[0].ToString());
    std::string dbName(*tmpDbName);
    if (info.IsConstructCall()) {
      KVDB::Database *database = new KVDB::Database(dbName);
      database->Wrap(info.This());
      return info.This();
    } else {
      const int argc = 1;
      Napi::Value argv[argc] = {info[0]};
      Napi::Function cons = Napi::New(env, constructor());
      return Napi::NewInstance(cons, argc, argv);
    }   
  }

  Napi::Value Database::GetKey(const Napi::CallbackInfo& info) {
      // Here we need some control
      Napi::String tmpKey(env, info[0].ToString());
      Napi::FunctionReference *callback = new Napi::FunctionReference(info[1].As<Napi::Function>());
      std::string key(*tmpKey);
      std::stringstream cmd;
      cmd << "GET " + key;
      KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
      AsyncQueueWorker(new GetKeyWorker(callback, database->db, cmd.str()));
      return env.Undefined();
  }

  Napi::Value Database::GetKeyBuffer(const Napi::CallbackInfo& info) {
    // Here we need some control
    Napi::String tmpKey(env, info[0].ToString());
    Napi::FunctionReference *callback = new Napi::FunctionReference(info[1].As<Napi::Function>());
    std::string key(*tmpKey);
    //std::stringstream cmd;
    //cmd << "GET " + key;
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    AsyncQueueWorker(new GetKeyBufferWorker(callback, database->db, key));
    return env.Undefined();
  }

  Napi::Value Database::GetKeySync(const Napi::CallbackInfo& info) {
    // Here we need some control
    Napi::String tmpKey(env, info[0].ToString());
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
    return Napi::New(env, value);
  }

  Napi::Value Database::PutKey(const Napi::CallbackInfo& info) {
    // Here we need some control
    Napi::String tmpKey(env, info[0].ToString());
    std::string key(*tmpKey);
    Napi::String tmpValue(env, info[1].ToString());
    std::string value(*tmpValue);
    std::stringstream cmd;
    cmd << "SET " + key + " '" + value + "'";
    Napi::FunctionReference *callback = new Napi::FunctionReference(info[2].As<Napi::Function>());
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    AsyncQueueWorker(new PutKeyWorker(callback, database->db, cmd.str()));
    return env.Undefined();
 }

  Napi::Value Database::PutKeyBuffer(const Napi::CallbackInfo& info) {
     // Here we need some control
     Napi::String tmpKey(env, info[0].ToString());
     std::string key(*tmpKey);
     //Napi::String tmpValue(env, info[1].ToString());
     //std::string value(*tmpValue);
     //std::stringstream cmd;
     //cmd << "SET " + key + " '" + value + "'";
     Napi::Object value = info[1].ToObject();
     Napi::FunctionReference *callback = new Napi::FunctionReference(info[2].As<Napi::Function>());
     KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
     AsyncQueueWorker(new PutKeyBufferWorker(callback, database->db, key, value));
     return env.Undefined();
  }

  Napi::Value Database::PutKeySync(const Napi::CallbackInfo& info) {
    // Here we need some control
    Napi::String tmpKey(env, info[0].ToString());
    std::string key(*tmpKey);
    Napi::String tmpValue(env, info[1].ToString());
    std::string value(*tmpValue);
    std::stringstream cmd;
    cmd << "SET " + key + " '" + value + "'";
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    int rc;
    rc = vedis_exec(database->db, (cmd.str()).c_str(), -1);
    if (rc != VEDIS_OK) {
      // Hanlde  error
    }
    return env.Undefined();
  }

  Napi::Value Database::DbName(const Napi::CallbackInfo& info) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    return Napi::New(env, database->db_name);
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