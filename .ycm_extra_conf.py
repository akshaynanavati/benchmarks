def FlagsForFile( filename, **kwargs ):
  return {
    'flags': ['-E', '-v', '-x', 'c++', '-std=c++17', '-stdlib=libstdc++', '-lbenchmark', '-I.', '-L$HOME/clang-lib', '-Ideps/sparsehash/build/include', '-Ideps/falcon/include'],
  }
