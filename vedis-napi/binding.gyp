{
  "targets": [
    {
      "target_name": "kvdb",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        "src/binding.cc",
        "src/kvdb.cc",
        "src/deps/vedis.c"
        ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      "xcode_settings": {
        "CLANG_CXX_LIBRARY": "libc++",
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
      }
    }
  ]
}