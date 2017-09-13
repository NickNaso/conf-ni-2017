{
  "targets": [
    {
      "target_name": "kvdb",
      "sources": ["src/binding.cc"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}