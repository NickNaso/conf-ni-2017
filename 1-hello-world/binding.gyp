{
  "targets": [
    {
      "target_name": "myModule",
      "sources": ["src/my_module.cc"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}