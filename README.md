pomelo-cocos2dchat
==================
This is a chat application based on [libpomelo](https://github.com/NetEase/libpomelo) and [cocos2d-x](https://github.com/cocos2d/cocos2d-x),  
the server side is [chatofpomelo-websocket](https://github.com/NetEase/chatofpomelo-websocket).  

##Windows  
open projects in proj.win32 with vistual studio and import libpomelo projects , then build and run    

##Android
* Use cocos2d-x create-android-project.bat(sh) to generate a new android project.
* Copy files in Classes and Resources to the android.
* Put libpomelo in`${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt` path.  
* Replace `Android.mk` in the jni directory.  
* Run `./build_native.sh` in your android project.

##IOS

* Create a cocos2d-x project in xcode.
* Copy files in `Classes/` and `Resources` to the project.
* Build the libaries of `libpomelo` for [ios](https://github.com/NetEase/libpomelo#ios) or [ios simulator](https://github.com/NetEase/libpomelo#ios-simulator).
* Configure project
	* Add include path: add `LIBPOMELO_ROOT/include`, `LIBPOMELO_ROOT/deps/uv/incude` and `LIBPOMELO_ROOT/deps/jansson/src` to the `User Header Search Paths` of project.
	* Add library path: add `libpomelo` libraries paths to the `Library Search Paths` of project. For example: `LIBPOMELO_ROOT/build/Default-iphonesimulator`, `LIBPOMELO_ROOT/deps/uv/build/Default-iphonesimulator` and `LIBPOMELO_ROOT/deps/jansson/build/Default-iphonesimulator`.
	* Add linker flags: add `-ljansson`, `-luv` and `-lpomelo` to the `Other Linker Flags` of project. 


##License
(The MIT License)

Copyright (c) 2012-2013 NetEase, Inc. and other contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
