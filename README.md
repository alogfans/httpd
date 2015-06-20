Simple HTTP Server
---

Copyright 2015 alogfans, all rights reserved.

This program is a simple HTTP server, which is capable for processing GET requests
only. Users could upload static contents (e.g. HTML file, images and binary files)
to the directory of `wwwroot`, and executing the server. The server used threading 
blocking-IO currently, so it could handle about 1000 requests every second.

Use GNU C/C++ compiler toolchain to compile it. (because I used the syntax like `__thread` 
to implemented thread-local storage)
