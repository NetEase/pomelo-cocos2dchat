pomelo-cocos2dchat
==================
This is a chat application based on [libpomelo](https://github.com/NetEase/libpomelo) and [cocos2d-x](https://github.com/cocos2d/cocos2d-x),  
the server side is [chatofpomelo-websocket](https://github.com/NetEase/chatofpomelo-websocket).  

##Windows  
open projects in proj.win32 with vistual studio and import libpomelo projects , then build and run    

##Android
use cocos2d-x create-android-project.bat(sh) to generate a new android project  
copy files in Classes and Resources into android project with the same directory  
put libpomelo in ${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt path  
replace Android.mk in the jni directory  
then run ./build_native.sh in your android project  

##IOS


##License
(The MIT License)

Copyright (c) 2012-2013 NetEase, Inc. and other contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
