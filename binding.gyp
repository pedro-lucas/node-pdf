{
    "targets": [
        {
            "target_name": "node_pdf",
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "sources": [
                "src/addon.cpp",
                "src/node_pdf.cpp",
                "src/node_pdf.h"
            ],
            "cflags": ["-Wall", "-std=c++11"],
            "conditions": [
                ["OS==\"mac\"", {
                    "xcode_settings": {
                        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                        "OTHER_CFLAGS": [
                            "-std=c++11"
                        ],
                    },
                    'include_dirs': [
                        'System/Library/Frameworks/CoreFoundation.Framework/Headers',
                        'System/Library/Frameworks/ApplicationServices.framework/Headers'
                    ],
                    'link_settings': {
                        'libraries': [
                            '-framework CoreFoundation',
                            '-framework ApplicationServices'
                        ]
                    }
                }]
            ]
        }
    ]
}
