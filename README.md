# termite-jobs
Fast, multiplatform fiber based job dispatcher based on Naughty Dogs' GDC2015 talk.

## Features
- Lightweight, lib size is 39kb
- Fast, uses [libdeboost-context](https://github.com/septag/deboost.context) as it's fiber backend which is the fastest multiplatform co-routine implementation
- Multiplatform

## Build
Uses _cmake_ as it's build system, all you need is install _cmake_ and run the command :
```
cd termite-jobs
mkdir .build
cd .build
cmake ..
```

Use specific cmake flags for your compiler/build tools
