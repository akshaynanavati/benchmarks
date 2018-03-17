def FlagsForFile( filename, **kwargs ):
  return {
    'flags': [ '-x', 'c++', '-std=c++17', '-lbenchmark', '-I.'],
  }
