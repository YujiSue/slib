{
  "targets": [
    {
      "target_name": "YOUR_APP_NAME", # <= Replace the name
      "sources": [ "main.cpp", "wrapper.cpp", ... ], # Add source files to compile
      "conditions": [        
        ['OS=="mac"',{
			'include_dirs': [ 
				'/usr/local/include/slib',
				...  # Add directories to search include headers
			],
			'libraries': [ 
				'/usr/local/lib/libsobj.so',
				'/usr/local/lib/libsnodeapp.so',
				... # Add libraries to link
			],
			'xcode_settings':{
				'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
				'CLANG_CXX_LIBRARY': 'libc++',
				'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
				... # Add build options
			}
		}],
		['OS=="win"', {
			'include_dirs': [
				'$HOME\\AppData\\Local\\slib\\include',
				'$CLASS_NAME',
				$INCLUDE_PATHS
			],
			'libraries': [ 
				'$HOME\\AppData\\Local\\slib\\lib\\static\\sobj.lib',
				'$HOME\\AppData\\Local\\slib\\lib\\static\\snodeapp.lib',
				$LIBRARY_PATHS
			],
			'msvs_settings': { 
			  'VCCLCompilerTool': {
			    'AdditionalOptions': [
			      '/utf-8', 
				  ... # Add build options
                ],
              },
			},
        }],
		['OS=="linux"',{
			'include_dirs': [ 
				'/usr/local/include/slib',
				...  # Add directories to search include headers
			],
			'libraries': [ 
				'/usr/local/lib/libsobj.so',
				'/usr/local/lib/libsnodeapp.so',
				... # Add libraries to link
			],
			'cflags_cc': [
				'-std=c++11',
				'-fexceptions',
				... # Add build options
			],
			'cflags_cc!': [
				'-fno-rtti',
				'-fno-exception',
				... # Add build options
			],
		}]
      ]
	}
  ]
}