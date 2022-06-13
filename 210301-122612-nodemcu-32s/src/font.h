//汉字字模
const unsigned char xing[] = {
  0x00, 0x00, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x80, 0x00, 0x88, 0x00,
  0xF8, 0x1F, 0x84, 0x00, 0x82, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0x80, 0x00, 0xFE, 0x3F, 0x00, 0x00
};  /*星*/
const unsigned char qi[] = {
  0x44, 0x00, 0x44, 0x3E, 0xFE, 0x22, 0x44, 0x22, 0x44, 0x22, 0x7C, 0x3E, 0x44, 0x22, 0x44, 0x22,
  0x7C, 0x22, 0x44, 0x3E, 0x44, 0x22, 0xFF, 0x22, 0x20, 0x21, 0x44, 0x21, 0x82, 0x28, 0x41, 0x10
};  /*期*/
const unsigned char Year1[] = {
  0x04, 0x00, 0x04, 0x00, 0xFC, 0x07, 0x42, 0x00, 0x41, 0x00, 0xFC, 0x03,
  0x44, 0x00, 0x44, 0x00, 0xFF, 0x07, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00
};/*"年",0*/

const unsigned char yue[] = {
  0x00, 0x00, 0xFC, 0x03, 0x04, 0x02, 0x04, 0x02, 0xFC, 0x03, 0x04, 0x02,
  0x04, 0x02, 0xFC, 0x03, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x81, 0x03
};/*"月",0*/
const unsigned char ri[] = {
  0x00, 0x00, 0xFE, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x03,
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x03, 0x02, 0x02
};/*"日"*/

const unsigned char yi[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};/*"一"*/

const unsigned char er[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};/*"二",0*/

const unsigned char san[] = {
  0x00, 0x00, 0x00, 0x00, 0xFE, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1F,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00
};/*"三",1*/

const unsigned char si[] = {
  0x00, 0x00, 0x00, 0x00, 0xFE, 0x3F, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
  0x12, 0x22, 0x12, 0x3C, 0x0A, 0x20, 0x06, 0x20, 0x02, 0x20, 0xFE, 0x3F, 0x02, 0x20, 0x00, 0x00
};/*"四"*/

const unsigned char wu[] = {
  0x00, 0x00, 0xFE, 0x3F, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0xFC, 0x0F, 0x20, 0x08,
  0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0xFF, 0x7F, 0x00, 0x00
};/*"五"*/

const unsigned char liu[] = {
  0x40, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x02, 0x20, 0x04, 0x10, 0x08, 0x10, 0x10, 0x08, 0x10, 0x04, 0x20, 0x02, 0x20, 0x00, 0x00
};/*"六"*/

const unsigned char shi[] = {
  0x01, 0x00, 0xFA, 0x03, 0x08, 0x02, 0xF8, 0x03, 0x09, 0x02, 0xFA, 0x03,
  0xA0, 0x00, 0xA4, 0x04, 0xA8, 0x02, 0xA2, 0x00, 0xA1, 0x00, 0xFC, 0x07
};/*"湿"*/

const unsigned char du[] = {
  0x40, 0x00, 0xFE, 0x07, 0x12, 0x01, 0xFE, 0x07, 0x12, 0x01, 0xF2, 0x01,
  0x02, 0x00, 0xFA, 0x03, 0x12, 0x02, 0x22, 0x01, 0xC2, 0x00, 0x39, 0x07
};/*"度"*/

const unsigned char li[] = {
  0x00, 0x28, 0xFE, 0xFE, 0x28, 0xAA, 0x28, 0xFE, 0xFE, 0xAA, 0xAA, 0xFE, 0xAA, 0x00, 0xAA, 0xFE,
  0xAE, 0x00, 0xC2, 0xFE, 0x82, 0x82, 0xFE, 0xFE, 0x82, 0x44, 0x82, 0x28, 0xFE, 0xFE, 0x82, 0x00
};/*"醴"*/

