{
    'targets': [
        {
            'target_name': 'node-pdf',
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            'sources': [
                'src/addon.cpp',
                'src/node_pdf.cpp'
            ],
            'cflags!': ['-fno-exceptions'],
            'cflags_cc!': ['-fno-exceptions'],
            'conditions': [
                ['OS=="mac"', {
                    'xcode_settings': {
                        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                    }
                }]
            ]
        }
    ]
}
