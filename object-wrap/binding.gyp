{
  "targets": [
    {
      "target_name": "kvdb",
      "sources": [
        "src/binding.cc",
        "src/kvdb.cc"
        ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}