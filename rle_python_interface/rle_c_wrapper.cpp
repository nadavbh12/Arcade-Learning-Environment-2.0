#include "rle_c_wrapper.h"

#include <cstring>
#include <string>
#include <stdexcept>

//void encodeState(RLEState *state, char *buf, int buf_len) {
//	std::string str = state->serialize();
//
//	if (buf_len < int(str.length())) {
//		throw new std::runtime_error("Buffer is not big enough to hold serialized RLEState. Please use encodeStateLen to determine the correct buffer size");
//	}
//
//	memcpy(buf, str.data(), str.length());
//}
//
//int encodeStateLen(RLEState *state) {
//	return state->serialize().length();
//}
//
//RLEState *decodeState(const char *serialized, int len) {
//	std::string str(serialized, len);
//
//	return new RLEState(str);
//}


static void rle_rearrangeRgb(uint8_t *dst_buffer, const pixel_t *src_buffer, size_t src_size , const RLEInterface *rle) {
  uint8_t *q = dst_buffer;
  uint8_t r, g, b;
  for(size_t i = 0; i < src_size; ++i, ++q){
  	rle->getScreen().getRGB(((uint16_t*)src_buffer)[i], r, g, b);
      *q = (unsigned char) (r);  q++;
      *q = (unsigned char) (g);  q++;
      *q = (unsigned char) (b);  q++;
  }
}

static void rle_rearrangeGrayscale(uint8_t *dst_buffer, const uint32_t *src_buffer, size_t src_size , const RLEInterface *rle) {
  uint8_t *q = dst_buffer;
  uint8_t r, g, b;
  for(size_t i = 0; i < src_size; ++i, ++q){
  	rle->getScreen().getRGB(((uint16_t*)src_buffer)[i], r, g, b);
    *q = (unsigned char) (0.3*r + 0.5*g + 0.2*b);
  }
}

void getScreenRGB(RLEInterface *rle, unsigned char *output_buffer){
  const rle::RLEScreen& screen = rle->getScreen();
  size_t w = screen.width();
  size_t h = screen.height();
  size_t size_in_pixels = w*h;
  rle_rearrangeRgb(output_buffer, screen.getArray(), size_in_pixels, rle);
}

void getScreenRGBWithDims(RLEInterface *rle, unsigned char *output_buffer, size_t width, size_t height){
  const rle::RLEScreen& screen = rle->getScreen();
  size_t w = width;
  size_t h = height;
  size_t size_in_pixels = w*h;
  rle_rearrangeRgb(output_buffer, screen.getArray(), size_in_pixels, rle);
}

//This method should receive an empty vector to fill it with
//the grayscale colors
void getScreenGrayscale(RLEInterface *rle, unsigned char *output_buffer){
  const rle::RLEScreen& screen = rle->getScreen();
  size_t w = rle->getScreen().width();
  size_t h = rle->getScreen().height();
  size_t size_in_pixels = w*h;
  rle_rearrangeGrayscale(output_buffer, (uint32_t*)screen.getArray(), size_in_pixels, rle);
}

void getScreenGrayscaleWithDims(RLEInterface *rle, unsigned char *output_buffer, size_t width, size_t height){
  const rle::RLEScreen& screen = rle->getScreen();
  size_t w = width;
  size_t h = height;
  size_t size_in_pixels = w*h;
  rle_rearrangeGrayscale(output_buffer, (uint32_t*)screen.getArray(), size_in_pixels, rle);
}

void getRAM(RLEInterface *rle,unsigned char *ram){
  unsigned char* rle_ram = rle->getRAM().array();
  int size = rle->getRAM().size();
  memcpy(ram, rle_ram, size*sizeof(unsigned char));
}
