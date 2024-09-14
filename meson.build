project('mdflash', 'c',
  version : '0.2.0',
  default_options : ['warning_level=3'])

gtk_dep = dependency('gtk+-3.0')

src = [
  'mdflash.c',
  'markdown_parser.c'
]

executable('mdflash',
           sources : src,
           dependencies : gtk_dep,
           install : true)
