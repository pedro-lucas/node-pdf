{
    'targets': [
        {
            'target_name': 'node-pdf',
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            'sources': [
                'src/addon.cc',
                'src/node_pdf.cc'
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
