{
  "targets": [
    {
      "target_name": "kvdb",
      "sources": ["src/kvdb.cc"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      'xcode_settings': {
          'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
          'CLANG_CXX_LIBRARY': 'libc++'
      }
    }
  ]
}