//
//  main.cpp
//  zlib-client
//
//  Created by Dmitrii Belousov on 7/22/22.
//

#include <iostream>
#include <string>
#include <fstream>
#include "zlib.h"
#include "boost/algorithm/hex.hpp"

#define CHECK_ERR(err, msg) { \
	if (err != Z_OK) { \
		fprintf(stderr, "%s error: %d\n", msg, err); \
		exit(1); \
	} \
}

int main(int argc, const char * argv[]) {
  const uint64_t comprLen = 461u;
  const uint64_t uncomprLen = 10000;
  std::string comprStr = boost::algorithm::unhex(std::string("78DAED955B72C5300843B7902523D878DBD888C30EFA91C9DCDC3826204B3CF4C4938F7EEF7AAF7C7FF9D4EF15FEFFBB1FABB397AFFDB997BF89771DB6ADBB1BF651AFE5D9AF1BF9AC4F84F3BE9FDAF2C48DFBAC8BEB781D04717F696BAE1A59BC57DEB3A6D7896F641CB53C954F95F8B66E94F21B5D7C270A192CC4EA37C39EAE87844F01FD39EB20219ABA1665DC876DDDF7AD299508339E3E5359F3B81A853D274EA38572A2E5CA803E43FB18E5267746C93287859DC6A8FBDCB9236B153E41A3AAC51491E96A928BA16664D8A4BAB5140D33C96C6C2D64DB8E420603B1C27591A80CB9320ABEE58C0D20219A70A5843D87F99235A51232E3B5B223C0ECC49E0C175014EA2A9DCF932D025641B9C99D5132CC616087553835C94E259FA051C5628AC8587DACFCC037052DE949E85C64979991B6ED2864508825D470006DBAF2C6773A6305244423578AEC797A585A532A91663C567608CC4EECC9F0040A4E8F723E4FB624B026940B74B1D645E84982E554213B6EADCC0FE7AED00166552BCB653E6A750176BC5C9D275697D762979951B6ADD57913DD49508BEC7577E0648D354138D56AA1998992F63C3DACAC29952833FECDFF6FFE7FF3FF9BFFDFFCFFE6FF37FFFFD3FCFF01A2C870F1"));
  const char * comprP = comprStr.c_str();
  
  char uncomprP[uncomprLen] {};
  
  int err;
  z_stream d_stream;
  d_stream.zalloc = Z_NULL;
  d_stream.zfree = Z_NULL;
  d_stream.opaque = Z_NULL;
  
  d_stream.next_in = (Byte *)comprP;
  d_stream.avail_in = 0;
  d_stream.next_out = (Byte *)uncomprP;
  err = inflateInit(&d_stream);
  CHECK_ERR(err, "inflateInit");
  
  while (d_stream.total_out < uncomprLen && d_stream.total_in < comprLen) {
	d_stream.avail_in = d_stream.avail_out = 1;
	err = inflate(&d_stream, Z_NO_FLUSH);
	if (err == Z_STREAM_END)
	  break;
	CHECK_ERR(err, "inflate");
  }
  
  err = inflateEnd(&d_stream);
  CHECK_ERR(err, "inflateEnd");
  
  // Output to  file
  std::ofstream file("/opt/projects/zlib-client/zlib-client/Resources/decompressed.bin", std::ios::binary);
  if (file.is_open()) {
	file.write(uncomprP, uncomprLen);
	file.close();
  }
  
  return 0;
}
