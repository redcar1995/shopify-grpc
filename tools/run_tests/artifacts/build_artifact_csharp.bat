@rem Copyright 2016, Google Inc.
@rem All rights reserved.
@rem
@rem Redistribution and use in source and binary forms, with or without
@rem modification, are permitted provided that the following conditions are
@rem met:
@rem
@rem     * Redistributions of source code must retain the above copyright
@rem notice, this list of conditions and the following disclaimer.
@rem     * Redistributions in binary form must reproduce the above
@rem copyright notice, this list of conditions and the following disclaimer
@rem in the documentation and/or other materials provided with the
@rem distribution.
@rem     * Neither the name of Google Inc. nor the names of its
@rem contributors may be used to endorse or promote products derived from
@rem this software without specific prior written permission.
@rem
@rem THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
@rem "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
@rem LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
@rem A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
@rem OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
@rem SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
@rem LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
@rem DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
@rem THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@rem (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
@rem OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

@rem Builds C# artifacts on Windows

set ARCHITECTURE=%1

@call tools\run_tests\helper_scripts\pre_build_csharp.bat %ARCHITECTURE% || goto :error

cd cmake\build\%ARCHITECTURE%
cmake --build . --target grpc_csharp_ext --config Release
cd ..\..\..

mkdir -p %ARTIFACTS_OUT%
copy /Y cmake\build\Win32\Release\grpc_csharp_ext.dll %ARTIFACTS_OUT% || copy /Y cmake\build\x64\Release\grpc_csharp_ext.dll %ARTIFACTS_OUT% || goto :error

goto :EOF

:error
echo Failed!
exit /b %errorlevel%
