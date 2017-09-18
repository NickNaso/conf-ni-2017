{
  "targets": [
    {
      "target_name": "kvdb",
      "sources": [
        "src/binding.cc",
        "src/kvdb.cc",
        "src/deps/vedis.c"
        ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": {
        "CLANG_CXX_LIBRARY": "libc++"
      }
    }
  ]
}