const unsigned char ling[] = {
  0x00, 0x08, 0x3C, 0x08, 0x24, 0x7F, 0x14, 0x08, 0x14, 0x08, 0x8C, 0xFF, 0x14, 0x22, 0x24, 0x45,
  0xA4, 0x84, 0x24, 0x3E, 0x2C, 0x23, 0x94, 0x14, 0x04, 0x08, 0x04, 0x14, 0x04, 0x23, 0xC4, 0xC0
};/*"陵"*/

const unsigned char hu[32] = {
  0x80, 0x00, 0x84, 0x78, 0x88, 0x48, 0xE8, 0x4B, 0x81, 0x48, 0x82, 0x78, 0x82, 0x48, 0xE8, 0x4B,
  0x28, 0x4A, 0x24, 0x7A, 0x27, 0x4A, 0xE4, 0x4B, 0x24, 0x4A, 0x04, 0x44, 0x04, 0x54, 0x00, 0x22
};/*"湖",0*/

const unsigned char nan1[] = {
  0x00, 0x01, 0x00, 0x01, 0xFE, 0xFF, 0x00, 0x01, 0x00, 0x01, 0xFC, 0x7F, 0x24, 0x48, 0x44, 0x44,
  0xE4, 0x4F, 0x04, 0x41, 0x04, 0x41, 0xF4, 0x5F, 0x04, 0x41, 0x04, 0x41, 0x04, 0x51, 0x04, 0x20
};/*"南",0*/

const unsigned char nong[] = {
  0x20, 0x00, 0x20, 0x00, 0xFF, 0x07, 0x11, 0x04, 0x31, 0x04, 0x28, 0x00,
  0x48, 0x02, 0x4C, 0x01, 0x8A, 0x00, 0x29, 0x01, 0x18, 0x06, 0x08, 0x00
};/*"农",0*/

const unsigned char li2[] = {
  0x00, 0x00, 0xFE, 0x07, 0x22, 0x00, 0x22, 0x00, 0xFA, 0x03, 0x22, 0x02,
  0x22, 0x02, 0x22, 0x02, 0x12, 0x02, 0x12, 0x02, 0x0A, 0x02, 0xC5, 0x01
};/*"历",0*/

const unsigned char Jin[] = {
  0x20, 0x00, 0x50, 0x00, 0x88, 0x00, 0x04, 0x01, 0x13, 0x06, 0x20, 0x00,
  0x00, 0x00, 0xFE, 0x01, 0x00, 0x01, 0x80, 0x00, 0x40, 0x00, 0x20, 0x00
};/*"今",0*/

const unsigned char Ming[] = {
  0xDF, 0x07, 0x51, 0x04, 0x51, 0x04, 0xD1, 0x07, 0x5F, 0x04, 0x51, 0x04,
  0xD1, 0x07, 0x51, 0x04, 0x5F, 0x04, 0x41, 0x04, 0x20, 0x04, 0x10, 0x07
};/*"明",0*/

const unsigned char Hou[] = {
  0x80, 0x03, 0x7C, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFC, 0x07, 0x04, 0x00,
  0x04, 0x00, 0xF4, 0x07, 0x14, 0x04, 0x12, 0x04, 0xF2, 0x07, 0x11, 0x04
};/*"后",0*/

