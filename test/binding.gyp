{
  "targets": [
    {
      "target_name": "NodeAppTest",
      "sources": [ "NodeAppTestMain.cpp", "NodeAppTest.cpp" ],
      "conditions": [        
        ['OS=="mac"',{
			'include_dirs': [ 
				'/usr/local/include/slib',
			],
			'libraries': [ 
				'/usr/local/lib/libsobj.so',
				'/usr/local/lib/libsnodeapp.so'
			],
			'xcode_settings':{
				'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
				'CLANG_CXX_LIBRARY': 'libc++',
				'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
			}
		}],
		['OS=="win"', {
			'include_dirs': [
				'C:\\ProgramData\\slib\\include',
			],
			'libraries': [ 
				'C:\\ProgramData\\slib\\lib\\static\\sobj.lib',
				'C:\\ProgramData\\slib\\lib\\static\\snodeapp.lib'
			],
			'msvs_settings': { 
			  'VCCLCompilerTool': {
			    'AdditionalOptions': [
			      '/utf-8', '/sdl-'
                ],
              },
			},
        }],
		['OS=="linux"',{
			'include_dirs': [ 
				'/usr/local/include/slib',
			],
			'libraries': [ 
				'/usr/local/lib/libsobj.so',
				'/usr/local/lib/libsnodeapp.so'
			],
			'cflags_cc': [
				'-std=c++11',
				'-fexceptions'
			],
			'cflags_cc!': [
				'-fno-rtti',
				'-fno-exception'
			],
		}]
      ]
	}
  ]
}