const unsigned char Tian[]= {
  0x00, 0x00, 0xFE, 0x03, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFF, 0x07,
  0x20, 0x00, 0x50, 0x00, 0x50, 0x00, 0x88, 0x00, 0x04, 0x01, 0x03, 0x06
};/*"天",0*/
const unsigned char hunan1[]= {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF2, 0xFF,
  0xFF, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x02, 0x00, 0x00, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x7F, 0x02, 0x00, 0x00, 0x60, 0xFE, 0xCF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x7F, 0xFE, 0xFF,
  0xFF, 0x7F, 0xFE, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xE3, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xC1,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x8F, 0xFF, 0xFF,
  0xFF, 0xFF, 0xF1, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x27, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xE4, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x67, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xE6,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xFC, 0xFF, 0xFF,
  0xFF, 0xFF, 0x3F, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x85, 0x73, 0x01, 0x88, 0xE6, 0x81, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x65, 0x0C, 0xFB,
  0xDF, 0x30, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0x25, 0x08, 0xFB, 0xCF, 0x10, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x25, 0x09, 0xFB, 0xCF, 0x10, 0x86, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0x25, 0x09, 0xFB,
  0xDF, 0x10, 0x86, 0xFF, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F,
  0xFF, 0x25, 0x09, 0xFB, 0xDF, 0x10, 0x86, 0xFF, 0xF8, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x1F, 0xFC, 0x25, 0x08, 0xF9, 0xCF, 0x10, 0x84, 0x3F,
  0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0xF0, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0F, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F,
  0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x30, 0xC0, 0xF9, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x3F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
  0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x7F, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
  0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x9F, 0xFF, 0xFB, 0xDF, 0xFF, 0xF9, 0xE7,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x9F, 0x83, 0xFB,
  0xDF, 0xC1, 0xF9, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xE7, 0x9F, 0x83, 0xFB, 0xCF, 0xC1, 0xF9, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x9F, 0x93, 0xFB, 0xCF, 0xC9, 0xF9, 0xE7,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xFF, 0xE7, 0x9F, 0x93, 0xFB,
  0xDF, 0xC9, 0xF9, 0xE7, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF,
  0xE7, 0x9F, 0x93, 0xFB, 0xDF, 0xC9, 0xF9, 0xE7, 0xFF, 0xFC, 0xFF, 0xFF,
  0xFF, 0xFF, 0x3F, 0xFC, 0xF7, 0x9F, 0x93, 0xFB, 0xDF, 0xC9, 0xF9, 0xE7,
  0x3F, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xF0, 0xE7, 0x9F, 0x93, 0xF1,
  0x8F, 0xC9, 0xF9, 0xE7, 0x0F, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x8F, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0xFE,
  0x3F, 0x00, 0x00, 0xC0, 0xE7, 0x9F, 0xFF, 0xFB, 0xCF, 0xFF, 0xF9, 0xE7,
  0x03, 0x00, 0x00, 0xFC, 0x1F, 0xF9, 0xFF, 0xE7, 0xE7, 0x9F, 0xFF, 0xFB,
  0xDF, 0xFF, 0xF9, 0xE7, 0x07, 0x00, 0x9B, 0xF8, 0x8F, 0xFF, 0xFF, 0xFF,
  0xE7, 0x9F, 0x83, 0xFB, 0xCF, 0xC1, 0xF9, 0xE7, 0xFF, 0xFF, 0xFF, 0xF9,
  0xCF, 0xFF, 0xFF, 0xFF, 0xE7, 0x9F, 0x83, 0xFB, 0xCF, 0xC1, 0xF9, 0xE7,
  0xFF, 0xFF, 0xFF, 0xF3, 0xE7, 0xFF, 0xFF, 0xFF, 0xE7, 0x9F, 0x93, 0xFB,
  0xDF, 0xC9, 0xF9, 0xE7, 0xFF, 0xFF, 0xFF, 0xE3, 0x07, 0x00, 0x00, 0xE0,
  0xE7, 0x9F, 0x93, 0xFB, 0xDF, 0xC9, 0xF9, 0xE7, 0x03, 0x00, 0x00, 0xE0,
  0x03, 0x00, 0x00, 0xC0, 0xE7, 0x9F, 0x93, 0xFB, 0xDF, 0xC9, 0xF9, 0xE7,
  0x03, 0x00, 0x00, 0xC0, 0x3F, 0x9F, 0x9F, 0xCF, 0xE7, 0x9F, 0x93, 0xF9,
  0xDF, 0xC9, 0xF9, 0xE7, 0xF3, 0xF3, 0xF9, 0xFC, 0x3F, 0x9F, 0xDF, 0x0F,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF3, 0xF9, 0xFC,
  0x3F, 0x9F, 0xDF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xF0, 0xF3, 0xF9, 0xFC, 0x3F, 0x9F, 0xDF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xF3, 0xF9, 0xFC, 0x3F, 0x9F, 0xDF, 0xCF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xF3, 0xF9, 0xFC,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF
};//地标logo
const unsigned char hunan2[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00,
  0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFD, 0xFF, 0xFF, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0C, 0x80, 0xFD, 0xFF, 0xFF, 0x9F, 0x01, 0x30,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x80, 0x01, 0x00,
  0x00, 0x80, 0x01, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x38, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3C, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x3E,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x70, 0x00, 0x00,
  0x00, 0x00, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xD8, 0x3F, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x1B, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x98, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x19,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00,
  0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x8C, 0xFE, 0x77, 0x19, 0x7E, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0xF3, 0x04,
  0x20, 0xCF, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xDA, 0xF7, 0x04, 0x30, 0xEF, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 0xF6, 0x04, 0x30, 0xEF, 0x79, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0xDA, 0xF6, 0x04,
  0x20, 0xEF, 0x79, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
  0x00, 0xDA, 0xF6, 0x04, 0x20, 0xEF, 0x79, 0x00, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xE0, 0x03, 0xDA, 0xF7, 0x06, 0x30, 0xEF, 0x7B, 0xC0,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0F, 0xFB, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xF0, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
  0xFE, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0x3F, 0x06, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xC0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x00, 0x04, 0x20, 0x00, 0x06, 0x18,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x7C, 0x04,
  0x20, 0x3E, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x18, 0x60, 0x7C, 0x04, 0x30, 0x3E, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0x6C, 0x04, 0x30, 0x36, 0x06, 0x18,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x18, 0x60, 0x6C, 0x04,
  0x20, 0x36, 0x06, 0x18, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00,
  0x18, 0x60, 0x6C, 0x04, 0x20, 0x36, 0x06, 0x18, 0x00, 0x03, 0x00, 0x00,
  0x00, 0x00, 0xC0, 0x03, 0x08, 0x60, 0x6C, 0x04, 0x20, 0x36, 0x06, 0x18,
  0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x18, 0x60, 0x6C, 0x0E,
  0x70, 0x36, 0x06, 0x18, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xCF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0xFF, 0x01,
  0xC0, 0xFF, 0xFF, 0x3F, 0x18, 0x60, 0x00, 0x04, 0x30, 0x00, 0x06, 0x18,
  0xFC, 0xFF, 0xFF, 0x03, 0xE0, 0x06, 0x00, 0x18, 0x18, 0x60, 0x00, 0x04,
  0x20, 0x00, 0x06, 0x18, 0xF8, 0xFF, 0x64, 0x07, 0x70, 0x00, 0x00, 0x00,
  0x18, 0x60, 0x7C, 0x04, 0x30, 0x3E, 0x06, 0x18, 0x00, 0x00, 0x00, 0x06,
  0x30, 0x00, 0x00, 0x00, 0x18, 0x60, 0x7C, 0x04, 0x30, 0x3E, 0x06, 0x18,
  0x00, 0x00, 0x00, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x18, 0x60, 0x6C, 0x04,
  0x20, 0x36, 0x06, 0x18, 0x00, 0x00, 0x00, 0x1C, 0xF8, 0xFF, 0xFF, 0x1F,
  0x18, 0x60, 0x6C, 0x04, 0x20, 0x36, 0x06, 0x18, 0xFC, 0xFF, 0xFF, 0x1F,
  0xFC, 0xFF, 0xFF, 0x3F, 0x18, 0x60, 0x6C, 0x04, 0x20, 0x36, 0x06, 0x18,
  0xFC, 0xFF, 0xFF, 0x3F, 0xC0, 0x60, 0x60, 0x30, 0x18, 0x60, 0x6C, 0x06,
  0x20, 0x36, 0x06, 0x18, 0x0C, 0x0C, 0x06, 0x03, 0xC0, 0x60, 0x20, 0xF0,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x0C, 0x06, 0x03,
  0xC0, 0x60, 0x20, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x0F, 0x0C, 0x06, 0x03, 0xC0, 0x60, 0x20, 0x30, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06, 0x03, 0xC0, 0x60, 0x20, 0x30,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x06, 0x03,
  0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};//地标logo反色